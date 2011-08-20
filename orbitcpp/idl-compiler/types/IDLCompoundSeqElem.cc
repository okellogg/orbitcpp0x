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
 *  Purpose:	Common base class for types that need to be in Compound{Un,}BoundedSeq
 *
 *
 */

#include "IDLCompoundSeqElem.h"
#include "IDLTypedef.h"

string
IDLCompoundSeqElem::get_seq_typename (unsigned int      length,
			       const IDLTypedef *active_typedef) const
{
	string retval;

	char *tmp = 0;
	std::string seq_typename = get_cpp_typename();
	std::string c_member_typename_base =
		active_typedef != 0 ?
			active_typedef->get_c_member_typename() :
			get_c_member_typename();

	std::string cpp_member_typename =
		active_typedef != 0 ?
			active_typedef->get_cpp_member_typename() :
			get_cpp_member_typename();

	std::string traits = 
		(conversion_required()) ?
			"seq_traits" :
			"seq_traits_assignable";

	std::string cxx_value =
		(conversion_required()) ?
			cpp_member_typename + ", " :
			"";
	
#ifdef IDL2CPP0X
	if (c_member_typename_base == "CORBA_string")
		tmp = g_strdup (IDL_IMPL_NS "::StringUnboundedSeq");
	else
#endif
	if (length)
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleBoundedSeq< " IDL_IMPL_NS "::%s< %s, %s %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct>, %d >",
				traits.c_str(),
				seq_typename.c_str (),
				cxx_value.c_str(),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str (),
				length);
	else
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleUnboundedSeq< " IDL_IMPL_NS "::%s< %s, %s %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct> >",
				traits.c_str(),
				seq_typename.c_str (),
				cxx_value.c_str(),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str ());
	
	retval = tmp;
	g_free (tmp);
	
	return retval;
}
