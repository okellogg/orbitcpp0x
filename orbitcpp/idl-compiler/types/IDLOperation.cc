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
 *  Purpose:	IDL compiler language representation
 *
 */

#include "IDLOperation.h"
#include "orbitcpp/idl-compiler/types/IDLException.h"
#include "orbitcpp/idl-compiler/types.h"


IDLOperation::IDLOperation (
			    IDL_tree      node,
			    IDLInterfaceRight&  parent):
	IDLInhibited<EmptyType> (IDL_OP_DCL(node).ident),
	IDLMethod (parent)
{
	std::string dummy;
	m_returntype = IDLTypeParser::parseDcl(IDL_OP_DCL(node).ident, IDL_OP_DCL(node).op_type_spec, dummy);

	IDL_tree parlist = IDL_OP_DCL(node).parameter_dcls;

	for (; parlist != 0; parlist = IDL_LIST(parlist).next) {
		ParameterInfo pi;
		pi.direction = IDL_PARAM_DCL(IDL_LIST(parlist).data).attr;
		IDL_tree d = IDL_LIST(parlist).data;
		pi.type = IDLTypeParser::parseDcl(
			IDL_PARAM_DCL(d).simple_declarator,
			IDL_PARAM_DCL(d).param_type_spec, pi.id);
		m_parameterinfo.push_back(pi);
	}

	IDL_tree raises_list = IDL_OP_DCL(node).raises_expr;
	for (; raises_list != 0; raises_list = IDL_LIST(raises_list).next) {
		IDL_tree exception = IDL_LIST(raises_list).data;
		if (IDL_NODE_TYPE(exception) == IDLN_EXCEPT_DCL)
			exception = IDL_EXCEPT_DCL(exception).ident;
		m_raises.push_back(new IDLExceptionInhibited(exception));
	}
}

string
IDLOperation::get_cpp_methodname () const
{
	return get_cpp_identifier ();
}

string
IDLOperation::get_c_methodname () const
{
	return get_c_typename ();
}
