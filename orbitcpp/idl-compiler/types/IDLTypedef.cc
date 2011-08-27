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

#include "IDLTypedef.h"
#include "types.h"
#include "language.h"

IDLTypedef::IDLTypedef(IDL_tree node)
:	IDLUserDefType(IDL_IDENT(simple_dcl_ident(node)).str,node,0),
	IDLIdentified (simple_dcl_ident(node))
{
	std::string dummy;
	IDL_tree dcl_list = IDL_NODE_UP(node);
	IDL_tree type_dcl = IDL_NODE_UP(dcl_list);
	IDL_tree type_spec = IDL_TYPE_DCL(type_dcl).type_spec;
	m_alias = IDLTypeParser::parseDcl(node, type_spec, dummy);
}

void
IDLTypedef::const_decl_write (ostream          &header,
			      ostream& module,
			      IDLScope const& scope,
			      Indent           &indent,
			      const string     &cpp_id,
			      const string     &value,
			      const IDLTypedef *active_typedef) const
{
	m_alias->const_decl_write (header, module, scope, indent, cpp_id, value,
				  active_typedef ? active_typedef : this);
}

void
IDLTypedef::typedef_decl_write (ostream          &ostr,
				Indent           &indent,
				IDLCompilerState &state,
				const IDLTypedef &target,
				const IDLTypedef *active_typedef) const
{
	// Don't fill active_typedef with `this'
	m_alias->typedef_decl_write (ostr, indent, state, target, active_typedef);
}

string
IDLTypedef::stub_decl_arg_get (const string     &cpp_id,
			       IDL_param_attr    direction,
			       const IDLTypedef *active_typedef) const
{
	return m_alias->stub_decl_arg_get (cpp_id, direction,
					  active_typedef ? active_typedef : this);
}

void
IDLTypedef::stub_impl_arg_pre (ostream          &ostr,
			       Indent           &indent,
			       const string     &cpp_id,
			       IDL_param_attr    direction,
			       const IDLTypedef *active_typedef) const
{
	m_alias->stub_impl_arg_pre (ostr, indent, cpp_id, direction,
				   active_typedef ? active_typedef : this);
}

string
IDLTypedef::stub_impl_arg_call (const string     &cpp_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	return m_alias->stub_impl_arg_call (cpp_id, direction,
					   active_typedef ? active_typedef : this);
}
	
void
IDLTypedef::stub_impl_arg_post (ostream          &ostr,
				Indent           &indent,
				const string     &cpp_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	m_alias->stub_impl_arg_post (ostr, indent, cpp_id, direction,
				    active_typedef ? active_typedef : this);
}




string
IDLTypedef::stub_decl_ret_get (const IDLTypedef *active_typedef) const
{
	return m_alias->stub_decl_ret_get (active_typedef ? active_typedef : this);
}
	
void
IDLTypedef::stub_impl_ret_pre (ostream          &ostr,
			       Indent           &indent,
			       const IDLTypedef *active_typedef) const
{
	m_alias->stub_impl_ret_pre (ostr, indent,
				   active_typedef ? active_typedef : this);
}

void
IDLTypedef::stub_impl_ret_call (ostream          &ostr,
				Indent           &indent,
				const string     &c_call_expression,
				const IDLTypedef *active_typedef) const
{
	m_alias->stub_impl_ret_call (ostr, indent, c_call_expression,
				   active_typedef ? active_typedef : this);
}

void
IDLTypedef::stub_impl_ret_post (ostream          &ostr,
				Indent           &indent,
				const IDLTypedef *active_typedef) const
{
	m_alias->stub_impl_ret_post (ostr, indent,
				   active_typedef ? active_typedef : this);
}
	



string
IDLTypedef::skel_decl_arg_get (const string     &c_id,
			       IDL_param_attr    direction,
			       const IDLTypedef *active_typedef) const
{
	return m_alias->skel_decl_arg_get (c_id, direction,
					  active_typedef ? active_typedef : this);
}

