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




#include <cstdio>
#include <cstring>
#include "language.h"
#include "error.h"
#include "types.h"
#include <algorithm>
#include <set>

IDL_tree
simple_dcl_ident(IDL_tree dcl)
{
	if (IDL_NODE_TYPE(dcl) == IDLN_IDENT)
		return dcl;

	if (IDL_NODE_TYPE(dcl) == IDLN_TYPE_ARRAY)
		return IDL_TYPE_ARRAY(dcl).ident;
	
	std::cerr << IDL_NODE_TYPE_NAME(dcl) << '\n';
	g_assert_not_reached();
}



// constant xlator ------------------------------------------------------------
static const char *idlNumChars = "0123456789abcdef";




static string idlInt2String(IDL_longlong_t value,char radix = 10)
{
	bool neg = value < 0;
	if (neg) value = -value;
	string temp;
	do {
		temp = idlNumChars[value % radix]+temp;
		value /= radix;
	} while (value);
	if (neg) temp = '-'+temp;
	return temp;
}




string idlTranslateConstant(IDL_tree const constant) {
	char buffer[1<<8];
	IDLElement *cns;
	switch (IDL_NODE_TYPE(constant)) {
		case IDLN_INTEGER:
			return idlInt2String(IDL_INTEGER(constant).value);
		case IDLN_STRING:
			return string("\"")+IDL_STRING(constant).value+'"';
		case IDLN_WIDE_STRING:
			// *** FIXME implement this
			ORBITCPP_NYI("wide string constant");
			return "";
		case IDLN_CHAR:
			return '\''+string(IDL_CHAR(constant).value,1)+'\'';
		case IDLN_WIDE_CHAR:
			// *** FIXME implement this
			ORBITCPP_NYI("wide char constant")
			return "";
		case IDLN_FIXED:
			// *** FIXME implement this
			ORBITCPP_NYI("fixed constant")
			return "";
		case IDLN_FLOAT:
			sprintf(buffer,"%f",IDL_FLOAT(constant).value);
			return buffer;
		case IDLN_BOOLEAN:
			if (IDL_BOOLEAN(constant).value) return "1";
			else return "0";
		case IDLN_IDENT:
			return IDL_IDENT(constant).str;
		case IDLN_UNARYOP: {
			char op = ' ';
			switch (IDL_UNARYOP(constant).op) {
				case IDL_UNARYOP_PLUS: op = '+'; break;
				case IDL_UNARYOP_MINUS: op = '-'; break;
				case IDL_UNARYOP_COMPLEMENT: op = '~'; break;
			}
			return string("(") + op + idlTranslateConstant(IDL_UNARYOP(constant).operand) + ")";
		}
		case IDLN_BINOP: {
			string op;
			switch (IDL_BINOP(constant).op) {
				case IDL_BINOP_OR: op = "|"; break;
				case IDL_BINOP_XOR: op = "^"; break;
				case IDL_BINOP_AND: op = "&"; break;
				case IDL_BINOP_SHR: op = ">>"; break;
				case IDL_BINOP_SHL: op = "<<"; break;
				case IDL_BINOP_ADD: op = "+"; break;
				case IDL_BINOP_SUB: op = "-"; break;
				case IDL_BINOP_MULT: op = "*"; break;
				case IDL_BINOP_DIV: op = "/"; break;
				case IDL_BINOP_MOD: op = "%"; 
			}
			return '(' + idlTranslateConstant(IDL_BINOP(constant).left) +
				op + idlTranslateConstant(IDL_BINOP(constant).right) + ')';
		}
		default:
			ORBITCPP_NYI("parsing "+idlGetNodeTypeString(constant)+" as a constant")
	}
}


