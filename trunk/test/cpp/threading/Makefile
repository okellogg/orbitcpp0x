
top_builddir := ../../..

idlname = helloworld

server_EXTRA_OBJECTS := server_thread.o

include ../rules.mk

-include ./$(DEPDIR)/client.Po
-include ./$(DEPDIR)/$(idlname)-impl.Po
-include ./$(DEPDIR)/server.Po
-include ./$(DEPDIR)/server_thread.Po

