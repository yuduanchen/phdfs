dnl
dnl $ Id: $
dnl

PHP_ARG_ENABLE(phdfs, whether to enable phdfs functions,
[  --enable-phdfs         Enable phdfs support])

if test "$PHP_PHDFS" != "no"; then
  export OLD_CPPFLAGS="$CPPFLAGS"
  export CPPFLAGS="$CPPFLAGS $INCLUDES -DHAVE_PHDFS "
    if test $(getconf LONG_BIT) == '64'  ; then
       HADOOP_DIR=" -I ${HADOOP_HOME}/src/c++/libhdfs -I ${JAVA_HOME}/include -I ${JAVA_HOME}/include/linux -L${HADOOP_HOME}/c++/Linux-amd64-64/lib -lhdfs -L${JAVA_HOME}/jre/lib/amd64/server -ljvm"
    else
        HADOOP_DIR=" -I ${HADOOP_HOME}/src/c++/libhdfs -I ${JAVA_HOME}/include -I ${JAVA_HOME}/include/linux -L${HADOOP_HOME}/c++/Linux-i386-32/lib -lhdfs -L${JAVA_HOME}/jre/lib/i386/server -ljvm"
    fi
  AC_MSG_CHECKING(PHP version)
  AC_TRY_COMPILE([#include <php_version.h>], [
#if PHP_VERSION_ID < 50000
#error  this extension requires at least PHP version 5.0.0
#endif
],
[AC_MSG_RESULT(ok)],
[AC_MSG_ERROR([need at least PHP 5.0.0])])

  export CPPFLAGS="$OLD_CPPFLAGS $HADOOP_DIR"
  PHP_SUBST(PHDFS_SHARED_LIBADD)
  AC_DEFINE(HAVE_PHDFS, 1, [ ])

  PHP_NEW_EXTENSION(phdfs, phdfs.c , $ext_shared)

fi

