#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([Aorta], [2.1], [http://sf.net/tracker/?group_id=179330&atid=888719])

AC_CONFIG_MACRO_DIR([m4])
AC_PREREQ([2.65])

dnl Detect the canonical host and target build environment.
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl Init automake.
AM_INIT_AUTOMAKE([foreign])
AM_CONFIG_HEADER([config.h])

dnl Check for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_OBJCXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memset pow sqrt])

dnl Some platform specific stuff.
AS_CASE([$target],
        [*-*-mingw32*],
        [ make_windows=true
          LDFLAGS="$LDFLAGS -Wl,-subsystem,windows"
          LIBS="$LIBS -lstdc++" ],
)

dnl Checks for libsquish.
PKG_CHECK_MODULES([SQUISH], [libsquish],[
    CXXFLAGS="$CXXFLAGS $SQUISH_CFLAGS -fopenmp"
    LIBS="$LIBS $SQUISH_LIBS"
    ],
    AC_MSG_ERROR("libsquish not found")
)

dnl check wxWigets.
AM_OPTIONS_WXCONFIG
reqwx=3.0.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)

if test "$wxWin" != 1; then
   AC_MSG_ERROR([wxWidgets is missing.])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY -std=gnu++11"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

AM_CONDITIONAL([MAKE_WINDOWS], [test "x$make_windows" = "xtrue"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "Configuration done. Now type \"make\"."
