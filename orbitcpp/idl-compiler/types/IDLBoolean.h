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
 *  Purpose:	IDL compiler type representation
 *
 *
 */


#ifndef ORBITCPP_TYPES_IDLBOOLEAN
#define ORBITCPP_TYPES_IDLBOOLEAN

#include "IDLSimpleType.h"
#include "IDLUnionDiscriminator.h"

class IDLBoolean :
	public IDLSimpleType,
	public IDLUnionDiscriminator,
	public IDLTypenameUnused
{
protected:
#if defined (IDL2CPP0X)
	std::string get_cpp_identifier () const { return "bool"; }
#else
	std::string get_cpp_identifier () const { return "Boolean"; }
#endif
	std::string get_cpp_typename () const;
	std::string get_c_typename () const;

 public:
	IDLBoolean () : IDLType (IDLType::T_BOOLEAN) {}
	virtual ~ IDLBoolean () {}

	std::string get_default_value (StringSet const &labels) const;

	std::string discr_get_c_typename () const {
		return get_fixed_c_typename ();
	}									
	
	std::string discr_get_cpp_typename () const {
		return get_fixed_cpp_typename ();
	}									
};

#endif //ORBITCPP_TYPES_IDLBOOLEAN

