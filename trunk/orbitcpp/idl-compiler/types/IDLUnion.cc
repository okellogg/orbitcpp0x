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

#include "IDLUnion.h"
#include "debug.h"
#include "types.h"
#include "IDLUnionDiscriminator.h"
#include "IDLCaseStmt.h"
#include "IDLMember.h"
#include "IDLTypedef.h"
#include "IDLEnum.h"

void
IDLUnion::doCaseStmt(IDL_tree node, std::string scopePrefix)
{
	string id;

	// member
	IDL_tree member = IDL_CASE_STMT(node).element_spec;
	g_assert(IDL_NODE_TYPE(member) == IDLN_MEMBER);
	
	IDL_tree dcl = IDL_LIST(IDL_MEMBER(member).dcls).data;   // dont handle multiple dcls	
	g_assert(IDL_NODE_TYPE(dcl) == IDLN_IDENT);

	IDLType *type = 
		IDLTypeParser::parseDcl(dcl,IDL_TYPE_DCL(member).type_spec,id);
	
	IDLMember *themember = new IDLMember(type,id,dcl);  // don't attach this to the scope
	new IDLCaseStmt(themember, scopePrefix, node, this);  // attach the case stmt instead
	// case stmt takes ownership of member
}

void IDLUnion::doSwitchBody(IDL_tree  member_list)
{
	std::string scopePrefix = "";
	IDLEnum *enum_d = dynamic_cast<IDLEnum*>(m_discriminator);
	if (enum_d)
	{
		const IDLScope *discrimScope = enum_d->getParentScope();
		if (discrimScope != getParentScope())
			scopePrefix = enum_d->get_cpp_typename() + "::";
		// *mDebug << "// IDLUnion::doSwitchBody: scopePrefix = " << scopePrefix << endl;
	}
	for (; member_list != 0; member_list = IDL_LIST (member_list).next)
	{
		IDL_tree casestmt = IDL_LIST (member_list).data;
		g_assert (IDL_NODE_TYPE (casestmt) == IDLN_CASE_STMT);

		doCaseStmt (casestmt, scopePrefix);
	}
}

IDLUnion::IDLUnion(IDL_tree                     node)
:	IDLType (IDLType::T_UNION),
	IDLScope (IDL_IDENT(IDL_TYPE_UNION(node).ident).str, node, 0),
	IDLIdentified (IDL_TYPE_UNION(node).ident),
	m_discriminator (dynamic_cast<IDLUnionDiscriminator*>(IDLTypeParser::parseTypeSpec(0, IDL_TYPE_UNION(node).switch_type_spec)))
{
	doSwitchBody(IDL_TYPE_UNION(node).switch_body);
}

const IDLUnionDiscriminator &
IDLUnion::get_discriminator () const
{
	return *m_discriminator;
}

std::string
IDLUnion::get_discriminator_default_value () const
{
	StringSet explicit_values;
	for (const_iterator i = begin (); i != end (); ++i)
	{
		const IDLCaseStmt &case_stmt = static_cast<const IDLCaseStmt&> (**i);
		if (!case_stmt.isDefault ())
		{
			for (IDLCaseStmt::const_iterator l = case_stmt.labelsBegin ();
							 l != case_stmt.labelsEnd (); ++l)
				explicit_values.insert (*l);
		}
	}
	string result = m_discriminator->get_default_value (explicit_values);
	// *mDebug << "// IDLUnion::get_discriminator_default_value: result = " << result << endl;
	return result;
}

bool
IDLUnion::is_fixed () const
{
	bool all_fixed = true;
	for (const_iterator i = begin (); all_fixed && i != end (); ++i)
	{
		const IDLCaseStmt &case_stmt = static_cast<const IDLCaseStmt&> (**i);
		const IDLMember   &member = case_stmt.get_member ();
		
		all_fixed = member.getType ()->is_fixed ();
	}
	
	return all_fixed;
}

