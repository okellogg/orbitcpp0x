dnl Process this file with autoconf to produce a configure script.
AC_INIT(configure.in)

AC_PREREQ(2.12) # required version of autoconf

# Before making a release, the LT_VERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A

LT_VERSION=10:0:0
AC_SUBST(LT_VERSION)

ORBITCPP_MAJOR_VERSION=1
ORBITCPP_MINOR_VERSION=3
ORBITCPP_MICRO_VERSION=10
ORBITCPP_VERSION=$ORBITCPP_MAJOR_VERSION.$ORBITCPP_MINOR_VERSION.$ORBITCPP_MICRO_VERSION
AC_SUBST(ORBITCPP_MAJOR_VERSION)
AC_SUBST(ORBITCPP_MINOR_VERSION)
AC_SUBST(ORBITCPP_MICRO_VERSION)
AC_SUBST(ORBITCPP_VERSION)


# For automake.

AM_CONFIG_HEADER(config.h)

dnl Initialize automake stuff
AM_INIT_AUTOMAKE(orbitcpp, $ORBITCPP_VERSION, no-define)

AC_LIBTOOL_DLOPEN
AM_PROG_LIBTOOL
AM_MAINTAINER_MODE

dnl Useful for automating stuff
AC_CACHE_CHECK([for aclocal flags], ac_cv_orbit_aclocal_flags,[
   ac_cv_orbit_aclocal_flags="$ACLOCAL_FLAGS"
])
ACLOCAL="$ACLOCAL $ac_cv_orbit_aclocal_flags"

AC_CANONICAL_HOST
AC_MSG_CHECKING([for Win32])
case "$host" in
  *-*-mingw*)
    os_win32=yes
    MINGW_LDFLAGS="-Wl,--enable-runtime-pseudo-reloc"
    ;;
  *)
    os_win32=no
    MINGW_LDFLAGS=
    ;;
esac
AC_MSG_RESULT([$os_win32])

AM_CONDITIONAL(OS_WIN32, [test "$os_win32" = "yes"])

AC_SUBST(MINGW_LDFLAGS)
LDFLAGS="$MINGW_LDFLAGS $LDFLAGS"

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

dnl --enable-comile-warnings
AC_ARG_ENABLE(compile-warnings, [  --enable-compile-warnings=[no/yes] Turn on compiler warnings.], [enable_compile_warnings="$enableval"],[enable_compile_warnings="yes"])

# AC_PROG_AWK

REQUIRED_VERSION_ORBIT=2.5.0
REQUIRED_VERSION_LIBIDL=0.7.4
AC_SUBST(REQUIRED_VERSION_ORBIT)
AC_SUBST(REQUIRED_VERSION_LIBIDL)

dnl Checks for libraries.
PKG_CHECK_MODULES(ORBITCPP, ORBit-2.0 >= $REQUIRED_VERSION_ORBIT)
AC_SUBST(ORBITCPP_LIBS)
AC_SUBST(ORBITCPP_CFLAGS)

PKG_CHECK_MODULES(ORBITCPP_IDL, ORBit-2.0 >= $REQUIRED_VERSION_ORBIT libIDL-2.0 >= $REQUIRED_VERSION_LIBIDL)
AC_SUBST(ORBITCPP_IDL_LIBS)
AC_SUBST(ORBITCPP_IDL_CFLAGS)

#Get the path of the backend installation directory:
ORBIT_BACKEND_DIR=`pkg-config --variable=backendsdir ORBit-idl-2.0`
AC_SUBST(ORBIT_BACKEND_DIR)
ORBIT_IDL_DIR=`pkg-config --variable=prefix ORBit-idl-2.0`/share/idl/orbit-2.0/
AC_SUBST(ORBIT_IDL_DIR)

AC_PROG_CXX

case "$CXX" in
cxx)
  CPPFLAGS="$CPPFLAGS -D__USE_STD_IOSTREAM"
  ;;
esac

AC_MSG_CHECKING(what warning flags to pass to the C compiler)
AC_MSG_RESULT($WARN_CFLAGS)
AC_SUBST(WARN_CFLAGS)

# AC_SUBST(pkglibdir)

DISABLE_DEPRECATED_CFLAGS=" \
	-DG_DISABLE_DEPRECATED"
AC_SUBST(DISABLE_DEPRECATED_CFLAGS)

AC_OUTPUT([
  Makefile
  ORBit-2.0-cpp.pc
  ORBit-CosNaming-2.0-cpp.pc
  orbitcpp/Makefile
    orbitcpp/idl-compiler/Makefile
      orbitcpp/idl-compiler/types/Makefile
    orbitcpp/orb-cpp/Makefile
      orbitcpp/orb-cpp/smartpointers/Makefile
    orbitcpp/services/Makefile
      orbitcpp/services/name/Makefile

  test/Makefile
])
