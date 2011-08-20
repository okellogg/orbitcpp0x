/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: f; c-basic-offset: 8 -*- */
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

#include "IDLInterface.h"

#include "IDLTypedef.h"

namespace
{
	
string get_c_id (const string &cpp_id)
{
	return "_c_" + cpp_id;
}

string get_cpp_id (const string &c_id)
{
	return "_cpp_" + c_id;
}

} // Anonymous namespace

IDL_tree uninhibit(IDL_tree iface)
{
	if (IDL_NODE_TYPE(iface) == IDLN_IDENT)
		iface = IDL_NODE_UP(iface);

	// check
	IDL_INTERFACE(iface);

	return iface;
}

void
IDLInterfaceBase::typedef_decl_write (ostream          &ostr,
				  Indent           &indent,
				  IDLCompilerState &state,
				  const IDLTypedef &target,
				  const IDLTypedef *active_typedef) const
{
	const string src_id = get_cpp_typename ();
	const string target_id = target.get_cpp_identifier ();

	ostr << indent << "typedef " << src_id << " " << target_id << ";" << endl;
	ostr << indent << "typedef " << src_id << "_ptr " << target_id << "_ptr;" << endl;
	ostr << indent << "typedef " << src_id << "_var " << target_id << "_var;" << endl;
	ostr << indent << "typedef " << src_id << "_out " << target_id << "_out;" << endl;
	ostr << indent << "typedef " << src_id << "Ref " << target_id << "Ref;" << endl;

	string ns_iface_begin, ns_iface_end;
	target.getParentScope ()->getCPPNamespaceDecl (ns_iface_begin, ns_iface_end);
	
	string ns_poa_begin, ns_poa_end;
	target.getParentScope ()->getCPPNamespaceDecl (ns_poa_begin, ns_poa_end, "POA_");
	
	// Enter the POA namespace of the target to create the POA typedef
	ostr << indent << ns_iface_end << endl;

	ostr << indent << ns_poa_begin << endl;
	ostr << indent << "typedef " << src_id << " " << target_id << ";" << endl;
	ostr << indent << ns_poa_end << endl;
	
	ostr << indent << ns_iface_begin << endl;
}


string
IDLInterfaceBase::stub_decl_arg_get (const string     &cpp_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef) const
{
	string retval;

	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = get_cpp_typename_ptr () + " " + cpp_id;
		break;
	case IDL_PARAM_INOUT:
		retval = get_cpp_typename_ptr () + " &" + cpp_id;
		break;
	case IDL_PARAM_OUT:
		retval = get_cpp_typename_out () + " " + cpp_id;
		break;
	}

	return retval;
}

void
IDLInterfaceBase::stub_impl_arg_pre (ostream        &ostr,
				 Indent         &indent,
				 const string   &cpp_id,
				 IDL_param_attr  direction,
				 const IDLTypedef *active_typedef) const
{
	const string unwrap = cpp_id + " ? " + cpp_id + "->_orbitcpp_cobj () : CORBA_OBJECT_NIL";
		
	switch (direction)
	{
	case IDL_PARAM_IN:
		ostr << indent << "const " << get_c_typename () << " "
		     << get_c_id (cpp_id) << " = " << unwrap << ";" << endl;
		break;

	case IDL_PARAM_INOUT:
		ostr << indent << get_c_typename () << " "
		     << get_c_id (cpp_id) << " = " << unwrap << ";" << endl;
		break;
		
	case IDL_PARAM_OUT:
		ostr << indent << get_c_typename () << " "
		     << get_c_id (cpp_id) << " = " << "CORBA_OBJECT_NIL;"
		     << endl;
		break;
	}
}
	
string
IDLInterfaceBase::stub_impl_arg_call (const string   &cpp_id,
				  IDL_param_attr  direction,
				  const IDLTypedef *active_typedef) const
{
	string retval;
	
	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = get_c_id (cpp_id);
		break;
	case IDL_PARAM_INOUT:
	case IDL_PARAM_OUT:
		retval = "&" + get_c_id (cpp_id);
		break;
	}

	return retval;
}
	
void
IDLInterfaceBase::stub_impl_arg_post (ostream        &ostr,
				  Indent         &indent,
				  const string   &cpp_id,
				  IDL_param_attr  direction,
				  const IDLTypedef *active_typedef) const
{
	switch (direction)
	{
	case IDL_PARAM_IN:
		// Do nothing
		break;
	case IDL_PARAM_INOUT:
	case IDL_PARAM_OUT:
		ostr << indent << cpp_id << " = "
		     << get_cpp_stub_typename () << "::_orbitcpp_wrap"
		     << " (" << get_c_id (cpp_id) << ");"
		     << endl;
	}
}




	
void
IDLInterfaceBase::stub_impl_ret_pre  (ostream &ostr,
				  Indent  &indent,
				  const IDLTypedef *active_typedef) const
{
	// Do nothing
}

