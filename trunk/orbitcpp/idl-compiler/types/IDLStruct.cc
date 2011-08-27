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

#include "IDLStruct.h"

#include "IDLMember.h"
#include "IDLTypedef.h"
#include "types.h"

IDLStruct::IDLStruct (IDL_tree      node)
:	IDLCompound (IDL_IDENT(IDL_TYPE_STRUCT(node).ident).str, node, 0),
	IDLIdentified (IDL_TYPE_STRUCT(node).ident)
{
	IDL_tree member = IDL_TYPE_STRUCT(node).member_list;

	for (; member != 0; member = IDL_LIST(member).next) {
		IDL_tree m = IDL_LIST(member).data;
		g_assert(IDL_NODE_TYPE(m) == IDLN_MEMBER);

		IDL_tree dcl = IDL_MEMBER(m).dcls;

		for (; dcl != 0; dcl = IDL_LIST(dcl).next) {
			IDL_tree d = IDL_LIST(dcl).data;
			std::string id;
			IDLType* type = IDLTypeParser::parseDcl(d, IDL_TYPE_DCL(m).type_spec, id);
			IDLElement* member = new IDLMember(type, id, d, 0);
			m_items->push_back(member);
		}
	}
}

#if 0
// Ignore this because there is too much casting required when working
// with arrays.
bool
IDLStruct::conversion_required () const
{
	bool have_nonsimple_member = false;
	for (const_iterator i = begin ();
	     i != end () && !have_nonsimple_member;
	     i++)
	{
		IDLMember &member = (IDLMember &) **i;
		have_nonsimple_member = member.getType ()->conversion_required ();
	}

	return have_nonsimple_member;
}
#endif

bool
IDLStruct::is_fixed () const
{
	bool fixed = true;
	for (const_iterator i = begin ();
	     i != end () && fixed;
	     i++)
	{
		IDLMember &member = (IDLMember &) **i;
		fixed = member.getType ()->is_fixed ();
	}

	return fixed;
}

void
IDLStructBase::typedef_decl_write (ostream          &ostr,
			       Indent           &indent,
			       IDLCompilerState &state,
			       const IDLTypedef &target,
			       const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

	// Struct type typedef
	ostr << indent << "typedef " << cpp_type
	     << " " << target.get_cpp_identifier () << ";" << endl;

	// _out typedef
	ostr << indent << "typedef " << cpp_type << "_out"
	     << " " << target.get_cpp_identifier () << "_out;" << endl;

#ifndef IDL2CPP0X
	if (!is_fixed ())
		// _var typedef
		ostr << indent << "typedef " << cpp_type << "_var"
		     << " " << target.get_cpp_identifier () << "_var;" << endl;
#endif
}

string
IDLStructBase::stub_decl_arg_get (const string     &cpp_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	string retval;
	string cpp_typename = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = "const " + cpp_typename + " &" + cpp_id;
		break;
	case IDL_PARAM_INOUT:
		retval = cpp_typename + " &" + cpp_id;
		break;
	case IDL_PARAM_OUT:
		retval = cpp_typename + "_out " + cpp_id;
		break;
	}

	return retval;
}

void
IDLStructBase::stub_impl_arg_pre (ostream          &ostr,
			      Indent           &indent,
			      const string     &cpp_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	string c_type = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();

	// Try to use casts if possible, otherwise use real conversion methods
	if (!conversion_required ())
	{
		string cast;
		
		switch (direction)
		{
		case IDL_PARAM_IN:
			cast = "(const " + c_type + "*)";
			c_type = "const " + c_type;
			break;
		case IDL_PARAM_INOUT:
		case IDL_PARAM_OUT:
			cast = "(" + c_type + "*)";
			break;
		}
		
		ostr << indent << c_type << " *_c_" << cpp_id << " = "
		     << cast + "&" + cpp_id << ";" << endl;

	} else {
		
		switch (direction)
		{
		case IDL_PARAM_IN:
		case IDL_PARAM_INOUT:
			ostr << indent << c_type << " *_c_" << cpp_id << " = "
			     << cpp_id << "._orbitcpp_pack ();" << endl;
			break;
		case IDL_PARAM_OUT:
			ostr << indent << c_type << " *_c_" << cpp_id
			     << " = NULL;" << endl;
			break;
		}
	}
}
	
string
IDLStructBase::stub_impl_arg_call (const string     &cpp_id,
			       IDL_param_attr    direction,
			       const IDLTypedef *active_typedef) const
{
	if (!is_fixed () && direction == IDL_PARAM_OUT)
		return "&_c_" + cpp_id;
	else
		return "_c_" + cpp_id;
}
	
