/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2000 Andreas Kloeckner
 *  Copyright (C) 2011 Oliver Kellogg
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
 *  Purpose: IDL compiler language representation
 *
 */

#ifndef ORBITCPP_IDLATTRIBUTE
#define ORBITCPP_IDLATTRIBUTE

#include <libIDL/IDL.h>
#include "types.h"
#include "language.h"
#include "IDLInhibited.h"

class IDLType;

class IDLAttribute :
	public IDLInhibited<EmptyType>,
	public virtual IDLNotAType {

	IDL_tree attr_dcl_;
	IDL_tree simple_dcl_;

	IDLType *m_type;
  
public:

	IDLAttribute(IDL_tree attr_dcl, IDL_tree simple_dcl)
	:	IDLInhibited<EmptyType>(simple_dcl_ident(simple_dcl)),
		attr_dcl_ (attr_dcl),
		simple_dcl_ (simple_dcl)
	{
		std::string dummy;
		m_type = IDLTypeParser::parseDcl(simple_dcl, IDL_ATTR_DCL(attr_dcl_).param_type_spec, dummy);
	}

	IDLType *getType() {return m_type;}
	bool isReadOnly() {return IDL_ATTR_DCL(attr_dcl_).f_readonly;}
	
};

#endif // ORBITCPP_IDLATTRIBUTE

