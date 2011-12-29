#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([myftp], [0.1], [silverzhaojr@gmail.com])
AC_CONFIG_SRCDIR([src/ftp_server/preprocess_path.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([crypt], [crypt])
AC_CHECK_LIB([pthread], [pthread_create])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([getcwd memset mkdir rmdir socket])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/common/Makefile
                 src/ftp_client/Makefile
                 src/ftp_server/Makefile])
AC_OUTPUT