void
IDLStructBase::stub_impl_arg_post (ostream          &ostr,
			       Indent           &indent,
			       const string     &cpp_id,
			       IDL_param_attr    direction,
			       const IDLTypedef *active_typedef) const
{
	if (!conversion_required ())
		// Do nothing
		return;

	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

	// Load back values
	switch (direction)
	{
	case IDL_PARAM_IN:
		// Do nothing
		break;
	case IDL_PARAM_INOUT:
		ostr << indent << cpp_id << "._orbitcpp_unpack "
		     << "(*_c_" << cpp_id << ");" << endl;
		break;
	case IDL_PARAM_OUT:
#ifdef IDL2CPP0X
		ostr << indent << cpp_id << "._orbitcpp_unpack "
		     << "(*_c_" << cpp_id << ");" << endl;
#else
		if (is_fixed ())
			ostr << indent << cpp_id << "._orbitcpp_unpack "
			     << "(*_c_" << cpp_id << ");" << endl;
		else
			ostr << indent << cpp_id << " = new " << cpp_type
			     << " (*_c_" << cpp_id << ");" << endl;
#endif
		break;
	}
	
	if (!is_fixed ())
	{
		ostr << indent << "CORBA_free (_c_" << cpp_id << ");" << endl;
	}
}




string
IDLStructBase::stub_decl_ret_get (const IDLTypedef *active_typedef) const
{
	string cpp_typename = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	string maybe_star;

#ifndef IDL2CPP0X
	if (!is_fixed ())
		maybe_star = "*";
#endif
	return cpp_typename + maybe_star;
}
	
void
IDLStructBase::stub_impl_ret_pre (ostream &ostr,
			      Indent  &indent,
			      const IDLTypedef *active_typedef) const
{
}

void
IDLStructBase::stub_impl_ret_call (ostream          &ostr,
			       Indent           &indent,
			       const string     &c_call_expression,
			       const IDLTypedef *active_typedef) const
{
	string c_typename = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();

	string ret_id = is_fixed () ? "_c_retval" : "*_c_retval";

	ostr << indent << c_typename << " " << ret_id << " = "
	     << c_call_expression << ";" << endl;
}

void
IDLStructBase::stub_impl_ret_post (ostream          &ostr,
			       Indent           &indent,
			       const IDLTypedef *active_typedef) const
{
	string cpp_typename = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

	if (!conversion_required ())
	{
		string cast = "(" + cpp_typename + "*)&";
		if (is_fixed ())
			cast = "*" + cast;
			
		ostr << indent << "return " << cast << "_c_retval;" << endl;
	} else {
#ifdef IDL2CPP0X
		ostr << indent << cpp_typename << " _cpp_retval;" << endl;
		ostr << indent << "_cpp_retval._orbitcpp_unpack  (";
		if (!is_fixed ())
			ostr << "*";
		ostr << "_c_retval);" << endl;
#else
		if (is_fixed ())
		{
			ostr << indent << cpp_typename << " _cpp_retval;" << endl;
			ostr << indent << "_cpp_retval._orbitcpp_unpack  (_c_retval);" << endl;
		} else {
			ostr << indent << cpp_typename << " *_cpp_retval = "
			     << "new " << cpp_typename << ";" << endl;
			ostr << indent << "_cpp_retval->_orbitcpp_unpack (*_c_retval);" << endl;
			ostr << indent << "CORBA_free (_c_retval);" << endl;
		}
#endif
		ostr << indent << "return _cpp_retval;" << endl;
	}
}
	



string
IDLStructBase::skel_decl_arg_get (const string     &c_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	string c_typename = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();
	string retval;

	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = "const " + c_typename + " *" + c_id;
		break;
	case IDL_PARAM_INOUT:
		retval = c_typename + " *" + c_id;
		break;		
	case IDL_PARAM_OUT:
		if (is_fixed ())
			retval = c_typename + " *" + c_id;
		else
			retval = c_typename + " **" + c_id;			
		break;		
	}

	return retval;
}

void
IDLStructBase::skel_impl_arg_pre (ostream          &ostr,
			      Indent           &indent,
			      const string     &c_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	string cpp_id = "_cpp_" + c_id;
	
	// Try to use casts if possible, otherwise use real conversion methods
	if (!conversion_required ())
	{
		string cast;
		
		switch (direction)
		{
		case IDL_PARAM_IN:
			cast = "(const " + cpp_type + "*)";
			cpp_type = "const " + cpp_type;
			break;
		case IDL_PARAM_INOUT:
		case IDL_PARAM_OUT:
			cast = "(" + cpp_type + "*)";
			break;
		}
		
		ostr << indent << cpp_type << " *" << cpp_id << " = "
		     << cast + c_id << ";" << endl;

	} else {
		
		switch (direction)
		{
		case IDL_PARAM_IN:
		case IDL_PARAM_INOUT:
			ostr << indent << cpp_type << " " << cpp_id << " (*" << c_id << ")"
			     << ";" << endl;
			break;
		case IDL_PARAM_OUT:
			ostr << indent << cpp_type;
#ifndef IDL2CPP0X
			ostr << "_var";
#endif
			ostr << " " << cpp_id << ";" << endl;
			break;
		}
	}
}
	
