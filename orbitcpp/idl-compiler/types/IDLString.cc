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

#include "IDLString.h"

#include "IDLTypedef.h"
#include "IDLUnion.h"

void
IDLString::const_decl_write (ostream          &header,
			     ostream& module,
			     IDLScope const& scope,
			     Indent           &indent,
			     const string     &cpp_id,
			     const string     &value,
			     const IDLTypedef *active_typedef) const
{
	header << indent;
	if (scope.scope_implementation() == IDLScopeImplementation::by_namespace) 
		header << "extern ";
		

	header << char_ << " const* const " << cpp_id << ";\n";
	module << char_ << " const* const " << scope.get_cpp_typename() << "::" << cpp_id << " = _orbitcpp::StringProperties<" << char_ << ">::convert(" << value << ");\n";
}

void
IDLString::typedef_decl_write (ostream          &ostr,
			       Indent           &indent,
			       IDLCompilerState &state,
			       const IDLTypedef &target,
			       const IDLTypedef *active_typedef) const
{
	string target_id = target.get_cpp_identifier ();
	
	ostr << indent << "typedef " << char_ << "* " << target_id << ';' << endl;	
	ostr << indent << "typedef ::CORBA::" << String_ << "_var " << target_id << "_var;" << endl;
}

string
IDLString::stub_decl_arg_get (const string     &cpp_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	string retval;

	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = char_ + " const *" + cpp_id;
		break;
	case IDL_PARAM_INOUT:
		retval = char_ + " *&" + cpp_id;
		break;
	case IDL_PARAM_OUT:
		retval = "::CORBA::" + String_ + "_out " + cpp_id;
		break;
	}

	return retval;
}

void
IDLString::stub_impl_arg_pre (ostream        &ostr,
			      Indent         &indent,
			      const string   &cpp_id,
			      IDL_param_attr  direction,
			      const IDLTypedef *active_typedef) const
{
	// Do nothing
}
	
string
IDLString::stub_impl_arg_call (const string   &cpp_id,
			       IDL_param_attr  direction,
			       const IDLTypedef *active_typedef) const
{
	string retval;

	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = cpp_id;
		break;
	case IDL_PARAM_INOUT:
		retval = '&' + cpp_id;
		break;
	case IDL_PARAM_OUT:
		retval = "&(" + char_ + "*&) " + cpp_id;
		break;
	}

	return retval;
}
	
void
IDLString::stub_impl_arg_post (ostream        &ostr,
			       Indent         &indent,
			       const string   &cpp_id,
			       IDL_param_attr  direction,
			       const IDLTypedef *active_typedef) const
{
	// Do nothing
}




string
IDLString::stub_decl_ret_get (const IDLTypedef *active_typedef) const
{
	return char_ + "*";
}
	
void
IDLString::stub_impl_ret_pre (ostream &ostr,
			      Indent  &indent,
			      const IDLTypedef *active_typedef) const
{
	// Do nothing
}

void
IDLString::stub_impl_ret_call (ostream      &ostr,
			       Indent       &indent,
			       const string &c_call_expression,
			       const IDLTypedef *active_typedef) const
{
	ostr << indent << char_ << "* _retval = " << c_call_expression
	     << ';' << endl;
}

void
IDLString::stub_impl_ret_post (ostream &ostr,
			       Indent  &indent,
			       const IDLTypedef *active_typedef) const
{
	ostr << indent << "return _retval;" << endl;
}
	



string
IDLString::skel_decl_arg_get (const string     &c_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	string retval;
	
	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = char_ + " const *" + c_id;
		break;
	case IDL_PARAM_INOUT:
	case IDL_PARAM_OUT:
		retval = char_ + "** " + c_id;
		break;
	}

	return retval;	
}

void
IDLString::skel_impl_arg_pre (ostream        &ostr,
			      Indent         &indent,
			      const string   &c_id,
			      IDL_param_attr  direction,
			      const IDLTypedef *active_typedef) const
{
	// Do nothing
}
	
string
IDLString::skel_impl_arg_call (const string   &c_id,
			       IDL_param_attr  direction,
			       const IDLTypedef *active_typedef) const
{
	string retval;
	
	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = c_id;
		break;
	case IDL_PARAM_INOUT:
		retval = "*" + c_id;
		break;
	case IDL_PARAM_OUT:
		retval = "::CORBA::" + String_ + "_out (*" + c_id + ")";
		break;
	}

	return retval;	
}
	
void
IDLString::skel_impl_arg_post (ostream        &ostr,
			       Indent         &indent,
			       const string   &c_id,
			       IDL_param_attr  direction,
			       const IDLTypedef *active_typedef) const
{
	// Do nothing
}




string
IDLString::skel_decl_ret_get (const IDLTypedef *active_typedef) const
{
	return char_ + "*";
}

