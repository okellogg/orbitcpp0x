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

#include "IDLType.h"
#include "IDLUnion.h"
#include "IDLCaseStmt.h"
#include "IDLMember.h"
#include "error.h"


void
IDLType::const_decl_write (ostream          &ostr, ostream&, IDLScope const&,
			   Indent           &indent,
			   const string     &cpp_id,
			   const string     &value,
			   const IDLTypedef *active_typedef) const
{
	throw IDLExNoConstantOfThisType (""); // FIXME
}

void IDLStandardUnionable::create_union_accessors(IDLUnion const& un, IDLCaseStmt const& case_stmt, std::ostream& header, Indent indent, std::ostream& module, Indent mod_indent) const
{
	const IDLMember   &member = case_stmt.get_member ();

	const string member_cpp_type = member.getType ()->get_cpp_member_typename ();
	const string member_name = member.get_cpp_identifier ();

	string full_member_name =
		(un.is_fixed () ?
			"m_target._u." :
			"m_target->_u.") +
		member_name;

	
	// Getter accessor
	header << indent << member_cpp_type << " "
		 << member_name << " () const;" << endl;

	module << member_cpp_type << " "
		 << un.get_cpp_method_prefix () << "::" << member_name
		 << " () const" << endl
		 << "{" << endl;

	module << ++mod_indent << member_cpp_type << " _ret;" << endl;

	member.getType ()->member_unpack_from_c (
		module, mod_indent, "_ret", full_member_name);
		
	module << mod_indent << "return _ret;" << endl;
	module << --mod_indent << "}" << endl << endl;
	

	// Setter accessor
	create_union_setter(un, case_stmt, header, indent, module, mod_indent);
}

void IDLStandardUnionable::create_union_setter(IDLUnion const& un, IDLCaseStmt const& case_stmt, std::ostream& header, Indent indent, std::ostream& module, Indent mod_indent)
{
	const IDLMember   &member = case_stmt.get_member ();

	const string member_cpp_type = member.getType ()->get_cpp_member_typename ();
	const string member_name = member.get_cpp_identifier ();

	string full_member_name =
		(un.is_fixed () ?
			"m_target._u." :
			"m_target->_u.") +
		member_name;

	string discr_val;
	if (case_stmt.isDefault ())
		discr_val = un.get_discriminator_default_value ();
	else
		discr_val = *(case_stmt.labelsBegin());
	
	header << indent << "void " << member_name
		 << " (" << member_cpp_type << " val);" << endl << endl;
	
	module << "void "
		 << un.get_cpp_method_prefix () << "::" << member_name
		 << " (" << member_cpp_type << " val)" << endl
		 << "{" << endl;

	module << ++mod_indent << "_clear_member ();" << endl;
	module << mod_indent << "_d (" << discr_val << ");" << endl;

	member.getType ()->member_pack_to_c (
		module, mod_indent, "val", full_member_name);

	module << --mod_indent << "}" << endl << endl;
}
	
void IDLReferentUnionable::create_union_accessors(IDLUnion const& un, IDLCaseStmt const& case_stmt, std::ostream& header, Indent indent, std::ostream& module, Indent mod_indent) const
{
	const IDLMember   &member = case_stmt.get_member ();

	const IDLType& memb_type = *member.getType();
	const string member_cpp_type = memb_type.get_cpp_member_typename ();
	const string member_name = member.get_cpp_identifier ();

	string full_member_name =
		(un.is_fixed () ?
			"m_target._u." :
			"m_target->_u.") +
		member_name;

	
	// Setter accessor
	string discr_val;
	if (case_stmt.isDefault ())
		discr_val = un.get_discriminator_default_value ();
	else
		discr_val = *(case_stmt.labelsBegin());
	
	header << indent << "void " << member_name
		 << " (" << member_cpp_type << " const& val);" << endl << endl;
	
	module << "void "
		 << un.get_cpp_method_prefix () << "::" << member_name
		 << " (" << member_cpp_type << " const& val)" << endl
		 << "{" << endl;

	module << ++mod_indent << "_clear_member ();" << endl;
	module << mod_indent << "_d (" << discr_val << ");" << endl;

	memb_type.member_pack_to_c (
		module, mod_indent, "val", full_member_name);

	module << --mod_indent << "}" << endl << endl;

	// Getter accessor
	// FIXME: Need to return by reference both const and non-const
	// versions. 

	header << indent << member_cpp_type << " "
		 << member_name << " () const;" << endl;

	module << member_cpp_type << " "
		 << un.get_cpp_method_prefix () << "::" << member_name
		 << " () const" << endl
		 << "{" << endl;

	module << ++mod_indent << member_cpp_type << " _ret;" << endl;

	memb_type.member_unpack_from_c (
		module, mod_indent, "_ret", full_member_name);
		
	module << mod_indent << "return _ret;" << endl;
	module << --mod_indent << "}" << endl << endl;
	

}
