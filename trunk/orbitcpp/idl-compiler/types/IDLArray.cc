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

#include "IDLArray.h"

#include "IDLTypedef.h"
#include "orbitcpp/idl-compiler/pass.h"
#include "orbitcpp/idl-compiler/pass_xlate.h"

std::string IDLArray::get_props_id(IDLTypedef const& target) const
{ return target.get_cpp_identifier() + "Props"; }

IDLArray::IDLArray (IDL_tree element_type,
		    IDL_tree       node)
:	IDLType (IDLType::T_ARRAY),
	IDLElement  (IDL_IDENT(IDL_TYPE_ARRAY(node).ident).str, node, 0),
	IDLIdentified (IDL_TYPE_ARRAY(node).ident),
	m_element_type (IDLTypeParser::parseTypeSpec(0, element_type))
{
	IDL_tree subtype;

	for (IDL_tree curdim = IDL_TYPE_ARRAY (node).size_list;
	     curdim; curdim = IDL_LIST (curdim).next)
	{
		m_dims.push_back (IDL_INTEGER (IDL_LIST (curdim).data).value);
	}

array_redo:
	for (IDL_tree curdim = IDL_TYPE_ARRAY (node).size_list;
	     curdim; curdim = IDL_LIST (curdim).next)
	{
		m_copy_dims.push_back (IDL_INTEGER (IDL_LIST (curdim).data).value);
	}

	subtype = IDL_TYPE_DCL (
			IDL_get_parent_node (
				node, IDLN_TYPE_DCL, NULL)).type_spec;

	if (IDL_NODE_TYPE(subtype) == IDLN_IDENT && 
			IDL_NODE_TYPE(IDL_NODE_UP(subtype)) == IDLN_TYPE_ARRAY) {
		node = IDL_NODE_UP(subtype);
		goto array_redo;
	}
}

bool
IDLArray::conversion_required () const
{
	return m_element_type->conversion_required ();
}

bool
IDLArray::is_fixed () const
{
	return m_element_type->is_fixed ();
}

void
IDLArray::init_cpp_array (ostream      &ostr,
			  Indent       &indent,
			  const string &cpp_id) const
{
	unsigned int depth = 0;
	string array_pos;
	for (const_iterator i = begin (); i != end (); i++, depth++)
	{
		gchar *iterator_name = g_strdup_printf ("i_%d", depth);
		array_pos += "[";
		array_pos += iterator_name;
		array_pos += "]";

		ostr << indent
		     << "for (CORBA::ULong " << iterator_name << " = 0; "
		     << iterator_name << " < " << *i << "; "
		     << iterator_name << "++)" << endl;
		ostr << indent++ << "{" << endl;

		g_free (iterator_name);
	}

	m_element_type->member_init_cpp (ostr, indent, cpp_id + array_pos);

	for (; depth; depth--)
	{
		ostr << --indent << "}" << endl;
	}	
}

void
IDLArray::init_c_array (ostream      &ostr,
			Indent       &indent,
			const string &c_id) const
{
	unsigned int depth = 0;
	string array_pos;
	for (const_iterator i = begin (); i != end (); i++, depth++)
	{
		gchar *iterator_name = g_strdup_printf ("i_%d", depth);
		array_pos += "[";
		array_pos += iterator_name;
		array_pos += "]";

		ostr << indent
		     << "for (CORBA::ULong " << iterator_name << " = 0; "
		     << iterator_name << " < " << *i << "; "
		     << iterator_name << "++)" << endl;
		ostr << indent++ << "{" << endl;

		g_free (iterator_name);
	}

	m_element_type->member_init_c (ostr, indent, c_id + array_pos);

	for (; depth; depth--)
	{
		ostr << --indent << "}" << endl;
	}	
}

