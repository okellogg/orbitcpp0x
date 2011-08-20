INCLUDES = -I$(top_builddir)                     \
	-I$(top_srcdir)                      \
	-I$(top_srcdir)/orbitcpp/idl-compiler \
	-DVERSION=\"$(ORBIT_VERSION)\"        \
	$(WARN_CFLAGS)                        \
	$(ORBITCPP_IDL_CFLAGS)


noinst_LTLIBRARIES = libidl-cpp-types.la

headers = 		\
  IDLAny.h		\
  IDLArray.h		\
  IDLArrayList.h	\
  IDLAttribAccessor.h	\
  IDLBoolean.h		\
  IDLCompound.h	\
  IDLCompoundSeqElem.h	\
  IDLEnum.h		\
  IDLElement.h		\
  IDLException.h	\
  IDLInhibited.h	\
  IDLInterface.h	\
  IDLModule.h		\
  IDLObject.h		\
  IDLOperation.h	\
  IDLScope.h		\
  IDLSequence.h	\
  IDLSequenceList.h    \
  IDLSimpleType.h	\
  IDLString.h		\
  IDLStruct.h		\
  IDLType.h		\
  IDLTypeCode.h	\
  IDLTypedef.h		\
  IDLUnion.h		\
  IDLUserDefScopeType.h \
  IDLUserDefSimpleType.h \
  IDLUserDefType.h	\
  IDLVoid.h

modules =		\
  IDLAny.cc		\
  IDLArray.cc		\
  IDLArrayList.cc	\
  IDLAttribAccessor.cc	\
  IDLBoolean.cc		\
  IDLCompound.cc	\
  IDLCompoundSeqElem.cc	\
  IDLEnum.cc		\
  IDLElement.cc		\
  IDLException.cc	\
  IDLInhibited.cc	\
  IDLInterface.cc	\
  IDLObject.cc		\
  IDLOperation.cc	\
  IDLScope.cc		\
  IDLSequence.cc	\
  IDLSequenceList.cc    \
  IDLSimpleType.cc	\
  IDLString.cc		\
  IDLStruct.cc		\
  IDLType.cc		\
  IDLTypeCode.cc	\
  IDLTypedef.cc		\
  IDLUnion.cc		\
  IDLUserDefScopeType.cc \
  IDLUserDefSimpleType.cc \
  IDLUserDefType.cc	\
  IDLVoid.cc

libidl_cpp_types_la_SOURCES = \
	$(headers) $(modules)

libidl_cpp_types_la_LIBADD =

