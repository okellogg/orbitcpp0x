idlname = helloworld

include $(top_srcdir)/test/cpp/Makefile.am_fragment

SUBDIRS=generated
check_PROGRAMS=client server

# Server is linked with stubs and skels:
server_SOURCES= server.cc helloworld-impl.cc helloworld.idl helloworld-impl.h server_thread.cc
server_LDADD= generated/libhelloworldSkels.a generated/libhelloworldStubs.a $(testcpp_libs)

# Client is linked with stubs:
client_SOURCES= client.cc helloworld.idl
client_LDADD= generated/libhelloworldStubs.a $(testcpp_libs)