void
IDLArray::copy_cpp_array (ostream      &ostr,
			  Indent       &indent,
			  const string &src_id,
			  const string &dest_id) const
{
	unsigned int depth = 0;
	string array_pos;
	for (const_iterator i = m_copy_dims.begin (); i != m_copy_dims.end (); i++, depth++)
	{
		gchar *iterator_name = g_strdup_printf ("i_%d", depth);
		array_pos += "[";
		array_pos += iterator_name;
		array_pos += "]";

		ostr << indent
		     << "for (CORBA::ULong " << iterator_name << " = 0; "
		     << iterator_name << " < " << *i << "; "
		     << iterator_name << "++)" << endl;
		ostr << indent++ << "{" << endl;

		g_free (iterator_name);
	}

	ostr << indent << dest_id + array_pos << " = "
	     << src_id + array_pos << ";" << endl;

	for (; depth; depth--)
	{
		ostr << --indent << "}" << endl;
	}	
}

void
IDLArray::fill_c_array (ostream      &ostr,
			Indent       &indent,
			const string &cpp_id,
			const string &c_id) const
{
	unsigned int depth = 0;
	string array_pos;
	for (const_iterator i = begin (); i != end (); i++, depth++)
	{
		gchar *iterator_name = g_strdup_printf ("i_%d", depth);
		array_pos += "[";
		array_pos += iterator_name;
		array_pos += "]";

		ostr << indent
		     << "for (CORBA::ULong " << iterator_name << " = 0; "
		     << iterator_name << " < " << *i << "; "
		     << iterator_name << "++)" << endl;
		ostr << indent++ << "{" << endl;

		g_free (iterator_name);
	}

	string cpp_member = cpp_id + array_pos;
	string c_member = c_id + array_pos;
	m_element_type->member_pack_to_c (ostr, indent, cpp_member, c_member);

	for (; depth; depth--)
	{
		ostr << --indent << "}" << endl;
	}
}

void
IDLArray::fill_cpp_array (ostream      &ostr,
			  Indent       &indent,
			  const string &cpp_id,
			  const string &c_id) const
{
	unsigned int depth = 0;
	string array_pos;
	for (const_iterator i = begin (); i != end (); i++, depth++)
	{
		gchar *iterator_name = g_strdup_printf ("i_%d", depth);
		array_pos += "[";
		array_pos += iterator_name;
		array_pos += "]";

		ostr << indent
		     << "for (CORBA::ULong " << iterator_name << " = 0; "
		     << iterator_name << " < " << *i << "; "
		     << iterator_name << "++)" << endl;
		ostr << indent++ << "{" << endl;

		g_free (iterator_name);
	}

	string cpp_member = cpp_id + array_pos;
	string c_member = c_id + array_pos;
	m_element_type->member_unpack_from_c (ostr, indent, cpp_member, c_member);

	for (; depth; depth--)
	{
		ostr << --indent << "}" << endl;
	}
}

