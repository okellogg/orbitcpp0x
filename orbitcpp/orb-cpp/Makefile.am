lib_LTLIBRARIES = libORBit-2-cpp.la

INCLUDES = \
	-I$(top_srcdir) \
	$(WARN_CFLAGS) \
	$(ORBITCPP_CFLAGS)

SUBDIRS = smartpointers

libORBit_2_cppincludedir = $(includedir)/orbitcpp-2.0/orbitcpp/orb-cpp

libORBit_2_cppinclude_HEADERS = 			\
			orbitcpp.h 			\
			orbitcpp_config.h 		\
			orbitcpp_constants.h 		\
			orbitcpp_exception.h 		\
			orbitcpp_exceptionlist.h 	\
			orbitcpp_poaexceptionlist.h 	\
			orbitcpp_object.h 		\
			orbitcpp_orb.h 			\
			orbitcpp_poa.h 			\
			orbitcpp_policy.h 			\
			orbitcpp_sequence.h 		\
			orbitcpp_compound_seq.h 	\
			orbitcpp_simple_seq.h 		\
			orbitcpp_string_seq.h		\
			orbitcpp_smartptr.h 		\
			orbitcpp_tools.h 		\
			orbitcpp_traits.h 		\
			orbitcpp_types.h 		\
			orbitcpp_any.h 			\
			orbitcpp_typecode.h		\
			poa-cpp-common.h		\
			poa-cpp-skels.h

libORBit_2_cpp_la_SOURCES = \
			$(libORBit_2_cppinclude_HEADERS)	\
			orbitcpp_types.cc 			\
			orbitcpp_exception.cc 			\
			orbitcpp_tools.cc 			\
			orbitcpp_object.cc 			\
			orbitcpp_orb.cc 			\
			orbitcpp_poa.cc 			\
			orbitcpp_policy.cc 			\
			orbitcpp_constants.cc 			\
			orbitcpp_any.cc 			\
			orbitcpp_typecode.cc			\
			poa-cpp-common.cc			\
			poa-cpp-skels.cc
			$(libORBit_2_cppinclude_HEADERS)

libORBit_2_cpp_la_LDFLAGS = -version-info $(LT_VERSION) -no-undefined
libORBit_2_cpp_la_LIBADD = $(ORBITCPP_LIBS) #-lstdc++
