#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([Aorta], [2.1], [http://sf.net/tracker/?group_id=179330&atid=888719])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memset pow sqrt])

# Checks for libsquish.
AC_CHECK_LIB([squish], [main], [founda=yes], [founda=no])
AC_LANG_PUSH([C++])
AC_CHECK_HEADER([squish.h], [foundb=yes], [foundb=no])
AC_LANG_POP()
if test "x$founda" = xno || test "x$foundb" = xno; then
	AC_MSG_ERROR([libsquish is missing])
fi

# check wxWigets.
AM_OPTIONS_WXCONFIG  
reqwx=2.8.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)

if test "$wxWin" != 1; then
	AC_MSG_ERROR([wxWidgets is missing.])
else
	CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
	CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
	CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
	LIBS="$LIBS $WX_LIBS"
fi

# add some windows goodies
case $target in
*-*-mingw32*)
	make_windows=true
	;;
*)
	make_windows=false
	;;
esac
AM_CONDITIONAL(MAKE_WINDOWS, test x$make_windows = xtrue)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "Configuration done. Now type \"make\"."