void
IDLArray::typedef_decl_write (ostream          &ostr,
			      Indent           &indent,
			      IDLCompilerState &state,
			      const IDLTypedef &target,
			      const IDLTypedef *) const
{
	string array_id = target.get_cpp_identifier ();
	string slice_id = array_id + "_slice";
	string slice_type = slice_id + "*";
	string c_slice_type = get_c_slice_type();

	string wrapper = m_element_type->is_fixed () ?
		IDL_IMPL_NS "::ArrayFixed" : IDL_IMPL_NS "::ArrayVariable";
	IDLScope* parent = target.getParentScope();
	
	int length = 1;
	for (const_iterator i = begin (); i != end (); i++)
		length *= *i;

	//////////////////////
	// Array types

	// Create array typedef
	ostr << indent << "typedef "
	     << m_element_type->get_cpp_member_typename () << " " << array_id;

	// Write dimensions
	for (const_iterator i = begin (); i != end (); i++)
		ostr << '[' << *i << ']';
	ostr << ';' << endl;
	

	// Create slice typedef
	ostr << indent << "typedef "
	     << m_element_type->get_cpp_member_typename () << " " << slice_id;

	// An array slice is all the dims except the first one
	const_iterator i = begin ();
	i++;
	for (; i != end (); i++)
		ostr << '[' << *i << ']';
	ostr << ';' << endl;
	
	//////////////////////
	// Array methods

	string static_prefix = target.interface_child() ? "static " : "";

	// Allocator
	ostr << indent << static_prefix << "inline "
	     << slice_type << " " << array_id << "_alloc ()" << endl
	     << indent++ << "{" << endl;
	
	// If no conversion is needed, use ORBit/C allocator.
	if (!conversion_required ())
	{
		#warning "Cast required because of possible bug in ORBit2"
		ostr << indent << slice_type << " array = (" << slice_type << ")"
		     << target.get_c_typename () << "__alloc ();" << endl;
	} else {
		ostr << indent << slice_type << " array = "
		     << "new " << slice_id << "[" << *begin () << "];" << endl;
	}
	
	init_cpp_array (ostr, indent, "array");
	ostr << indent << "return array;" << endl;
	ostr << --indent << "}" << endl << endl;
	

	// copy
	ostr << indent << static_prefix << "inline "
	     << "void " << array_id << "_copy ("
	     << slice_type << " dest, const " << slice_type << " source)" << endl
	     << indent++ << "{" << endl;

	copy_cpp_array (ostr, indent, "source", "dest");

	ostr << --indent << "}" << endl << endl;
	
	ostr << indent << static_prefix << "inline "
	     << "void " << array_id << "_pack ("
	     << c_slice_type << " dest, " << slice_type << " source)" << endl
	     << indent++ << "{" << endl;

	member_pack_to_c (ostr, indent, "source", "dest", &target);

	ostr << --indent << "}" << endl << endl;

	ostr << indent << static_prefix << "inline "
	     << "void " << array_id << "_unpack ("
	     << slice_type << " dest, " << c_slice_type << " source)" << endl


	     << indent++ << "{" << endl;

	member_unpack_from_c  (ostr, indent, "dest", "source", &target);

	ostr << --indent << "}" << endl << endl;

	// Dup
	ostr << indent << static_prefix << "inline "
	     << slice_type << " " << array_id << "_dup ("
	     << slice_type << " source)" << endl
	     << indent++ << "{" << endl;
	
	ostr << indent << slice_type << " ret = " << array_id << "_alloc ();" << endl;
	ostr << indent << array_id << "_copy (ret, source);" << endl;
	ostr << indent << "return ret;" << endl;
	
	ostr << --indent << "}" << endl << endl;
	

	// Free
	ostr << indent << static_prefix << "inline "
	     << "void " << array_id << "_free (" << slice_type << " val)" << endl
	     << indent++ << "{" << endl;
	if (!conversion_required ())
	{
		ostr << indent << "CORBA_free (val);" << endl;
	} else {
		ostr << indent << "delete[] val;" << endl;
	}
	ostr << --indent << "}" << endl << endl;

	// Props
	std::string props_id = get_props_id(target);
	std::string address = (parent->scope_implementation() == IDLScopeImplementation::by_type) ?
		"&" + parent->get_cpp_identifier() + "::" :
		"&";

	ostr << indent << "typedef " << IDL_IMPL_NS "::ArrayProperties< "
	     << slice_id << ", "
	     << get_c_slice_id() << ", "
	     << address << array_id << "_alloc" << ", "
	     << address << array_id << "_free" << ", "
	     << address << array_id << "_copy" << ", "
	     << address << array_id << "_pack" << ", "
	     << address << array_id << "_unpack" 
	     << "> " << props_id << ";" << endl;

	// Var
	ostr << indent << "typedef " << wrapper + "_var< " << slice_id << ", " << length << ", " << props_id << " > "
	     << array_id + "_var;" << endl;
	
	// Out
	if (m_element_type->is_fixed ())
	{
		ostr << indent << "typedef " << array_id << " "
		     << array_id + "_out;" << endl;
	} else {
		ostr << indent << "typedef "
		     << IDL_IMPL_NS "::ArrayVariable_out< " << slice_id << ", " << length << ", " << props_id << " > "
		     << array_id + "_out;" << endl;
	}

	// For_any
	ostr << indent << "typedef " << wrapper + "_forany< " << slice_id << ", " << length << " > "
	     << array_id + "_forany;" << endl;

		
	ostr << endl;
	

	ostr << endl;
	
	
	//////////////////////
	// Implementation specializations
	if (!state.m_arrays.array_exists (*this))
	{
                ORBITCPP_MEMCHECK (new IDLWriteArrayAnyFuncs (*this, target, state, *state.m_pass_xlate));
	}
}

