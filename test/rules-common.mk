# rules-common.mk
# Rules which are common to a test directory and its subdirectory "generated"

.c.o:
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(MV) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

.c.obj:
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `echo '$<'`
	$(MV) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

.c.lo:
	$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(MV) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo

.cc.o:
	$(CXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(MV) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

.cc.obj:
	$(CXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `echo '$<'`
	$(MV) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

.cc.lo:
	$(LTCXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(MV) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo

mostlyclean-libtool:
	$(RM) *.lo

clean-libtool: mostlyclean-libtool
	$(RM) -r .libs _libs

clean-compile:
	$(RM) *.$(OBJEXT)

