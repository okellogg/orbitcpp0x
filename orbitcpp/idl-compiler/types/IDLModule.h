/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2003 Bowie Owens
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
 *  Author:	Bowie Owens <bowie.owens@csiro.au>
 *
 *  Purpose: IDL compiler language representation
 *
 */

#ifndef ORBITCPP_TYPES_IDLMODULE
#define ORBITCPP_TYPES_IDLMODULE

#include "IDLScope.h"
#include "IDLInhibited.h"

class IDLModule :
	public IDLScope,
	public virtual IDLNotAType,
	public IDLScopeNamespaceImplementation,
	public IDLIdentified {
public:
	IDLModule(string const &id,IDL_tree node,IDLScope *parentscope = NULL, ItemList* items = NULL, ReplaceType update = no_replace)
	:	IDLScope(id,node,parentscope,items,update),
		IDLIdentified (IDL_MODULE(node).ident)
	{
	}

	IDLModule(IDL_tree node)
	:	IDLScope(IDL_IDENT(IDL_MODULE(node).ident).str, node, 0, 0, no_replace),
		IDLIdentified (IDL_MODULE(node).ident)
	{
	}
};

#endif

