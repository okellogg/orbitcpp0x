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
 *  Purpose:	stub generation pass
 *
 */




#ifndef ORBITCPP_PASS_STUBS
#define ORBITCPP_PASS_STUBS




#include <libIDL/IDL.h>
#include "pass.h"



class IDLPassStubs : public IDLOutputPass {
public:
	IDLPassStubs(IDLCompilerState &state,ostream &header,ostream &module)
		: IDLOutputPass(state,header,module) {
		state.m_pass_stubs = this;
	}

	void runPass();
 
protected:
};




#endif
