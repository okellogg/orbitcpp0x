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


#ifndef ORBITCPP_TYPES_IDLENUM
#define ORBITCPP_TYPES_IDLENUM

#include "IDLUserDefSimpleType.h"
#include "orbitcpp/idl-compiler/language.h"

class IDLEnum :
	public IDLUserDefSimpleType,
	public IDLUnionDiscriminator,
	public IDLIdentified
{
public:
	typedef std::vector<IDLEnumComponent *> 	ElementsVec;
	typedef ElementsVec::const_iterator 		const_iterator;

private:
	ElementsVec m_elements;
	
public:
	IDLEnum(IDL_tree node);

	const_iterator begin() const;
	const_iterator end() const;

	string get_seq_typename (unsigned int length, const IDLTypedef *active_typedef) const;

	std::string get_default_value (std::set<std::string> const &labels) const;

	std::string discr_get_c_typename () const {
		return get_fixed_c_typename ();
	}
	
	std::string discr_get_cpp_typename () const {
		return get_fixed_cpp_typename ();
	}

	string
	stub_impl_arg_call (const string     &cpp_id,
						  IDL_param_attr    direction,
					  const IDLTypedef *active_typedef) const;
	void
	member_unpack_from_c (ostream      &ostr,
				     Indent       &indent,
				     const string &cpp_id,
				     const string &c_id,
				     const IDLTypedef *active_typedef) const;
};

#endif //ORBITCPP_TYPES_IDLENUM