void
IDLInterfaceBase::stub_impl_ret_call (ostream      &ostr,
				  Indent       &indent,
				  const string &c_call_expression,
				  const IDLTypedef *active_typedef) const
{
	ostr << indent << get_c_typename () << " _retval = "
	     << c_call_expression << ";" << endl;
}

void
IDLInterfaceBase::stub_impl_ret_post (ostream &ostr,
				  Indent  &indent,
				  const IDLTypedef *active_typedef) const
{
	ostr << indent << "return " << get_cpp_stub_typename ()
	     << "::_orbitcpp_wrap (_retval);" << endl;
}

string
IDLInterfaceBase::stub_decl_ret_get (const IDLTypedef *active_typedef) const
{
	return get_cpp_typename_ptr ();
}

string
IDLInterfaceBase::skel_decl_arg_get (const string     &c_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef) const
{
	string retval;
	
	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = get_c_typename () + " " + c_id;
		break;
	case IDL_PARAM_INOUT:
	case IDL_PARAM_OUT:
		retval = get_c_typename () + " *" + c_id;
		break;
	}

	return retval;
}

void
IDLInterfaceBase::skel_impl_arg_pre  (ostream        &ostr,
				  Indent         &indent,
				  const string   &c_id,
				  IDL_param_attr  direction,
				  const IDLTypedef *active_typedef) const
{
	switch (direction)
	{
	case IDL_PARAM_IN:
		ostr << indent << get_cpp_typename_var () << " "
		     << get_cpp_id (c_id) << " = " << get_cpp_stub_typename ()
		     << "::_orbitcpp_wrap (" << c_id << ", true);" << endl;
		break;
	case IDL_PARAM_INOUT:
		ostr << indent << get_cpp_typename_var () << " "
		     << get_cpp_id (c_id) << " = " << get_cpp_stub_typename ()
		     << "::_orbitcpp_wrap (*" << c_id << ", true);" << endl;
		break;
	case IDL_PARAM_OUT:
		ostr << indent << get_cpp_typename_var () << " "
		     << get_cpp_id (c_id) << ";" << endl;
		break;
	}
}
	
string
IDLInterfaceBase::skel_impl_arg_call (const string   &c_id,
				  IDL_param_attr  direction,
				  const IDLTypedef *active_typedef) const
{
	switch (direction)
	{
	case IDL_PARAM_IN:
		return get_cpp_id (c_id) + ".in()";
	case IDL_PARAM_INOUT:
		return get_cpp_id (c_id) + ".inout()";
	case IDL_PARAM_OUT:
		return get_cpp_id (c_id) + ".out()";
	}

	return get_cpp_id (c_id);
}
	
void
IDLInterfaceBase::skel_impl_arg_post (ostream        &ostr,
				  Indent         &indent,
				  const string   &c_id,
				  IDL_param_attr  direction,
				  const IDLTypedef *active_typedef) const
{
	switch (direction)
	{
	case IDL_PARAM_IN:
		// Do nothing
		break;
	case IDL_PARAM_INOUT:
		ostr << indent << "*" << c_id << " = ::_orbitcpp::duplicate_cobj_guarded(" << get_cpp_id (c_id)
		     << "._retn ());"
		     << endl;
		break;
	case IDL_PARAM_OUT:
		ostr << indent << "*" << c_id << " = ::_orbitcpp::duplicate_cobj_guarded(" << get_cpp_id (c_id)
		     << ");" << endl;
		break;
	}
}




string
IDLInterfaceBase::skel_decl_ret_get (const IDLTypedef *active_typedef) const
{
	return get_c_typename ();
}

void
IDLInterfaceBase::skel_impl_ret_pre  (ostream &ostr,
				  Indent  &indent,
				  const IDLTypedef *active_typedef) const
{
	ostr << indent << get_cpp_typename_var () << " _retval"
	     << ';' << endl;
}

void
IDLInterfaceBase::skel_impl_ret_call (ostream      &ostr,
				  Indent       &indent,
				  const string &cpp_call_expression,
				  const IDLTypedef *active_typedef) const
{
	ostr << indent << " _retval = " << cpp_call_expression
	     << ';' << endl;
}

void
IDLInterfaceBase::skel_impl_ret_post (ostream &ostr,
				  Indent  &indent,
				  const IDLTypedef *active_typedef) const
{
	ostr << indent << "return ::_orbitcpp::duplicate_cobj_guarded(_retval);" << endl;
}


/***************************************************
 * Member
 ***************************************************/

string
IDLInterfaceBase::get_cpp_member_typename (const IDLTypedef *active_typedef) const
{
	return get_cpp_typename_var ();
}

string
IDLInterfaceBase::get_c_member_typename (const IDLTypedef *active_typedef) const
{
	return get_c_typename ();
}


string
IDLInterfaceBase::member_decl_arg_get (const IDLTypedef *active_typedef) const
{
	return get_cpp_typename_ptr ();
}
	
