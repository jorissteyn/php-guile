dnl $Id$
dnl config.m4 for extension guile

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(guile, for guile support,
[  --with-guile             Include guile support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(guile, whether to enable guile support,
dnl Make sure that the comment is aligned:
dnl [  --enable-guile           Enable guile support])

if test "$PHP_GUILE" != "no"; then
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/guile/2.0/libguile.h"
  if test -r $PHP_GUILE/$SEARCH_FOR; then # path given as parameter
     GUILE_DIR=$PHP_GUILE
  else # search default path list
     AC_MSG_CHECKING([for guile files in default path])
     for i in $SEARCH_PATH ; do
         if test -r $i/$SEARCH_FOR; then
             GUILE_DIR=$i
             AC_MSG_RESULT(found in $i)
         fi
      done
  fi

  if test -z "$GUILE_DIR"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR([Please reinstall the guile distribution])
  fi

  PHP_ADD_INCLUDE($GUILE_DIR/include/guile/2.0)

  LIBNAME=guile-2.0
  LIBSYMBOL=scm_version

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
      PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $GUILE_DIR/$PHP_LIBDIR, GUILE_SHARED_LIBADD)
      AC_DEFINE(HAVE_GUILELIB,1,[ ])
  ],[
      AC_MSG_ERROR([wrong libguile version or libguile 2.0 not found])
  ],[
      -L$GUILE_DIR/$PHP_LIBDIR/guile/2.0 -lm
  ])

  PHP_SUBST(GUILE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(guile, guile.c, $ext_shared)
fi
