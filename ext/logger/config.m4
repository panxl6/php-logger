dnl $Id$
dnl config.m4 for extension logger

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(logger, for logger support,
dnl Make sure that the comment is aligned:
dnl [  --with-logger             Include logger support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(logger, whether to enable logger support,
dnl Make sure that the comment is aligned:
[  --enable-logger           Enable logger support])

if test "$PHP_LOGGER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-logger -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/logger.h"  # you most likely want to change this
  dnl if test -r $PHP_LOGGER/$SEARCH_FOR; then # path given as parameter
  dnl   LOGGER_DIR=$PHP_LOGGER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for logger files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       LOGGER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$LOGGER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the logger distribution])
  dnl fi

  dnl # --with-logger -> add include path
  dnl PHP_ADD_INCLUDE($LOGGER_DIR/include)

  dnl # --with-logger -> check for lib and symbol presence
  dnl LIBNAME=logger # you may want to change this
  dnl LIBSYMBOL=logger # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LOGGER_DIR/$PHP_LIBDIR, LOGGER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LOGGERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong logger lib version or lib not found])
  dnl ],[
  dnl   -L$LOGGER_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(LOGGER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(logger, logger.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
