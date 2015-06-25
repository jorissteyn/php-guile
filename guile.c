
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_guile.h"

#include <libguile.h>

/* If you declare any globals in php_guile.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(guile)
*/

/* True global resources - no need for thread safety here */
static int le_guile;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("guile.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_guile_globals, guile_globals)
    STD_PHP_INI_ENTRY("guile.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_guile_globals, guile_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_guile_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_guile_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "guile", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_guile_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_guile_init_globals(zend_guile_globals *guile_globals)
{
	guile_globals->global_value = 0;
	guile_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(guile)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(guile)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(guile)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(guile)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(guile)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "guile support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto void guile_init()
   { */
PHP_FUNCTION(guile_init)
{
    scm_init_guile();

    RETURN_NULL();
}
/* }}} */

/* {{{ proto void guile_eval()
   { */
PHP_FUNCTION(guile_eval)
{
    int argc = ZEND_NUM_ARGS();
    char* data = NULL;
    int data_len = 0;

    if (zend_parse_parameters(argc TSRMLS_CC, "s", &data, &data_len) != SUCCESS)
		return;

    zval *retval = NULL;
    ALLOC_INIT_ZVAL(retval);

    SCM scmval = scm_c_eval_string(data);

    // We have our return value now. Map it to C data types, then map
    // it to PHP values.
    if (php_guile_scm_to_zval(&scmval, retval) == FAILURE) {
        zend_error(E_ERROR, "Unable to convert SCM to ZVAL");
    }

    // TODO: Should we copy? and dtor?
    RETURN_ZVAL(retval, 1, 1);
}

/* }}} */

/* {{{ zval guile_scm_to_zval()
   { */
int php_guile_scm_to_zval(SCM *scmval, zval *retval)
{
    // Convert all supported types. Returns SUCCESS, or FAILURE if the
    // SCM could not be fully converted to a zval.

    // Strings.
    if (scm_is_string(*scmval)) {
        char *strval = scm_to_locale_string(*scmval);

        ZVAL_STRING(retval, strval, 1);

        return SUCCESS;
    }

    // Integers.
    if (scm_is_integer(*scmval)) {
        long intval = scm_to_long(*scmval);

        ZVAL_LONG(retval, intval);

        return SUCCESS;
    }

    // Floating-point numbers.
    if (scm_is_real(*scmval)) {
        double dblval = scm_to_double(*scmval);

        ZVAL_DOUBLE(retval, dblval);

        return SUCCESS;
    }

    //
    // And finally... LISTS!
    //
    if (scm_is_pair(*scmval)) {
        // Null, not nil.
        if (scm_is_null_and_not_nil(*scmval)) {
            ZVAL_NULL(retval);

            return SUCCESS;
        }

        HashTable listval = {0};
        zend_hash_init(&listval, 0, retval, ZVAL_PTR_DTOR, 0);

        // Nil and not null, nil is an empty list.
        if (scm_is_null(scmval)) {
            return SUCCESS;
        }

        SCM carscm = scm_car(scmval);
        SCM cdrscm = scm_cdr(scmval);

        zval *carval = NULL;
        zval *cdrval = NULL;

        ALLOC_INIT_ZVAL(carval);
        ALLOC_INIT_ZVAL(cdrval);

        if (php_guile_scm_to_zval(&carscm, &carval) == FAILURE) {
            return FAILURE;
        }

        if (php_guile_scm_to_zval(&cdrscm, &cdrval) == FAILURE) {
            return FAILURE;
        }

        // TODO: segfaults! See tests/006.phpt.
        if (zend_hash_add(&listval, 0, sizeof(0), &carval, sizeof(zval*), NULL) == FAILURE) {
            return FAILURE;
        }

        if (zend_hash_add(&listval, 1, sizeof(0), &cdrval, sizeof(zval*), NULL) == FAILURE) {
            return FAILURE;
        }

        return SUCCESS;
    }

    // TODO: Impelement linked lists and all the other types.

    // Unsupported data type.
    return FAILURE;
}
/* }}} */

/* {{{ guile_functions[]
 *
 * Every user visible function must have an entry in guile_functions[].
 */
const zend_function_entry guile_functions[] = {
    PHP_FE(guile_init,	NULL)
    PHP_FE(guile_eval,	NULL)
    PHP_FE_END	/* Must be the last line in guile_functions[] */
};
/* }}} */

/* {{{ guile_module_entry
 */
zend_module_entry guile_module_entry = {
	STANDARD_MODULE_HEADER,
	"guile",
	guile_functions,
	PHP_MINIT(guile),
	PHP_MSHUTDOWN(guile),
	PHP_RINIT(guile),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(guile),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(guile),
	PHP_GUILE_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GUILE
ZEND_GET_MODULE(guile)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
