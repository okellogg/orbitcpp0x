/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2000 Andreas Kloeckner
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Author:	Andreas Kloeckner <ak@ixion.net>
 *
 *  Purpose: stub generation pass
 *
 */




#include "orbitcpp/idl-compiler/error.h"
#include "orbitcpp/idl-compiler/pass_stubs.h"

#include "orbitcpp/idl-compiler/types/IDLAttribAccessor.h"


// IDLPassStubs --------------------------------------------------------------
void 
IDLPassStubs::runPass() {
	m_header
	<< indent << "#ifndef ORBITCPP_IDL_" << idlUpper(m_state.m_basename) << "_STUBS" << endl
	<< indent << "#define ORBITCPP_IDL_" << idlUpper(m_state.m_basename) << "_STUBS" << endl
	<< indent << endl << endl
	<< indent << "#include <string.h>" << endl
	<< indent << "#include \"" << m_state.m_basename << IDL_CPP_HEADER_EXT"\"" << endl
	<< indent << endl << endl
	<< indent << "// Stub declaration ------------------------------------" << endl
	<< indent << endl;

	m_module
	<< indent << "#include \"" << m_state.m_basename << IDL_CPP_STUB_HEADER_EXT"\"" << endl
	<< indent << endl << endl
	<< indent << "// Stub code -------------------------------------------" << endl << endl;

	
	runJobs();

	m_header << endl << indent << "#endif" << endl;
}


