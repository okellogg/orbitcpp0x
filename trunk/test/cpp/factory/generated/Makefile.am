idlname = factory

include $(top_srcdir)/test/cpp/Makefile_generated.am_fragment

check_LIBRARIES = libfactoryStubs.a libfactorySkels.a

libfactoryStubs_a_SOURCES= $(stub_sources)
libfactoryStubs_a_LIBADD= $(testcpp_generated_libs)

libfactorySkels_a_SOURCES= $(skel_sources)
libfactorySkels_a_LIBADD= $(testcpp_generated_libs)

