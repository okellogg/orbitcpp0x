# rules-generated.mk
# Rules for a test's subdirectory named "generated"

include $(top_builddir)/test/defs.mk

all: $(DEPDIR) $(check_LIBRARIES)

include $(top_builddir)/test/rules-common.mk

idl_file = ../$(idlname).idl

$(idl_built): $(idl_file)
	$(ORBIT_IDL) -l c $(idl_file)
	$(ORBIT_IDL) --backenddir=$(top_builddir)/orbitcpp/idl-compiler/.libs -l cpp $(idl_file)

$(DEPDIR):
	mkdir -p $@

$(libSkels_a): $(libSkels_a_OBJECTS) $(orbitcpp_lib) 
	$(RM) $(libSkels_a)
	$(CALL_AR) $(libSkels_a) $(libSkels_a_OBJECTS) $(libSkels_a_LIBADD)
	$(RANLIB) $(libSkels_a)
$(libStubs_a): $(libStubs_a_OBJECTS) $(orbitcpp_lib) 
	$(RM) $(libStubs_a)
	$(CALL_AR) $(libStubs_a) $(libStubs_a_OBJECTS) $(libStubs_a_LIBADD)
	$(RANLIB) $(libStubs_a)

clean-checkLIBRARIES:
	$(RM) $(check_LIBRARIES)

clean: clean-libtool clean-compile clean-checkLIBRARIES

distclean:
	$(MAKE) clean
	$(RM) $(idl_built)
	$(RM) ./$(DEPDIR)/*

