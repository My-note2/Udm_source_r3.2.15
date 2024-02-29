#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([GOCL], [1.00])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([GOCL], [1.00])
AC_CONFIG_SRCDIR([src/OCLParserStatic.cpp])
AC_DISABLE_STATIC
AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_EGREP

AC_ARG_WITH(antlr,
	AC_HELP_STRING([--with-antlr=PATH],
			[Name of antlr (version 1.33) from PCCTS [[default=antlr]]]),
	[antlr_name="$withval"],
	[antlr_name="antlr"])
AC_CHECK_PROG(antlr, $antlr_name, $antlr_name)
if test x$antlr = x; then
   AC_MSG_ERROR([$antlr_name not found])
fi
AC_MSG_CHECKING([if $antlr_name is version 1.33])
if $antlr 2>&1 | $EGREP -iq 'Antlr parser generator   Version 1.33'; then
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
   AC_MSG_ERROR([antlr version 1.33 is needed])
fi

AC_CHECK_PROG(dlg, dlg, dlg)
if test x$dlg = x; then
   AC_MSG_ERROR([dlg not found])
fi

# Checks for libraries.

# Checks for header files.
AC_ARG_WITH(pccts-dir,
	AC_HELP_STRING([--with-pccts-dir=PATH],
			[Directory containing PCCTS headers and sources [[default=/usr/include/pccts]]]),
	[pcctsdir="$withval"],
	[pcctsdir="/usr/include/pccts"])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.

AC_SUBST([antlr])
AC_SUBST([dlg])
AC_SUBST([pcctsdir])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
