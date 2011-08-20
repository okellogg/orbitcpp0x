idlname = everything

include $(top_srcdir)/test/cpp/Makefile_generated.am_fragment

check_LIBRARIES = libeverythingStubs.a libeverythingSkels.a

libeverythingStubs_a_SOURCES= $(stub_sources)
libeverythingStubs_a_LIBADD= $(testcpp_generated_libs)

libeverythingSkels_a_SOURCES= $(skel_sources)
libeverythingSkels_a_LIBADD= $(testcpp_generated_libs)

