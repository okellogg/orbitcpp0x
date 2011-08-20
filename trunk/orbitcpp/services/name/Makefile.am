lib_includedir = $(includedir)/orbitcpp-2.0/services/name

idlname=CosNaming

include $(top_srcdir)/test/cpp/Makefile_generated.am_fragment

idl_file=$(ORBIT_IDL_DIR)/CosNaming.idl

lib_LTLIBRARIES=libORBitCosNaming-2-cpp.la

libORBitCosNaming_2_cpp_la_SOURCES = \
	CosNaming-common.c \
	CosNaming-stubs.c \
	CosNaming-cpp-common.cc \
	CosNaming-cpp-stubs.cc

lib_include_HEADERS = \
	CosNaming.h \
	CosNaming-cpp-common.h \
	CosNaming-cpp-stubs.h


libORBitCosNaming_2_cpp_la_LDFLAGS = -version-info $(LT_VERSION) -no-undefined
libORBitCosNaming_2_cpp_la_LIBADD = $(top_builddir)/orbitcpp/orb-cpp/libORBit-2-cpp.la $(ORBITCPP_LIBS)