string
IDLArray::stub_decl_arg_get (const string     &cpp_id,
			     IDL_param_attr    direction,
			     const IDLTypedef *) const
{
	string retval;

	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = "const " + get_cpp_typename () + 
			" " + cpp_id;	
		break;
	case IDL_PARAM_INOUT:
		retval = get_cpp_typename () + 
			" " + cpp_id;	
		break;
	case IDL_PARAM_OUT:
		retval = get_cpp_typename () + 
			"_out " + cpp_id;	
		break;
	}

	return retval;
}

void
IDLArray::stub_impl_arg_pre (ostream          &ostr,
			     Indent           &indent,
			     const string     &cpp_id,
			     IDL_param_attr    direction,
			     const IDLTypedef *) const
{
	if (!m_element_type->conversion_required ())
		return;

	string c_id = "_c_" + cpp_id;

	if (direction == IDL_PARAM_OUT && !m_element_type->is_fixed())
	{
		// Create slice pointer and do nothing with it
		ostr << indent << get_c_slice_type()
		     << " " << c_id << " = 0;" << endl;
		return;
	}
	
	// Create and fill C array
	ostr << indent << get_c_typename ()
	     << " " << c_id << ';' << endl;

	fill_c_array (ostr, indent, cpp_id, c_id);
	ostr << endl;
}
	
string
IDLArray::stub_impl_arg_call (const string   &cpp_id,
			      IDL_param_attr  direction,
			      const IDLTypedef *) const
{
	if (!m_element_type->conversion_required ())
		return cpp_id;

	string retval;

	switch (direction)
	{
	case IDL_PARAM_IN:
	case IDL_PARAM_INOUT:
		retval = "_c_" + cpp_id;
		break;
	case IDL_PARAM_OUT:
		if (m_element_type->is_fixed())
			retval = "_c_" + cpp_id;
		else
			retval = "&_c_" + cpp_id;
		break;
	}

	return retval;
}
	
void
IDLArray::stub_impl_arg_post (ostream          &ostr,
			      Indent           &indent,
			      const string     &cpp_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *) const
{
	if (!m_element_type->conversion_required ())
		return;
	
	string cpp_typename = get_cpp_typename ();

	// Allocate C++ array (if needed)
	if (!is_fixed ())
		ostr << indent << cpp_id << " = " << cpp_typename << "_alloc ();" << endl;
	
	// Re-load from C array
	if (direction == IDL_PARAM_INOUT || direction == IDL_PARAM_OUT)
	fill_cpp_array (ostr, indent, cpp_id, "_c_" + cpp_id);
	
	// Free C array
	if (direction == IDL_PARAM_OUT)
		ostr << indent << "CORBA_free (_c_" << cpp_id << ");"
		     << endl;
	else
		ostr << indent << get_c_typename () << "__freekids ("
		     << "_c_" << cpp_id << ", 0);" << endl;
		
}




string
IDLArray::stub_decl_ret_get (const IDLTypedef *) const
{
	return get_cpp_typename () + "_slice *";
}
	
void
IDLArray::stub_impl_ret_pre (ostream          &ostr,
			     Indent           &indent,
			     const IDLTypedef *) const
{
	// Do nothing
}

void
IDLArray::stub_impl_ret_call (ostream          &ostr,
			      Indent           &indent,
			      const string     &c_call_expression,
			      const IDLTypedef *) const
{
	ostr << indent << get_c_slice_type () << " _retval = "
	     << c_call_expression << ";" << endl;
}

void
IDLArray::stub_impl_ret_post (ostream          &ostr,
			      Indent           &indent,
			      const IDLTypedef *) const
{
	if (!m_element_type->conversion_required ())
	{
		ostr << indent << "return _retval;" << endl;
		return;
	}
	
	// Create and fill C++ array
	ostr << indent << get_cpp_typename ()
	     << "_slice *_cpp_retval = "
	     << get_cpp_typename () << "_alloc ();" << endl;

	fill_cpp_array (ostr, indent, "_cpp_retval", "_retval");
	
	ostr << indent << "CORBA_free (_retval);" << endl;

	ostr << indent << "return _cpp_retval;" << endl;
}
	



