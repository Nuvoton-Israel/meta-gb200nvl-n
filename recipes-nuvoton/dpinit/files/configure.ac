#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
#AC_INIT(bbexample, 0.1, ajlennon@dynamicdevices.co.uk)

AC_INIT([dpinit], [0.1])
AC_LANG([C++])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Wno-portability -Werror foreign dist-xz])


#AC_CONFIG_SRCDIR([bbexample.c])
#AM_INIT_AUTOMAKE(bbexample, main)

# Checks for programs.
AC_PROG_CXX
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
#AM_PROG_LIBTOOL


PKG_CHECK_MODULES([PHOSPHOR_LOGGING], [phosphor-logging])
PKG_CHECK_MODULES(
    [SDBUSPLUS],
    [sdbusplus],
    [],
    [AC_MSG_ERROR(["sdbusplus required and not found"])]
)

# Set shared libraries
#AC_DISABLE_STATIC
#AC_ENABLE_SHARED

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

#AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
