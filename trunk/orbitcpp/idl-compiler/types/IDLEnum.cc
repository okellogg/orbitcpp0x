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

#include "IDLEnum.h"
#include "IDLTypedef.h"
#include "error.h"

IDLEnum::IDLEnum(IDL_tree en)
:	IDLType (IDLType::T_ENUM),
	IDLUserDefSimpleType(IDL_IDENT(IDL_TYPE_ENUM(en).ident).str, en, 0),
	IDLIdentified (IDL_TYPE_ENUM(en).ident)
{
	IDL_tree l = IDL_TYPE_ENUM(en).enumerator_list;
	
	for (; l != 0; l = IDL_LIST(l).next) {
		IDL_tree c = IDL_LIST(l).data;

		IDLEnumComponent *enc =
			new IDLEnumComponent(IDL_IDENT(c).str,c,0);
			
		ORBITCPP_MEMCHECK(enc)
		m_elements.push_back(enc);
	}
}

IDLEnum::const_iterator IDLEnum::begin() const
{
	return m_elements.begin();
}

IDLEnum::const_iterator IDLEnum::end() const
{
	return m_elements.end();
}

std::string IDLEnum::get_default_value (std::set<std::string> const &labels) const
{
	string result = "";

	for (const_iterator i = begin (); i != end (); i++)
	{
		string test = (*i)->get_cpp_typename ();
		if (labels.find (test) == labels.end ())
		{
			result = test;
			break;
		}
	}

	return result;
}

string
IDLEnum::stub_impl_arg_call (const string     &cpp_id,
					  IDL_param_attr    direction,
					  const IDLTypedef *active_typedef) const
{
	string retval;
	string expr = cpp_id;
	
	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = "(" + get_c_typename() + ")" + expr + "";
		break;
	case IDL_PARAM_INOUT:
	case IDL_PARAM_OUT:
		retval = "(" + get_c_typename() + "*)&" + expr + "";
		break;
	}
	
	return retval;
}

void
IDLEnum::member_unpack_from_c (ostream      &ostr,
				     Indent       &indent,
				     const string &cpp_id,
				     const string &c_id,
				     const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_fixed_cpp_typename ();

	ostr << indent << cpp_id << " = "
	     << "(" << cpp_type << ")" << c_id << ';' << endl;
}

string
IDLEnum::get_seq_typename (unsigned int      length,
			       const IDLTypedef *active_typedef) const
{
	string retval;

	char *tmp = 0;
	std::string seq_typename = get_cpp_typename();
	std::string c_member_typename_base =
		active_typedef != 0 ?
			active_typedef->get_c_typename() :
			get_c_typename();
	
	if (length)
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleBoundedSeq< " IDL_IMPL_NS "::seq_traits_assignable< %s, %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct>, %d >",
				       seq_typename.c_str (),
				       c_member_typename_base.c_str (),
				       c_member_typename_base.c_str (),
				       c_member_typename_base.c_str (),
				       length);
	else
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleUnboundedSeq< " IDL_IMPL_NS "::seq_traits_assignable< %s, %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct> >",
				       seq_typename.c_str (),
				       c_member_typename_base.c_str (),
				       c_member_typename_base.c_str (),
				       c_member_typename_base.c_str ());
	
	retval = tmp;
	g_free (tmp);
	
	return retval;
}
