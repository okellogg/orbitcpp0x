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

#include "IDLSequence.h"

#include "IDLTypedef.h"
#include "IDLString.h"

IDLSequence::IDLSequence (const IDLType &element_type,
			  unsigned int   length) :
	IDLType (IDLType::T_SEQUENCE),
	m_element_type (element_type),
	m_length (length)
{
}

bool
IDLSequence::is_fixed () const
{
	return false;
}

void
IDLSequence::const_decl_write (ostream          &ostr, ostream&, IDLScope const&,
			  Indent           &indent,
			  const string     &cpp_id,
			  const string     &value,
			  const IDLTypedef *active_typedef) const
{
#warning "WRITE ME"
}

void
IDLSequence::typedef_decl_write (ostream          &ostr,
				 Indent           &indent,
				 IDLCompilerState &state,
				 const IDLTypedef &target,
				 const IDLTypedef *active_typedef) const
{
	string cpp_typedef = target.get_cpp_identifier ();
	string cpp_type = m_element_type.get_seq_typename (m_length);
	
	ostr << indent << "typedef " << cpp_type << " " << cpp_typedef
	     << ";" << endl;
	
#ifdef IDL2CPP0X
	/* General remark:
	 * Conversion of sequences to C++0x is work in progress.
	 * Currently, only sequence<string> is converted to C++0x.
	 * This code is hacky right now - once all
	 * sequence classes are converted to C++0x it will be cleaned up.
	 */
	if (cpp_type.find ("StringUnboundedSeq") != std::string::npos)
	{
		ostr << indent << "typedef " << cpp_typedef << "& "
		     << cpp_typedef << "_out;" << endl;
	}
	else
	{
		/* Create smart pointers.
		   This code will be changed soon - see remark above.  */
		ostr << indent << "typedef " << IDL_IMPL_NS << "::Sequence_var<"
		     << cpp_typedef << "> " << cpp_typedef << "_var;" << endl;
		ostr << indent << "typedef " << IDL_IMPL_NS << "::Sequence_out<"
		     << cpp_typedef << "> " << cpp_typedef << "_out;" << endl;
	}
#else
	// Create smart pointers
	ostr << indent << "typedef " << IDL_IMPL_NS << "::Sequence_var<" << cpp_typedef << "> "
	     << cpp_typedef << "_var;" << endl;
	ostr << indent << "typedef " << IDL_IMPL_NS << "::Sequence_out<" << cpp_typedef << "> "
	     << cpp_typedef << "_out;" << endl;
#endif
}

string
IDLSequence::stub_decl_arg_get (const string     &cpp_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	string retval;
	string cpp_type = get_cpp_member_typename (active_typedef);
		
	
	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = "const " + cpp_type + " &" + cpp_id;
		break;
	case IDL_PARAM_INOUT:
		retval = cpp_type + " &" + cpp_id;
		break;
	case IDL_PARAM_OUT:
		retval = cpp_type + "::_out_type " + cpp_id;
		break;
	}

	return retval;
}

void
IDLSequence::stub_impl_arg_pre (ostream          &ostr,
				Indent           &indent,
				const string     &cpp_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	string c_id = "_c_" + cpp_id;
	
	ostr << indent << get_c_member_typename() << ' ' << c_id << ";" << endl;

	if (direction == IDL_PARAM_IN || direction == IDL_PARAM_INOUT)
		ostr << indent << c_id << " = " << cpp_id << "._orbitcpp_pack ();" << endl;
		
}
	
string
IDLSequence::stub_impl_arg_call (const string     &cpp_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef) const
{
	if (direction == IDL_PARAM_OUT)
		return "&_c_" + cpp_id;
	
	return "_c_" + cpp_id;
}
	
void
IDLSequence::stub_impl_arg_post (ostream          &ostr,
				 Indent           &indent,
				 const string     &cpp_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef) const
{
	if (direction == IDL_PARAM_INOUT)
	{
		// Load back values
		ostr << indent << cpp_id << "._orbitcpp_unpack ("
		     << "*_c_" << cpp_id << ");" << endl;
	}

	if (direction == IDL_PARAM_OUT)
	{
		string ref = "->";
#ifdef IDL2CPP0X
		// This is hacky. See "general remark" above.
		string cpp_type = m_element_type.get_seq_typename (m_length);
		if (cpp_type.find ("StringUnboundedSeq") != std::string::npos)
			ref = ".";
#else
		string cpp_type = get_cpp_member_typename(active_typedef);
		ostr << indent << cpp_id << " = new " << cpp_type << ";" << endl;
#endif
		ostr << indent << cpp_id << ref << "_orbitcpp_unpack ("
		     << "*_c_" << cpp_id << ");" << endl;
	}
	
	ostr << indent << "CORBA_free (_c_" << cpp_id << ");" << endl;
}




