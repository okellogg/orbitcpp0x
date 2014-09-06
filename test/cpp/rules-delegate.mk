# rules-delegate.mk
# Standard rules for directories containing only subdirectories

# handle "(g)make -k" for FOR_ALL_SUBDIRS
# if -k is not set, a failed sub-make will abort the loop
ifeq ($(findstring k,$(word 1,$(MFLAGS))),k)
  MAKE_K := true
else
  MAKE_K := exit 1
endif

# delegation: call (g)make in every subdirectory in SUBDIRS
FOR_ALL_LOOP = for i in ${1}; do $(MAKE) -C $$i $(MAKEFILE) $@ || $(MAKE_K); done
FOR_ALL = $(if $(strip $(1)),$(call FOR_ALL_LOOP,$(1)),true)
FOR_ALL_SUBDIRS = $(call FOR_ALL,$(SUBDIRS))

all:
	$(FOR_ALL_SUBDIRS)

clean:
	$(FOR_ALL_SUBDIRS)

distclean:
	$(FOR_ALL_SUBDIRS)

# Insert more rules as needed. Action is always $(FOR_ALL_SUBDIRS)
