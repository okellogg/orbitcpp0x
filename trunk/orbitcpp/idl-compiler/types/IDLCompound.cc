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

#include "IDLCompound.h"
#include "IDLType.h"
#include "IDLMember.h"

void
IDLCompound::write_packing_decl (ostream &ostr,
				 Indent  &indent) const
{
	string c_id = IDL_IMPL_C_NS_NOTUSED + get_c_typename ();

	ostr << indent << c_id << " *" << "_orbitcpp_pack () const;" << endl;
	ostr << indent << "void _orbitcpp_pack (" << c_id << " &) const";
	if (m_items->empty())
		ostr << " { }\n";
	else
		ostr << ";\n";

	ostr << indent << "void _orbitcpp_unpack (const " << c_id << " &)";
	if (m_items->empty())
		ostr << " { }\n";
	else
		ostr << ";\n";
}

void
IDLCompound::write_packing_impl (ostream &ostr,
				 Indent  &indent) const
{
	string c_id = IDL_IMPL_C_NS_NOTUSED + get_c_typename ();

	// Implementation of _orbitcpp_pack that returns a newly
	// allocated C structure on heap
	string c_alloc = c_id + "__alloc ()";
	
	ostr << c_id << " * "
	     << get_cpp_method_prefix () << "::_orbitcpp_pack () const" << endl
	     << '{' << endl;
	ostr << ++indent << c_id << " *_c_struct = " << c_alloc << ';' << endl << endl;
	ostr << indent << "if (!_c_struct)" << endl;
	ostr << ++indent << "throw CORBA::NO_MEMORY ();" << endl << endl;
	ostr << --indent << "_orbitcpp_pack (*_c_struct);" << endl;
	ostr << indent << "return _c_struct;" << endl;
	ostr << --indent << '}' << endl << endl;

	if (m_items->empty())
		return;


	
	// Implementation of _orbitcpp_pack that fills a
	// caller-supplied C structure
	ostr << "void " << get_cpp_method_prefix () << "::_orbitcpp_pack "
	     << "(" << c_id << " &_c_struct) const" << endl
	     << '{' << endl;
	++indent;

	for (const_iterator i = begin (); i != end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;
		string cpp_id = member.get_cpp_identifier ();
#ifdef IDL2CPP0X
		cpp_id.insert (0, "_");
#endif
		string c_id = "_c_struct." + member.get_c_identifier ();
		
		member.getType ()->member_pack_to_c (ostr, indent, cpp_id, c_id);
	}
	ostr << endl;
	
	ostr << --indent << '}' << endl << endl;


	// Implementation of _orbitcpp_unpack
	ostr << "void " << get_cpp_method_prefix () << "::_orbitcpp_unpack "
	     << "(const " << c_id << " &_c_struct)" << endl
	     << '{' << endl;
	++indent;

	for (const_iterator i = begin (); i != end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;
		string cpp_id = member.get_cpp_identifier ();
#ifdef IDL2CPP0X
		cpp_id.insert (0, "_");
#endif
		string c_id = "_c_struct." + member.get_c_identifier ();

		member.getType ()->member_unpack_from_c (ostr, indent, cpp_id, c_id);
	}

	ostr << --indent << '}' << endl << endl;
}

void
IDLCompound::write_member_decls (ostream &ostr,
				 Indent  &indent) const
{
	if (size () == 0)
		return;
	ostr << indent << "// members" << endl;

#ifdef IDL2CPP0X
	for (const_iterator i = begin (); i != end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;
		const IDLType *t = member.getType ();
		const string typ = t->get_cpp_member_typename ();
		const string id = member.get_cpp_identifier ();
		ostr << indent << "void " << id << " (";
		if (!t->is_scalar ())
			ostr << "const ";
		ostr << typ;
		if (!t->is_scalar ())
			ostr << "&";
		ostr << " value) { _" << id << " = value; }" << endl;
		if (t->is_scalar ())
		{
			ostr << indent << typ << " " << id
				 << "() const { return _" << id << "; }" << endl;
		}
		else
		{
			ostr << indent << "const " << typ << "& " << id
				 << "() const { return _" << id << "; }" << endl;
			ostr << indent << typ << "& " << id << "() { return _"
				 << id << "; }" << endl;
		}
	}
	ostr << endl;
	ostr << --indent << "private:" << endl;
	++indent;
#endif
	for (const_iterator i = begin (); i != end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;
		ostr << indent << member.getType ()->get_cpp_member_typename ()
			 << " ";
#ifdef IDL2CPP0X
		ostr << "_";
#endif
		ostr << member.get_cpp_identifier () << ";" << endl;
	}
	ostr << endl;

#ifdef IDL2CPP0X
	ostr << --indent << "public:" << endl;
	++indent;
#endif
}

