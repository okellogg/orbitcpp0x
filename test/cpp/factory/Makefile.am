idlname = factory

include $(top_srcdir)/test/cpp/Makefile.am_fragment

SUBDIRS=generated
check_PROGRAMS=client server

# Server is linked with stubs and skels:
server_SOURCES= server.cc factory-impl.cc factory.idl factory-impl.h
server_LDADD= generated/libfactorySkels.a generated/libfactoryStubs.a $(testcpp_libs)

# Client is linked with stubs:
client_SOURCES= client.cc factory.idl
client_LDADD= generated/libfactoryStubs.a $(testcpp_libs)