string
IDLSequence::stub_decl_ret_get (const IDLTypedef *active_typedef) const
{
#ifdef IDL2CPP0X
	return get_cpp_member_typename(active_typedef);
#else
	return get_cpp_member_typename(active_typedef) + "*";
#endif
}
	
void
IDLSequence::stub_impl_ret_pre (ostream &ostr,
				Indent  &indent,
				const IDLTypedef *active_typedef) const
{
	// Do nothing
}

void
IDLSequence::stub_impl_ret_call (ostream          &ostr,
				 Indent           &indent,
				 const string     &c_call_expression,
				 const IDLTypedef *active_typedef) const
{
	ostr << indent << get_c_member_typename(active_typedef) << " _c_retval"
	     << " = " << c_call_expression << ";" << endl;
}

void
IDLSequence::stub_impl_ret_post (ostream          &ostr,
				 Indent           &indent,
				 const IDLTypedef *active_typedef) const
{
	string cpp_type = get_cpp_member_typename(active_typedef);
	
	ostr << indent << cpp_type;
#ifdef IDL2CPP0X
	ostr << " _cpp_retval;" << endl;
	ostr << indent << "_cpp_retval.";
#else
	ostr << " *_cpp_retval = new " << cpp_type << ";" << endl;
	ostr << indent << "_cpp_retval->";
#endif
	ostr << "_orbitcpp_unpack (*_c_retval);" << endl;
	ostr << indent << "CORBA_free (_c_retval);" << endl << endl;
	ostr << indent << "return _cpp_retval;" << endl;
}
	



string
IDLSequence::skel_decl_arg_get (const string     &c_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	string retval;
	string c_type = get_c_member_typename_base(active_typedef);
	
	switch (direction)
	{
	case IDL_PARAM_IN:
		retval = "const " + c_type + " *" + c_id;
		break;
	case IDL_PARAM_INOUT:
		retval = c_type + " *" + c_id;
		break;
	case IDL_PARAM_OUT:
		retval = c_type + " **" + c_id;
		break;
	}

	return retval;	
}

void
IDLSequence::skel_impl_arg_pre (ostream          &ostr,
				Indent           &indent,
				const string     &c_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef) const
{
	string cpp_id = "_cpp_" + c_id;
	string cpp_type = get_cpp_member_typename(active_typedef);
	string elem_type = m_element_type.get_c_member_typename();

	switch (direction)
	{
	case IDL_PARAM_IN:
	case IDL_PARAM_INOUT:
		ostr << indent << cpp_type << " " << cpp_id << ";" << endl;
		ostr << indent << cpp_id << "._orbitcpp_unpack (*" << c_id << ");" << endl;
		break;
	case IDL_PARAM_OUT:
		ostr << indent << cpp_type;
#ifdef IDL2CPP0X
		if (elem_type != "CORBA_string")
			ostr << "::_var_type";
#endif
		ostr << " " << cpp_id << ";" << endl;
		break;
	}
}
	
string
IDLSequence::skel_impl_arg_call (const string     &c_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef) const
{
	return "_cpp_" + c_id;
}
	
void
IDLSequence::skel_impl_arg_post (ostream          &ostr,
				 Indent           &indent,
				 const string     &c_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef) const
{
	if (direction == IDL_PARAM_INOUT)
		ostr << indent << "_cpp_" << c_id << "._orbitcpp_pack"
		     << " (*" << c_id << ");" << endl;

	if (direction == IDL_PARAM_OUT)
	{
		string ref = "->";
#ifdef IDL2CPP0X
		string cpp_type = m_element_type.get_seq_typename (m_length);
		// This is hacky. See "general remark" above.
		if (cpp_type.find ("StringUnboundedSeq") != std::string::npos)
			ref = ".";
#endif
		ostr << indent << "*" << c_id << " = "
		     << "_cpp_" << c_id << ref << "_orbitcpp_pack ();" << endl;
	}
}




string
IDLSequence::skel_decl_ret_get (const IDLTypedef *active_typedef) const
{
	return get_c_member_typename(active_typedef);
}

void
IDLSequence::skel_impl_ret_pre (ostream          &ostr,
				Indent           &indent,
				const IDLTypedef *active_typedef) const
{
	ostr << indent << get_cpp_member_typename(active_typedef);
#ifndef IDL2CPP0X
	ostr << "::_var_type";
#endif
	ostr << " _cpp_retval;" << endl;
}

