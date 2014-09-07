# rules.mk
# Rules for a test's main directory

include $(top_builddir)/test/cpp/defs.mk

all:
	$(MAKE) -C generated
	$(MAKE) $(check_PROGRAMS)

include $(top_builddir)/test/cpp/rules-common.mk

check_PROGRAMS = client$(EXEEXT) server$(EXEEXT)
client_OBJECTS = client.$(OBJEXT)

client_DEPENDENCIES = generated/$(libStubs_a) $(orbitcpp_lib)
server_OBJECTS = server.$(OBJEXT) $(server_EXTRA_OBJECTS) $(idlname)-impl.$(OBJEXT)
server_DEPENDENCIES = generated/$(libSkels_a) $(client_DEPENDENCIES)
#        client_SOURCES = client.cc
#        server_SOURCES = server.cc $(idlname)-impl.cc $(idlname)-impl.h
#        SOURCES = $(client_SOURCES) $(server_SOURCES)

testcpp_libs = $(orbitcpp_lib) $(ORBITCPP_LIBS)

# Server is linked with stubs and skels:
server_LDADD = generated/$(libSkels_a) generated/$(libStubs_a) $(testcpp_libs)

# Client is linked with stubs:
client_LDADD = generated/$(libStubs_a) $(testcpp_libs)

client$(EXEEXT): $(client_OBJECTS) $(client_DEPENDENCIES) 
	$(CXXLINK) $(client_OBJECTS) $(client_LDADD) $(LIBS)
server$(EXEEXT): $(server_OBJECTS) $(server_DEPENDENCIES) 
	$(CXXLINK) $(server_OBJECTS) $(server_LDADD) $(LIBS)

clean-checkPROGRAMS:
	$(RM) $(check_PROGRAMS)

clean: clean-libtool clean-compile clean-checkPROGRAMS
	$(MAKE) -C generated $@

distclean: clean
	$(MAKE) -C generated $@
	$(RM) ./$(DEPDIR)/*