string
IDLArray::skel_decl_arg_get (const string     &c_id,
			     IDL_param_attr    direction,
			     const IDLTypedef *) const
{
	string retval;

	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = "const " + get_c_typename () + 
			" " + c_id;	
		break;
	case IDL_PARAM_INOUT:
		retval = get_c_typename () + 
			" " + c_id;	
		break;
	case IDL_PARAM_OUT:
		if (!m_element_type->is_fixed ())
			retval = get_c_slice_type () + 
				"* " + c_id;	
		else
			retval = get_c_typename () +
				" " + c_id;
		break;
	}

	return retval;
}

void
IDLArray::skel_impl_arg_pre (ostream          &ostr,
			     Indent           &indent,
			     const string     &c_id,
			     IDL_param_attr    direction,
			     const IDLTypedef *) const
{
	if (!m_element_type->conversion_required ())
		// Do nothing
		return;

	switch (direction)
	{
	case IDL_PARAM_IN:
	case IDL_PARAM_INOUT:
		ostr << indent << get_cpp_typename ()
		     << " _cpp_" << c_id << ";" << endl;
		fill_cpp_array (ostr, indent, "_cpp_" + c_id, c_id);
		break;

	case IDL_PARAM_OUT:
		ostr << indent << get_cpp_typename ()
		     << "_var _cpp_" << c_id << ";" << endl;
		break;
	}

	ostr << endl;
}
	
string
IDLArray::skel_impl_arg_call (const string     &c_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *) const
{
#warning "WRITE ME"

	if (!m_element_type->conversion_required ())
		return c_id;
	else
		return "_cpp_" + c_id;
}
	
void
IDLArray::skel_impl_arg_post (ostream          &ostr,
			      Indent           &indent,
			      const string     &c_id,
			      IDL_param_attr    direction,
			      const IDLTypedef *) const
{
	if (!m_element_type->conversion_required ())
		// Do nothing
		return;
	
	if (direction == IDL_PARAM_IN)
		// No need to reload IN parameters
		return;
		
	// Load C array from C++
	string cpp_root = "_cpp_" + c_id;
	string c_root = c_id;
	if (direction == IDL_PARAM_OUT && !m_element_type->is_fixed())
	{
		// We need to allocate OUT parameters ourselves
		c_root = "(*" + c_root + ")";
		ostr << indent << c_root << " = "
		     << get_c_typename () << "__alloc ()"
		     << ";" << endl;
	}

	fill_c_array (ostr, indent, cpp_root, c_root);

	ostr << endl;
}




string
IDLArray::skel_decl_ret_get (const IDLTypedef *) const
{
	return get_c_typename () + "_slice *";
}

void
IDLArray::skel_impl_ret_pre (ostream          &ostr,
			     Indent           &indent,
			     const IDLTypedef *) const
{
	ostr << indent << get_cpp_typename ()
	     << "_slice *_retval = 0;" << endl;
}

void
IDLArray::skel_impl_ret_call (ostream          &ostr,
			      Indent           &indent,
			      const string     &cpp_call_expression,
			      const IDLTypedef *) const
{
	ostr << indent << "_retval = " << cpp_call_expression << ";" << endl;
}

void
IDLArray::skel_impl_ret_post (ostream          &ostr,
			      Indent           &indent,
			      const IDLTypedef *) const
{
	if (!m_element_type->conversion_required ())
	{
		ostr << indent << "return _retval;" << endl;
		return;
	}

	// Create C return value and fill it
	ostr << indent << get_c_typename ()
	     << "_slice *_c_retval = "
	     << get_c_typename () << "__alloc ()"
	     << ";" << endl;

	fill_c_array (ostr, indent, "_retval", "_c_retval");

	ostr << indent << get_cpp_typename ()
	     << "_free (_retval);" << endl;
	
	ostr << indent << "return _c_retval;" << endl;
}


string
IDLArray::get_cpp_member_typename (const IDLTypedef *) const
{
	return get_cpp_typename ();
}