void
IDLString::skel_impl_ret_pre (ostream &ostr,
			      Indent  &indent,
			      const IDLTypedef *active_typedef) const
{
	ostr << indent << char_ << "* _retval = 0;\n";
}

void
IDLString::skel_impl_ret_call (ostream      &ostr,
			       Indent       &indent,
			       const string &cpp_call_expression,
			       const IDLTypedef *active_typedef) const
{
	ostr << indent << "_retval = " << cpp_call_expression
	     << ';' << endl;
}

void
IDLString::skel_impl_ret_post (ostream &ostr,
			       Indent  &indent,
			       const IDLTypedef *active_typedef) const
{
	ostr << indent << "return _retval;" << endl;
}


string
IDLString::get_cpp_member_typename (const IDLTypedef *active_typedef) const
{
	return "CORBA::" + String_ + "_var";
}

string
IDLString::get_c_member_typename (const IDLTypedef *active_typedef) const
{
	return "CORBA_" + string_;
}

string
IDLString::member_decl_arg_get (const IDLTypedef *active_typedef) const
{
	return char_ + " const*";
}

void
IDLString::member_impl_arg_copy (ostream      &ostr,
				 Indent       &indent,
				 const string &cpp_id,
				 const IDLTypedef *active_typedef) const
{
	ostr << indent << cpp_id << " = _par_" << cpp_id
	     << ';' << endl;
}

void
IDLString::member_init_c (ostream          &ostr,
			  Indent           &indent,
			  const string     &c_id,
			  const IDLTypedef *active_typedef) const
{
	ostr << indent << c_id << " = CORBA::" + string_ + "_dup (\"\");" << endl;
}

void
IDLString::member_pack_to_c (ostream      &ostr,
			     Indent       &indent,
			     const string &cpp_id,
			     const string &c_id,
			     const IDLTypedef *active_typedef) const
{
	ostr << indent << c_id << " = "
	     << "CORBA::" << string_ << "_dup (" << cpp_id << ")"
	     << ';' << endl;
}

void
IDLString::member_unpack_from_c (ostream      &ostr,
				 Indent       &indent,
				 const string &cpp_id,
				 const string &c_id,
				 const IDLTypedef *active_typedef) const
{
	ostr << indent << cpp_id << " = CORBA::" << string_ << "_dup (" << c_id << ")"
	     << ';' << endl;
}

void
IDLString::create_union_accessors(IDLUnion const& un, IDLCaseStmt const& case_stmt, std::ostream& header, Indent indent, std::ostream& module, Indent mod_indent) const
{
	const IDLMember& member = case_stmt.get_member ();

	const string member_name = member.get_cpp_identifier ();

	string full_member_name = (un.is_fixed ()) ?
		"m_target._u." + member_name :
		"m_target->_u." + member_name;
	
	// Getter accessor
	header << indent << char_ << " const* "
		<< member_name << " () const { return "
		<< full_member_name << "; }" << endl;
	

	// Setter accessor
	string discr_val;
	if (case_stmt.isDefault ())
		discr_val = un.get_discriminator_default_value ();
	else
		discr_val = *(case_stmt.labelsBegin());
	

	// void z(const String_var&);
	header << indent << "void " << member_name
		 << " (CORBA::" << String_ << "_var const& val);" << endl << endl;
	
	module << mod_indent << "void "
		 << un.get_cpp_method_prefix () << "::" << member_name
		 << " (CORBA::" << String_ << "_var const& val)" << endl
		 << mod_indent++ << "{" << endl;

	module << mod_indent << "_clear_member ();" << endl;
	module << mod_indent << "_d (" << discr_val << ");" << endl;

	module << mod_indent << full_member_name << " = CORBA::" << string_ << "_dup(val);";

	module << --mod_indent << "}" << endl << endl;

	// void z(const char*)
	header << indent << "void " << member_name
		 << " (" << char_ << " const* val);" << endl << endl;
	
	module << mod_indent << "void "
		 << un.get_cpp_method_prefix () << "::" << member_name
		 << " (" << char_ << " const* val)" << endl
		 << mod_indent++ << "{" << endl;

	module << mod_indent << "_clear_member ();" << endl;
	module << mod_indent << "_d (" << discr_val << ");" << endl;

	module << mod_indent << full_member_name << " = CORBA::" << string_ << "_dup(val);";
	module << --mod_indent << "}" << endl << endl;

	// void z(char*)
	header << indent << "void " << member_name
		 << " (" << char_ << "* val);" << endl << endl;
	
	module << mod_indent << "void "
		 << un.get_cpp_method_prefix () << "::" << member_name
		 << " (" << char_ << "* val)" << endl
		 << mod_indent++ << "{" << endl;

	module << mod_indent << "_clear_member ();" << endl;
	module << mod_indent << "_d (" << discr_val << ");" << endl;

	module << mod_indent << full_member_name << " = val;";

	module << --mod_indent << "}" << endl << endl;
}
