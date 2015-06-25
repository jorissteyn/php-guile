
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_guile.h"

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

/* {{{ guile_functions[]
 *
 * Every user visible function must have an entry in guile_functions[].
 */
const zend_function_entry guile_functions[] = {
	PHP_FE(confirm_guile_compiled,	NULL)		/* For testing, remove later. */
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