string
IDLArray::get_c_member_typename (const IDLTypedef *) const
{
	return get_c_typename ();
}

string
IDLArray::get_seq_typename (unsigned int      length,
			    const IDLTypedef * active_typedef) const
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

	std::string traits = "seq_traits_array";

	std::string cxx_value =
		(conversion_required()) ?
			cpp_member_typename + ", " :
			"";

	if (!active_typedef)
		g_error("Can't use anonymous array type in sequence.\nPlease typedef array and use alias in sequence declaration.\n");

	std::string props_id = get_props_id(*active_typedef);

	std::string c_slice_id = get_c_slice_id();
	
	if (length)
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleBoundedSeq< " IDL_IMPL_NS "::%s< %s, %s %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct, %s>, %d >",
				traits.c_str(),
				seq_typename.c_str (),
				cxx_value.c_str(),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str (),
				props_id.c_str(),
				length);
	else
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleUnboundedSeq< " IDL_IMPL_NS "::%s< %s, %s %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct, %s> >",
				traits.c_str(),
				seq_typename.c_str (),
				cxx_value.c_str(),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str (),
				c_member_typename_base.c_str (),
				props_id.c_str());
	
	retval = tmp;
	g_free (tmp);
	
	return retval;
}

string
IDLArray::member_decl_arg_get (const IDLTypedef *) const
{
	
	return "const " + get_cpp_typename () + " &";
}

void
IDLArray::member_impl_arg_copy (ostream          &ostr,
				Indent           &indent,
				const string     &cpp_id,
				const IDLTypedef *) const
{
	string copy_func = get_cpp_typename () + "_copy";
	string member_id = cpp_id;
	string param_id = "_par_" + member_id;
	
#ifdef IDL2CPP0X
	member_id.insert (0, "_");
#endif
	ostr << indent << copy_func << " (" << member_id << ", " << param_id
	     << ");" << endl;
}

void
IDLArray::member_init_c (ostream          &ostr,
			 Indent           &indent,
			 const string     &c_id,
			 const IDLTypedef *) const
{
#warning "WRITE ME"
}

void
IDLArray::member_pack_to_c (ostream          &ostr,
			    Indent           &indent,
			    const string     &cpp_id,
			    const string     &c_id,
			    const IDLTypedef *) const
{
	if (conversion_required ())
		fill_c_array (ostr, indent, cpp_id, c_id);
	else
		copy_cpp_array (ostr, indent, cpp_id, c_id);

}

void
IDLArray::member_unpack_from_c  (ostream          &ostr,
				 Indent           &indent,
				 const string     &cpp_id,
				 const string     &c_id,
				 const IDLTypedef *) const
{
	if (conversion_required ())
		fill_cpp_array (ostr, indent, cpp_id, c_id);
	else
		copy_cpp_array (ostr, indent, c_id, cpp_id);
}

void
IDLArray::create_union_accessors(IDLUnion const& un, IDLCaseStmt const& case_stmt, std::ostream& header, Indent indent, std::ostream& module, Indent mod_indent) const
{
	const IDLMember   &member = case_stmt.get_member ();
	string member_name = member.get_cpp_identifier ();
	string full_member_name =
		(un.is_fixed () ?
		 	"m_target._u." :
			"m_target->_u.") +
		member_name;
	string member_cpp_type = member.getType ()->get_cpp_member_typename ();
	string slice_id = member_cpp_type + "_slice";
	string slice_type = slice_id + "*";

	header << indent << slice_type << " "
		<< member_name << " () const;" << endl;

	module << mod_indent << slice_type << " "
		<< un.get_cpp_method_prefix () << "::" << member_name
		<< " () const" << endl
		<< mod_indent << "{" << endl;

	 module << ++mod_indent << member_cpp_type << " _ret;" << endl;
	 member_unpack_from_c (module, mod_indent, "_ret", full_member_name);

	 // FIXME: leaks memory
	 module << mod_indent << "return " << member_cpp_type << "_dup(_ret);" << endl;
	 module << --mod_indent << "}" << endl << endl;

	// setter
	 IDLStandardUnionable::create_union_setter(un, case_stmt, header, indent, module, mod_indent);
}