void
IDLInterfaceBase::member_impl_arg_copy (ostream      &ostr,
				    Indent       &indent,
				    const string &cpp_id,
				    const IDLTypedef *active_typedef) const
{
#warning "WRITE ME"
	// WRITE ME
	ostr << indent << cpp_id << " = _par_" << cpp_id
	     << ';' << endl;
}


void
IDLInterfaceBase::member_init_c (ostream          &ostr,
			     Indent           &indent,
			     const string     &c_id,
			     const IDLTypedef *active_typedef) const
{
#warning "WRITE ME"
}

void
IDLInterfaceBase::member_pack_to_c (ostream          &ostr,
				Indent           &indent,
				const string     &cpp_id,
				const string     &c_id,
				const IDLTypedef *active_typedef) const
{
	ostr << indent << c_id << " = (!::CORBA::is_nil(" << cpp_id << ".in()) ? ::_orbitcpp::duplicate_guarded("
	     << cpp_id << ".in()->_orbitcpp_cobj ()) : CORBA_OBJECT_NIL)" << ';' << endl;
}

void
IDLInterfaceBase::member_unpack_from_c (ostream      &ostr,
				    Indent       &indent,
				    const string &cpp_id,
				    const string &c_id,
				    const IDLTypedef *active_typedef) const
{
	ostr << indent << cpp_id << " = "
	     << get_cpp_stub_typename () << "::_orbitcpp_wrap ("
	     << "::_orbitcpp::duplicate_guarded (" << c_id << ")"
	     << ")"
	     << ';' << endl;
}

string
IDLInterfaceBase::get_cpp_typename_ptr () const
{
	return get_cpp_typename () + "_ptr";
}

string
IDLInterfaceBase::get_cpp_typename_var () const
{
	return get_cpp_typename () + "_var";
}

string
IDLInterfaceBase::get_cpp_typename_out () const
{
	return get_cpp_typename () + "_out";
}

string
IDLInterfaceBase::get_cpp_identifier_ptr () const
{
	return get_cpp_identifier () + "_ptr";
}

string
IDLInterfaceBase::get_cpp_identifier_var () const
{
	return get_cpp_identifier () + "_var";
}

string
IDLInterfaceBase::get_cpp_identifier_out () const
{
	return get_cpp_identifier () + "_out";
}

string
IDLInterfaceBase::get_cpp_stub_identifier () const
{
	return get_cpp_identifier ();
}

string
IDLInterfaceBase::get_cpp_stub_typename () const
{
	return "::" + get_cpp_typename ();
}

string
IDLInterfaceBase::get_cpp_stub_method_prefix () const
{
	string retval = get_cpp_stub_typename ();

	// Remove :: from head
	string::iterator i = retval.begin ();
	while (i != retval.end () && *i == ':')
		i = retval.erase (i);
	
	return retval;
}



string
IDLInterfaceRight::get_cpp_poa_identifier () const
{
	return get_cpp_identifier ();
}

string
IDLInterfaceRight::get_cpp_poa_typename () const
{
	return "::" + get_cpp_poa_method_prefix ();
}

string
IDLInterfaceRight::get_cpp_poa_method_prefix () const
{
	string cpp_typename = get_cpp_typename ();

	// Remove :: from head
	string::iterator i = cpp_typename.begin ();
	while (i != cpp_typename.end () && *i == ':')
		i = cpp_typename.erase (i);
	
	return "POA_" + cpp_typename;	
}

string
IDLInterfaceRight::get_c_poa_typename () const
{
	return "POA_" + get_c_typename ();
}

string
IDLInterfaceRight::get_c_poa_epv () const
{
	return get_c_poa_typename () + "__epv";
}

string
IDLInterfaceRight::get_c_poa_vepv () const
{
	return get_c_poa_typename () + "__vepv";
}

struct PushIdent {
	std::vector<std::string>& ident_;

	PushIdent(std::vector<std::string>& ident) : ident_ (ident) { }

	PushIdent& operator++() { return *this; }
	PushIdent& operator++(int) { return *this; }
	PushIdent& operator*() { return *this; }
	PushIdent& operator=(IDL_tree node)
	{
		ident_.push_back(IDL_IDENT(IDL_MODULE(node).ident).str);
		return *this;
	}
};

void
IDLInterfaceRight::common_write_typedefs (ostream &ostr,
				     Indent  &indent) const
{
		
	ostr << indent << "typedef " << get_cpp_stub_typename ()
	     << "* " << get_cpp_identifier_ptr ()
	     << ';' << endl;
	
	ostr << indent << "typedef "IDL_IMPL_NS "::ObjectPtr_var"
	     << "<" << get_cpp_identifier () << "> "
	     << get_cpp_identifier_var ()
	     << ';' << endl;
	
	
	ostr << indent << "typedef " IDL_IMPL_NS "::ObjectPtr_out"
	     << "<" << get_cpp_identifier () << "> "
	     << get_cpp_identifier_out ()
	     << ';' << endl;

	ostr << indent << "typedef " << get_cpp_identifier_ptr ()
	     << " " << get_cpp_identifier () << "Ref"
	     << ';' << endl;
	
}