void
IDLTypedef::skel_impl_arg_pre (ostream        &ostr,
			       Indent         &indent,
			       const string   &c_id,
			       IDL_param_attr  direction,
			       const IDLTypedef *active_typedef) const
{
	m_alias->skel_impl_arg_pre (ostr, indent, c_id, direction,
				   active_typedef ? active_typedef : this);
}
	
string
IDLTypedef::skel_impl_arg_call (const string     &c_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	return m_alias->skel_impl_arg_call (c_id, direction,
					   active_typedef ? active_typedef : this);
}
	
void
IDLTypedef::skel_impl_arg_post (ostream          &ostr,
				Indent           &indent,
				const string     &c_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	m_alias->skel_impl_arg_post (ostr, indent, c_id, direction,
				    active_typedef ? active_typedef : this);
}




string
IDLTypedef::skel_decl_ret_get (const IDLTypedef *active_typedef) const
{
	return m_alias->skel_decl_ret_get (active_typedef ? active_typedef : this);
}

void
IDLTypedef::skel_impl_ret_pre (ostream          &ostr,
			       Indent           &indent,
			       const IDLTypedef *active_typedef) const
{
	m_alias->skel_impl_ret_pre (ostr, indent,
				   active_typedef ? active_typedef : this);
}

void
IDLTypedef::skel_impl_ret_call (ostream          &ostr,
				Indent           &indent,
				const string     &cpp_call_expression,
				const IDLTypedef *active_typedef) const
{
	m_alias->skel_impl_ret_call (ostr, indent, cpp_call_expression,
				    active_typedef ? active_typedef : this);
}

void
IDLTypedef::skel_impl_ret_post (ostream          &ostr,
				Indent           &indent,
				const IDLTypedef *active_typedef) const
{
	m_alias->skel_impl_ret_post (ostr, indent,
				    active_typedef ? active_typedef : this);
}



string
IDLTypedef::get_cpp_member_typename (const IDLTypedef *active_typedef) const
{
	return m_alias->get_cpp_member_typename (active_typedef ? active_typedef : this);
}

string
IDLTypedef::get_c_member_typename (const IDLTypedef *active_typedef) const
{
	return m_alias->get_c_member_typename (active_typedef ? active_typedef : this);
}

string
IDLTypedef::get_seq_typename (unsigned int      length,
			      const IDLTypedef *active_typedef) const
{
	return m_alias->get_seq_typename (length, active_typedef ? active_typedef : this);
}

string
IDLTypedef::member_decl_arg_get (const IDLTypedef *active_typedef) const
{
	return m_alias->member_decl_arg_get (active_typedef ? active_typedef : this);
}

void
IDLTypedef::member_impl_arg_copy (ostream          &ostr,
				  Indent           &indent,
				  const string     &cpp_id,
				  const IDLTypedef *active_typedef) const
{
	m_alias->member_impl_arg_copy (ostr, indent, cpp_id,
				      active_typedef ? active_typedef : this);
}


void
IDLTypedef::member_init_cpp (ostream          &ostr,
			     Indent           &indent,
			     const string     &cpp_id,
			     const IDLTypedef *active_typedef) const
{
	m_alias->member_init_cpp (ostr, indent, cpp_id,
				 active_typedef ? active_typedef : this);
}

void
IDLTypedef::member_init_c (ostream          &ostr,
			   Indent           &indent,
			   const string     &c_id,
			   const IDLTypedef *active_typedef) const
{
	m_alias->member_init_c (ostr, indent, c_id,
			       active_typedef ? active_typedef : this);
}

void
IDLTypedef::member_pack_to_c (ostream          &ostr,
			      Indent           &indent,
			      const string     &cpp_id,
			      const string     &c_id,
			      const IDLTypedef *active_typedef) const
{
	m_alias->member_pack_to_c (ostr, indent, cpp_id, c_id,
				  active_typedef ? active_typedef : this);
}

void
IDLTypedef::member_unpack_from_c (ostream          &ostr,
				  Indent           &indent,
				  const string     &cpp_id,
				  const string     &c_id,
				  const IDLTypedef *active_typedef) const
{
	m_alias->member_unpack_from_c (ostr, indent, cpp_id, c_id,
				      active_typedef ? active_typedef : this);
}