string
IDLStructBase::skel_impl_arg_call (const string     &c_id,
			       IDL_param_attr    direction,
			       const IDLTypedef *active_typedef) const
{
	if (!conversion_required ())
		return "*_cpp_" + c_id;

#ifndef IDL2CPP0X
	if (direction == IDL_PARAM_OUT)
		return get_cpp_typename () + "_out (_cpp_" + c_id + ")";
#endif

	return "_cpp_" + c_id;
}
	
void
IDLStructBase::skel_impl_arg_post (ostream          &ostr,
			       Indent           &indent,
			       const string     &c_id,
			       IDL_param_attr    direction,
			       const IDLTypedef *active_typedef) const
{
	string cpp_id = "_cpp_" + c_id;
	string ref;
	
	if (!conversion_required ())
		// Do nothing
		return;

	// Load back values
	switch (direction)
	{
	case IDL_PARAM_IN:
		// Do nothing
		break;
	case IDL_PARAM_INOUT:
		ostr << indent << cpp_id << "._orbitcpp_pack "
		     << "(*" << c_id << ");" << endl;
		break;
	case IDL_PARAM_OUT:
#ifdef IDL2CPP0X
		ref = ".";
#else
		ref = "->";
#endif
		if (is_fixed())
			ostr << indent << cpp_id << ref
			     << "_orbitcpp_pack (*" << c_id << ");" << endl;
		
		else
			ostr << indent << "*" << c_id << " = "
			     << cpp_id << ref << "_orbitcpp_pack ();" << endl;
		break;
	}
}




string
IDLStructBase::skel_decl_ret_get (const IDLTypedef *active_typedef) const
{
	string c_typename = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();

	if (is_fixed ())
		return c_typename;
	else
		return c_typename + "*";
}

void
IDLStructBase::skel_impl_ret_pre (ostream          &ostr,
			      Indent           &indent,
			      const IDLTypedef *active_typedef) const
{
	string cpp_typename = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	
	if (is_fixed ())
	{
		ostr << indent << cpp_typename << " _cpp_retval;" << endl;
	}
	else
	{
		ostr << indent << cpp_typename;
#ifdef IDL2CPP0X
		ostr << " _cpp_retval;" << endl;
#else
		ostr << "_var _cpp_retval = 0;" << endl;
#endif
	}
}

void
IDLStructBase::skel_impl_ret_call (ostream          &ostr,
			       Indent           &indent,
			       const string     &cpp_call_expression,
			       const IDLTypedef *active_typedef) const
{
	ostr << indent << "_cpp_retval = " << cpp_call_expression
	     << ";" << endl;
}

void
IDLStructBase::skel_impl_ret_post (ostream          &ostr,
			       Indent           &indent,
			       const IDLTypedef *active_typedef) const
{
	string c_typename = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();
	
	if (!conversion_required ())
	{
		string cast = "(" + c_typename + "*)&";
		if (is_fixed ())
			cast = "*" + cast;
		
		ostr << indent << "return " << cast << "_cpp_retval;"
		     << endl;
	} else {
		if (is_fixed ())
		{
			ostr << indent << c_typename << " _c_retval;" << endl;
			ostr << indent << "_cpp_retval._orbitcpp_pack (_c_retval);" << endl;
			ostr << indent << "return _c_retval;" << endl;
		} else {
			string ref;
#ifdef IDL2CPP0X
			ref = ".";
#else
			ref = "->";
#endif
			ostr << indent << "return _cpp_retval" << ref
			     << "_orbitcpp_pack ();" << endl;
		}
	}
}


string
IDLStructBase::get_cpp_member_typename (const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	
	return cpp_type;
}

string
IDLStructBase::get_c_member_typename (const IDLTypedef *active_typedef) const
{
	string c_type = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();

	return c_type;
}

string
IDLStructBase::member_decl_arg_get (const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

	return "const " + cpp_type + " &";
}

void
IDLStructBase::member_impl_arg_copy (ostream          &ostr,
				 Indent           &indent,
				 const string     &cpp_id,
				 const IDLTypedef *active_typedef) const
{
	ostr << indent;
#ifdef IDL2CPP0X
	ostr << "_";
#endif
	ostr << cpp_id << " = " << "_par_" << cpp_id << ";" << endl;
}

void
IDLStructBase::member_init_c (ostream          &ostr,
			  Indent           &indent,
			  const string     &cpp_id,
			  const IDLTypedef *active_typedef) const
{
}

void
IDLStructBase::member_pack_to_c (ostream          &ostr,
			     Indent           &indent,
			     const string     &cpp_id,
			     const string     &c_id,
			     const IDLTypedef *active_typedef) const
{
	// FIXME: Write passthrough optimization impl
	
	ostr << indent << cpp_id << "._orbitcpp_pack"
	     << " (" << c_id << ");" << endl;
}

void
IDLStructBase::member_unpack_from_c  (ostream          &ostr,
				  Indent           &indent,
				  const string     &cpp_id,
				  const string     &c_id,
				  const IDLTypedef *active_typedef) const
{
	// FIXME: Write passthrough optimization impl

	ostr << indent << cpp_id << "._orbitcpp_unpack"
	     << " (" << c_id << ");" << endl;	
}
