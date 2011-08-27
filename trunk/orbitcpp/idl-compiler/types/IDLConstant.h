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

#ifndef ORBITCPP_IDLCONSTANT
#define ORBITCPP_IDLCONSTANT

#include <libIDL/IDL.h>
#include "language.h"
#include "IDLElement.h"

class IDLType;
class IDLScope;

class IDLConstant :
	public IDLElement,
	public virtual IDLNotAType,
	public IDLIdentified {
protected:
	IDLType	*m_type;
public:
	IDLConstant(IDLType *type, std::string const &id, IDL_tree node, IDLScope *parentscope = NULL)
	:	IDLElement(id,node,parentscope),
		IDLIdentified (IDL_CONST_DCL(node).ident),
		m_type(type)
	{
	}

	string getValue() const {
		return idlTranslateConstant(IDL_CONST_DCL(getNode()).const_exp);
	}
	IDLType *getType() {
		return m_type;
	}
};

#endif // ORBITCPP_IDLCONSTANT

