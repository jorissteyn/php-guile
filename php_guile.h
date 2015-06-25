
#ifndef PHP_GUILE_H
#define PHP_GUILE_H

#include <libguile.h>

extern zend_module_entry guile_module_entry;
#define phpext_guile_ptr &guile_module_entry

#define PHP_GUILE_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_GUILE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_GUILE_API __attribute__ ((visibility("default")))
#else
#	define PHP_GUILE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(guile)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(guile)
*/

/* In every utility function you add that needs to use variables 
   in php_guile_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as GUILE_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define GUILE_G(v) TSRMG(guile_globals_id, zend_guile_globals *, v)
#else
#define GUILE_G(v) (guile_globals.v)
#endif

int php_guile_scm_to_zval(SCM *scmval, zval *retval);

#endif	/* PHP_GUILE_H */

