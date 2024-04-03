#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([Wizard], [0.9], [erickfarestveit@gmail.com])
AC_CONFIG_SRCDIR([Wizard.hpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX

PKG_CHECK_MODULES([GINAC], [ginac >= 1.8.6])
PKG_CHECK_MODULES([CLN], [cln >= 1.3.6])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