void
IDLUnion::typedef_decl_write (ostream          &ostr,
			      Indent           &indent,
			      IDLCompilerState &state,
			      const IDLTypedef &target,
			      const IDLTypedef *active_typedef) const
{
#warning "WRITE ME"
}

string
IDLUnion::stub_decl_arg_get (const string     &cpp_id,
			     IDL_param_attr    direction,
			     const IDLTypedef *active_typedef) const
{
	const string cpp_typename = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	string retval;
	
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
IDLUnion::stub_impl_arg_pre (ostream          &ostr,
			     Indent           &indent,
			     const string     &cpp_id,
			     IDL_param_attr    direction,
			     const IDLTypedef *active_typedef) const
{
	const string c_type = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();
	const string c_id = "_c_" + cpp_id;

	if (is_fixed ())
		ostr << indent << c_type << " " << c_id << ";" << endl;
	else
		ostr << indent << c_type << " *" << c_id << ";" << endl;
		
	
	switch (direction)
	{
	case IDL_PARAM_IN:
	case IDL_PARAM_INOUT:
		if (is_fixed ())
			ostr << indent << cpp_id << "._orbitcpp_pack ("
			     << c_id << ");" << endl;
		else	
			ostr << indent << c_id << " = " << cpp_id << "._orbitcpp_pack ()"
			     << ";" << endl;
		break;
	case IDL_PARAM_OUT:
		if (!is_fixed ())
			ostr << indent << c_id << " = " << c_type << "__alloc ()"
			     << ";" << endl;
		break;
	}		
}
	
string
IDLUnion::stub_impl_arg_call (const string     &cpp_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	if (is_fixed ())
		return "&_c_" + cpp_id;
	
	if (direction == IDL_PARAM_OUT)
		return "&_c_" + cpp_id;
	else
		return "_c_" + cpp_id;
}
	
void
IDLUnion::stub_impl_arg_post (ostream          &ostr,
			      Indent           &indent,
			      const string     &cpp_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	const string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	const string c_id = "_c_" + cpp_id;
	
	// Load back values
	switch (direction)
	{
	case IDL_PARAM_IN:
		// Do nothing
		break;
	case IDL_PARAM_INOUT:
		if (is_fixed ())
			ostr << indent << cpp_id << "._orbitcpp_unpack "
			     << "(" << c_id << ");" << endl;
		else
			ostr << indent << cpp_id << "._orbitcpp_unpack "
			     << "(*" << c_id << ");" << endl;
		break;
	case IDL_PARAM_OUT:
		if (is_fixed ())
		{
			ostr << indent << cpp_id << "._orbitcpp_unpack "
			     << "(" << c_id << ");" << endl;
		} else {
			ostr << indent << cpp_id << " = new " << cpp_type << ";" << endl;
			ostr << indent << cpp_id << "->_orbitcpp_unpack "
			     << "(*" << c_id << ");" << endl;			
		}
		break;
	}

	if (!is_fixed ())
		ostr << indent << "CORBA_free (_c_" << cpp_id << ");" << endl;
}




string
IDLUnion::stub_decl_ret_get (const IDLTypedef *active_typedef) const
{
	string cpp_typename = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	
	if (is_fixed ())
		return cpp_typename;
	else
		return cpp_typename + "*";
}
	
void
IDLUnion::stub_impl_ret_pre (ostream &ostr,
			     Indent  &indent,
			     const IDLTypedef *active_typedef) const
{
	// Do nothing
}

void
IDLUnion::stub_impl_ret_call (ostream          &ostr,
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
IDLUnion::stub_impl_ret_post (ostream          &ostr,
			      Indent           &indent,
			      const IDLTypedef *active_typedef) const
{
	string cpp_typename = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

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
		
	ostr << indent << "return _cpp_retval;" << endl;
}
	



string
IDLUnion::skel_decl_arg_get (const string     &c_id,
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
IDLUnion::skel_impl_arg_pre (ostream          &ostr,
			     Indent           &indent,
			     const string     &c_id,
			     IDL_param_attr    direction,
			     const IDLTypedef *active_typedef) const
{
	const string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	const string cpp_id = "_cpp_" + c_id;
	
	switch (direction)
	{
	case IDL_PARAM_IN:
	case IDL_PARAM_INOUT:
		ostr << indent << cpp_type << " " << cpp_id << " (*" << c_id << ")"
		     << ";" << endl;
		break;
	case IDL_PARAM_OUT:
		if (is_fixed ())
			ostr << indent << cpp_type << " " << cpp_id
			     << ";" << endl;
		else
			ostr << indent << cpp_type << "_var " << cpp_id
			     << ";" << endl;
		break;
	}
}
	
string
IDLUnion::skel_impl_arg_call (const string     &c_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	if (direction == IDL_PARAM_OUT && !is_fixed ())
		return get_cpp_typename () + "_out (_cpp_" + c_id + ")";

	return "_cpp_" + c_id;
}
	
void
IDLUnion::skel_impl_arg_post (ostream          &ostr,
			      Indent           &indent,
			      const string     &c_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *active_typedef) const
{
	const string cpp_id = "_cpp_" + c_id;
	
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
		if (is_fixed ())
			ostr << indent << cpp_id << "._orbitcpp_pack (*" << c_id << ")"
			     << ";" << endl;
		else
			ostr << indent << "*" << c_id << " = "
			     << cpp_id << "->_orbitcpp_pack ();" << endl;
		break;
	}
}




string
IDLUnion::skel_decl_ret_get (const IDLTypedef *active_typedef) const
{
	if (is_fixed ())
		return get_c_typename ();
	else
		return get_c_typename () + "*";
}

void
IDLUnion::skel_impl_ret_pre (ostream          &ostr,
			     Indent           &indent,
			     const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

	if (is_fixed ())
		ostr << indent << cpp_type << " _cpp_retval;" << endl;
	else
		ostr << indent << cpp_type << "_var _cpp_retval = 0;" << endl;
}

void
IDLUnion::skel_impl_ret_call (ostream          &ostr,
			      Indent           &indent,
			      const string     &cpp_call_expression,
			      const IDLTypedef *active_typedef) const
{
	ostr << indent << "_cpp_retval = " << cpp_call_expression
	     << ";" << endl;
}

void
IDLUnion::skel_impl_ret_post (ostream          &ostr,
			      Indent           &indent,
			      const IDLTypedef *active_typedef) const
{
	string c_typename = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();
	
	if (is_fixed ())
	{
		ostr << indent << c_typename << " _c_retval;" << endl;
		ostr << indent << "_cpp_retval._orbitcpp_pack (_c_retval);" << endl;
		ostr << indent << "return _c_retval;" << endl;
	} else {
		ostr << indent << "return _cpp_retval->_orbitcpp_pack ();" << endl;
	}
}


string
IDLUnion::get_cpp_member_typename (const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();
	
	return cpp_type;
}

string
IDLUnion::get_c_member_typename (const IDLTypedef *active_typedef) const
{
	string c_type = active_typedef ?
		active_typedef->get_c_typename () : get_c_typename ();

	return c_type;
}

string
IDLUnion::member_decl_arg_get (const IDLTypedef *active_typedef) const
{
	string cpp_type = active_typedef ?
		active_typedef->get_cpp_typename () : get_cpp_typename ();

	return "const " + cpp_type + " &";
}

void
IDLUnion::member_impl_arg_copy (ostream          &ostr,
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
IDLUnion::member_init_c (ostream          &ostr,
			 Indent           &indent,
			 const string     &c_id,
			 const IDLTypedef *active_typedef) const
{
#warning WRITEME
}

void
IDLUnion::member_pack_to_c (ostream          &ostr,
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
IDLUnion::member_unpack_from_c  (ostream          &ostr,
				 Indent           &indent,
				 const string     &cpp_id,
				 const string     &c_id,
				 const IDLTypedef *active_typedef) const
{
	// FIXME: Write passthrough optimization impl

	ostr << indent << cpp_id << "._orbitcpp_unpack"
	     << " (" << c_id << ");" << endl;	
}
