# defs.mk  - definition of symbols which are common to all tests
# The symbol top_builddir must be set before including this file.

AR ?= ar
ARFLAGS ?= cru
AWK ?= gawk
CC ?= gcc
CFLAGS ?= -g -O2
CPP ?= gcc -E
CXX ?= g++
CXXCPP ?= g++ -E
CXXFLAGS ?= -g -O2
DEFS ?= -DHAVE_CONFIG_H
DEPDIR ?= .deps
DISABLE_DEPRECATED_CFLAGS ?= -DG_DISABLE_DEPRECATED
ECHO_N ?= -n
EGREP ?= /usr/bin/grep -E
FGREP ?= /usr/bin/grep -F
GREP ?= /usr/bin/grep
INSTALL ?= /usr/bin/install -c
INSTALL_DATA ?= ${INSTALL} -m 644
INSTALL_PROGRAM ?= ${INSTALL}
INSTALL_SCRIPT ?= ${INSTALL}
INSTALL_STRIP_PROGRAM ?= $(install_sh) -c -s
# LD ?= /opt/gnat/gpl/libexec/gcc/i686-pc-linux-gnu/4.5.4/ld
LIBTOOL ?= $(SHELL) $(top_builddir)/libtool
LN_S ?= ln -s
LT_VERSION ?= 10:0:0
MKDIR_P ?= /bin/mkdir -p
MV ?= /bin/mv
NM ?= /usr/bin/nm -B
OBJDUMP ?= objdump
OBJEXT ?= o
ORBITCPP_CFLAGS ?= -DORBIT2=1 -pthread -I/usr/include/orbit-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include  
ORBITCPP_IDL_CFLAGS ?= -DORBIT2=1 -pthread -I/usr/include/orbit-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/libIDL-2.0  
ORBITCPP_IDL_LIBS ?= -pthread -lORBit-2 -lgobject-2.0 -lgthread-2.0 -lrt -lIDL-2 -lglib-2.0  
ORBITCPP_LIBS ?= -pthread -lORBit-2 -lgobject-2.0 -lgthread-2.0 -lrt -lglib-2.0  
ORBITCPP_MAJOR_VERSION = 1
ORBITCPP_MINOR_VERSION = 3
ORBITCPP_MICRO_VERSION = 10
ORBITCPP_VERSION ?= $(ORBITCPP_MAJOR_VERSION).$(ORBITCPP_MINOR_VERSION).$(ORBITCPP_MICRO_VERSION)
ORBIT_BACKEND_DIR ?= /usr/lib/orbit-2.0/idl-backends
ORBIT_IDL_DIR ?= /usr/share/idl/orbit-2.0/
PACKAGE = orbitcpp
PATH_SEPARATOR ?= :
PKG_CONFIG ?= /usr/bin/pkg-config
RANLIB ?= ranlib
REQUIRED_VERSION_LIBIDL ?= 0.7.4
REQUIRED_VERSION_ORBIT ?= 2.5.0
RM ?= /bin/rm -f
SED ?= /usr/bin/sed
SHELL ?= /bin/bash
STRIP ?= strip
VERSION ?= $(ORBITCPP_VERSION)
top_srcdir ?= ${top_builddir}
install_sh ?= ${SHELL} ${top_srcdir}/install-sh
top_build_prefix = ${top_builddir}/
ORBIT_IDL = orbit-idl-2
orbitcpp_lib = $(top_builddir)/orbitcpp/orb-cpp/libORBit-2-cpp.la

INCLUDES = -I$(top_srcdir) $(ORBITCPP_CFLAGS) -Igenerated -D_ORBITCPP_TESTCODE

DEFAULT_INCLUDES = -I. -I$(top_builddir)

CALL_AR = $(AR) $(ARFLAGS)
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(CPPFLAGS)  $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --tag=CC  $(LIBTOOLFLAGS) \
	--mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(CPPFLAGS)  $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --tag=CC  $(LIBTOOLFLAGS) \
	--mode=link $(CCLD)  $(CFLAGS) \
	$(LDFLAGS) -o $@
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(CPPFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) --tag=CXX  $(LIBTOOLFLAGS) \
	--mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(CPPFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) --tag=CXX  $(LIBTOOLFLAGS) \
	--mode=link $(CXXLD) $(CXXFLAGS) \
	$(LDFLAGS) -o $@

libSkels_a = lib$(idlname)Skels.a
libSkels_a_OBJECTS = $(idlname)-skels.$(OBJEXT) $(idlname)-cpp-skels.$(OBJEXT)

libStubs_a = lib$(idlname)Stubs.a
libStubs_a_OBJECTS = $(idlname)-common.$(OBJEXT) $(idlname)-cpp-common.$(OBJEXT) $(idlname)-stubs.$(OBJEXT) $(idlname)-cpp-stubs.$(OBJEXT)

c_common_h = $(idlname).h
c_common = $(c_common_h) $(idlname)-common.c
c_stub = $(idlname)-stubs.c
c_skel = $(idlname)-skels.c
c_all = $(c_common) $(c_stub) $(c_skel)
cpp_common_hh = $(idlname)-cpp-common.h
cpp_common = $(cpp_common_hh) $(idlname)-cpp-common.cc
cpp_stub = $(idlname)-cpp-stubs.h $(idlname)-cpp-stubs.cc
cpp_skel = $(idlname)-cpp-skels.h $(idlname)-cpp-skels.cc
cpp_lump = $(idlname)-cpp.h $(idlname)-cpp.cc
cpp_all = $(cpp_common) $(cpp_stub) $(cpp_skel) $(cpp_lump)

# Stubs are built from common .h/.cc, and stubs h/.cc
stub_sources = $(c_common) $(cpp_common) $(c_stub) $(cpp_stub)

# Skels are built from common headers, and stubs h/.cc, and linked to stubs.
skel_sources = $(c_common_h) $(cpp_common_hh) $(c_skel) $(cpp_skel)

# SOURCES = $(skel_sources) $(stub_sources)
idl_built = $(c_all) $(cpp_all)
# BUILT_SOURCES = $(idl_built)
check_LIBRARIES = $(libStubs_a) $(libSkels_a)