void
IDLSequence::skel_impl_ret_call (ostream          &ostr,
				 Indent           &indent,
				 const string     &cpp_call_expression,
				 const IDLTypedef *active_typedef) const
{
	ostr << indent << "_cpp_retval = " << cpp_call_expression << ";" << endl;
}

void
IDLSequence::skel_impl_ret_post (ostream          &ostr,
				 Indent           &indent,
				 const IDLTypedef *active_typedef) const
{
	string ref = ".";
#ifdef IDL2CPP0X
	string cpp_type = m_element_type.get_seq_typename (m_length);
	// ostr << "// cpp_type : " << cpp_type << endl;
	// This is hacky. See "general remark" above.
	if (cpp_type.find ("StringUnboundedSeq") != std::string::npos)
		ref = ".";
#endif
	ostr << indent << "return _cpp_retval" << ref << "_orbitcpp_pack ();"
	     << endl << endl;
}


string
IDLSequence::get_cpp_member_typename (const IDLTypedef *active_typedef) const
{
	if (active_typedef)
		return active_typedef->get_cpp_typename ();

	const string seq_typename = m_element_type.get_seq_typename (m_length);
	return seq_typename;
}

string
IDLSequence::get_c_member_typename_base (const IDLTypedef *active_typedef) const
{
	if (active_typedef != 0) 
		return active_typedef->get_c_typename();

	std::string base = "CORBA_sequence_" + m_element_type.get_c_member_typename();
	// deal with sequence of sequence
	std::string::size_type pos = base.find('*');
	if (pos != std::string::npos)
		base.replace(pos, 1, "");

	return base;
}

string
IDLSequence::get_c_member_typename (const IDLTypedef *active_typedef) const
{

	return get_c_member_typename_base() + "*";
}

string
IDLSequence::get_seq_typename (unsigned int      length,
			       const IDLTypedef *active_typedef) const
{
	string retval;

	char *tmp = 0;
	std::string seq_typename = m_element_type.get_seq_typename (m_length);
	std::string c_member_typename_base = get_c_member_typename_base (active_typedef);
	std::string cpp_member_typename = get_cpp_member_typename (active_typedef);

#ifdef IDL2CPP0X
	if (c_member_typename_base == "CORBA_string")
		tmp = g_strdup (IDL_IMPL_NS "::StringUnboundedSeq");
	else
#endif
	if (length)
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleBoundedSeq< " IDL_IMPL_NS "::seq_traits< %s, %s, %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct>, %d >",
				       seq_typename.c_str (),
				       cpp_member_typename.c_str(),
				       c_member_typename_base.c_str(),
				       c_member_typename_base.c_str(),
				       c_member_typename_base.c_str(),
				       length);
	else
		tmp = g_strdup_printf (
			IDL_IMPL_NS "::SimpleUnboundedSeq< " IDL_IMPL_NS "::seq_traits< %s, %s, %s, CORBA_sequence_%s, &TC_CORBA_sequence_%s_struct> >",
				       seq_typename.c_str (),
				       cpp_member_typename.c_str(),
				       c_member_typename_base.c_str (),
				       c_member_typename_base.c_str (),
				       c_member_typename_base.c_str());
	
	retval = tmp;
	g_free (tmp);
	
	return retval;
}
	
string
IDLSequence::member_decl_arg_get (const IDLTypedef *active_typedef) const
{
	return get_cpp_member_typename(active_typedef) + " const&";
}

void
IDLSequence::member_impl_arg_copy (ostream          &ostr,
			      Indent           &indent,
			      const string     &cpp_id,
			      const IDLTypedef *active_typedef) const
{
	ostr << indent;
#ifdef IDL2CPP0X
	ostr << "_";
#endif
	ostr << cpp_id << " = _par_" << cpp_id << ';' << endl;
}

void
IDLSequence::member_init_c (ostream          &ostr,
			    Indent           &indent,
			    const string     &c_id,
			    const IDLTypedef *active_typedef) const
{
#warning "WRITE ME"
}

void
IDLSequence::member_pack_to_c (ostream          &ostr,
			       Indent           &indent,
			       const string     &cpp_id,
			       const string     &c_id,
			       const IDLTypedef *active_typedef) const
{
	ostr << indent << cpp_id << "._orbitcpp_pack (" << c_id << ");" << endl;
}

void
IDLSequence::member_unpack_from_c  (ostream          &ostr,
				    Indent           &indent,
				    const string     &cpp_id,
				    const string     &c_id,
				    const IDLTypedef *active_typedef) const
{
	ostr << indent << cpp_id << "._orbitcpp_unpack (" << c_id << ");" << endl;
}
