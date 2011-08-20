#
# ORBit-C++: C++ bindings for ORBit.
#
# Copyright (C) 2000 Andreas Kloeckner
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Author:	Andreas Kloeckner <ak@ixion.net>

INCLUDES = -I$(top_builddir) -I$(top_srcdir)  \
	-DVERSION=\"$(ORBIT_VERSION)\"        \
	$(WARN_CFLAGS)                        \
	$(ORBITCPP_IDL_CFLAGS)

SUBDIRS = types

cppbackendlibdir = $(ORBIT_BACKEND_DIR)
cppbackendlib_LTLIBRARIES = libORBit-idl-backend-cpp.la

headers = \
	base.h \
  debug.h \
  IDLMethod.h \
	language.h \
	types.h \
	pass.h \
	error.h \
	pass.h \
	pass_idl_it.h \
	pass_gather.h \
	pass_xlate.h \
	pass_stubs.h \
	pass_skels.h

modules = \
	base.cc \
  IDLMethod.cc \
	language.cc \
	types.cc \
	pass.cc \
	pass_idl_it.cc \
	pass_gather.cc \
	pass_xlate.cc \
	pass_stubs.cc \
	pass_skels.cc \
	main.cc 

libORBit_idl_backend_cpp_la_SOURCES = $(headers) $(modules)
libORBit_idl_backend_cpp_la_LDFLAGS = -module -avoid-version -no-undefined
libORBit_idl_backend_cpp_la_LIBADD = types/libidl-cpp-types.la $(ORBITCPP_IDL_LIBS)
