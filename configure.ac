#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([fcitx-imlist], [0.2.0], [kenhys@gmail.com])
AC_CONFIG_SRCDIR([src/fcitximlist.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign tar-pax])

# Checks for programs.
AC_PROG_CC

PKG_CHECK_MODULES([GLIB], glib-2.0 >= 2.26)
PKG_CHECK_MODULES([GIO], gio-2.0)
PKG_CHECK_MODULES([FCITX_GCLIENT], fcitx-gclient)

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
	Makefile		
	src/Makefile		
])

AC_OUTPUT
