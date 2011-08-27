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
 *          Phil Dawes <philipd@users.sourceforge.net>
 *			John K. Luebs <jkluebs@marikarpress.com>
 *
 *  Purpose: IDL compiler type representation
 *
 */




#include "types.h"
#include "pass_xlate.h"
#include "orbitcpp/idl-compiler/error.h"
#include "orbitcpp/idl-compiler/language.h"
#include "orbitcpp/idl-compiler/types/IDLType.h"
#include "orbitcpp/idl-compiler/types/IDLInhibited.h"
#include "orbitcpp/idl-compiler/types/IDLAny.h"
#include "orbitcpp/idl-compiler/types/IDLArray.h"
#include "orbitcpp/idl-compiler/types/IDLVoid.h"
#include "orbitcpp/idl-compiler/types/IDLString.h"
#include "orbitcpp/idl-compiler/types/IDLBoolean.h"
#include "orbitcpp/idl-compiler/types/IDLSequence.h"
#include "orbitcpp/idl-compiler/types/IDLObject.h"
#include "orbitcpp/idl-compiler/types/IDLTypeCode.h"
#include "orbitcpp/idl-compiler/types/IDLStruct.h"
#include "orbitcpp/idl-compiler/types/IDLEnum.h"
#include "orbitcpp/idl-compiler/types/IDLUnion.h"
#include "orbitcpp/idl-compiler/types/IDLException.h"
#include "orbitcpp/idl-compiler/types/IDLModule.h"
#include "orbitcpp/idl-compiler/types/IDLTypedef.h"

static IDLVoid idlVoid;
#ifdef IDL2CPP0X
static IDLString idlString ("CORBA_char", "string", "std::string");
static IDLString idlWString ("CORBA_wchar", "wstring", "std::wstring");
#else
static IDLString idlString ("CORBA_char", "string", "String");
static IDLString idlWString ("CORBA_wchar", "wstring", "WString");
#endif

#define ORBITCPP_MAKE_SIMPLE_TYPE(name,cname)			\
	static class IDL##name : public IDLSimpleType, public IDLTypenameUnused { \
	protected:						\
		string get_cpp_identifier () const {		\
			return #name;				\
		} 						\
		string get_cpp_typename () const {		\
			return IDL_CORBA_NS "::" #name;		\
		} 						\
		string get_c_typename () const {		\
			return #cname;				\
		}						\
	public:							\
		IDL##name () : IDLType (IDLType::T_FLOAT) {}	\
	} idl##name;


#define ORBITCPP_MAKE_SIMPLE_INT_TYPE(name,cname) \
	static class IDL##name : public IDLSimpleType, public IDLUnionDiscriminator, public IDLTypenameUnused {	\
	protected:							\
		string get_cpp_identifier () const {			\
			return #name;					\
		} 							\
		string get_cpp_typename () const {			\
			return IDL_CORBA_NS "::" #name;			\
		} 							\
		string get_c_typename () const {			\
			return #cname;					\
		}							\
	public:								\
		IDL##name () : IDLType (IDLType::T_INTEGER) {}		\
		std::string get_default_value (std::set<std::string> const &labels) const { \
			short val = 0;					\
			char *val_buf = 0;				\
			std::string val_str;	       			\
		  	do {						\
				val_buf = g_strdup_printf ("%d", val++);\
				val_str = val_buf;			\
				g_free (val_buf);			\
		  	} while (labels.find (val_str) != labels.end ());\
			return val_str; 				\
		}							\
									\
		string discr_get_c_typename () const {			\
			return get_fixed_c_typename ();			\
		}							\
									\
		string discr_get_cpp_typename () const {		\
			return get_fixed_cpp_typename ();		\
		}							\
	} idl##name;

#define ORBITCPP_MAKE_SIMPLE_CHAR_TYPE(name,cname)					\
	static class IDL##name : public IDLSimpleType, public IDLUnionDiscriminator, public IDLTypenameUnused {	\
	protected:									\
		std::string get_cpp_identifier () const {				\
			return #name;							\
		}									\
		std::string get_cpp_typename () const {					\
			return IDL_CORBA_NS "::" #name;					\
		}									\
		std::string get_c_typename () const {		       			\
			return #cname;							\
		}									\
	public:										\
		IDL##name () : IDLType (IDLType::T_CHAR) {}				\
		std::string get_default_value (std::set<std::string> const &labels) const { \
			return "\'\\0\'"; 						\
		}									\
											\
		std::string discr_get_c_typename () const {				\
			return get_fixed_c_typename ();					\
		}									\
											\
		std::string discr_get_cpp_typename () const {				\
			return get_fixed_cpp_typename ();				\
		}									\
	} idl##name;


static IDLBoolean idlBoolean;


ORBITCPP_MAKE_SIMPLE_CHAR_TYPE(Char, CORBA_char)
ORBITCPP_MAKE_SIMPLE_CHAR_TYPE(WChar, CORBA_wchar)
ORBITCPP_MAKE_SIMPLE_TYPE(Octet, CORBA_octet)

ORBITCPP_MAKE_SIMPLE_INT_TYPE(Short, CORBA_short)
ORBITCPP_MAKE_SIMPLE_INT_TYPE(UShort, CORBA_unsigned_short)
ORBITCPP_MAKE_SIMPLE_INT_TYPE(Long, CORBA_long)
ORBITCPP_MAKE_SIMPLE_INT_TYPE(ULong, CORBA_unsigned_long)
ORBITCPP_MAKE_SIMPLE_INT_TYPE(LongLong, CORBA_long_long)
ORBITCPP_MAKE_SIMPLE_INT_TYPE(ULongLong, CORBA_unsigned_long_long)

ORBITCPP_MAKE_SIMPLE_TYPE(Float, CORBA_float)
ORBITCPP_MAKE_SIMPLE_TYPE(Double, CORBA_double)
ORBITCPP_MAKE_SIMPLE_TYPE(LongDouble, CORBA_long_double)


static IDLAny idlAny;
static IDLObject idlObject;
static IDLTypeCode idlTypeCode;



// IDLTypeParser -------------------------------------------------------------

IDLType *
IDLTypeParser::parseTypeSpec(IDLScope* scope,IDL_tree typespec)
{
	IDLType* type = dynamic_cast<IDLType*>(parse(scope, typespec));
	g_assert(type != 0);
	return type;
}

IDLScope *
IDLTypeParser::parseScope(IDLScope* parent,IDL_tree spec)
{
	IDLScope* scope = dynamic_cast<IDLScope*>(parse(parent, spec));
	g_assert(scope != 0);
	return scope;
}

IDLTypeDiscrim *
IDLTypeParser::parse(IDLScope* scope,IDL_tree typespec)
{

	// I'm starting to think that this should be 2 methods:
	//  - one to parse the type, and one to parse the dcl into an id - PD
	// I noticed the same making some mods to the gather pass - JKL
	
	IDLTypeDiscrim *type = NULL;

	if (typespec == NULL) type = &idlVoid;
	else
switch_typespec:
		switch (IDL_NODE_TYPE(typespec)) {
		case IDLN_TYPE_BOOLEAN:
			type = &idlBoolean;
			break;
		case IDLN_TYPE_CHAR:
			type = &idlChar;
			break;
		case IDLN_TYPE_WIDE_CHAR:
			type = &idlWChar;
			break;
		case IDLN_TYPE_OCTET:
			type = &idlOctet;
			break;
		case IDLN_TYPE_STRING:
			type = &idlString;
			break;
		case IDLN_TYPE_WIDE_STRING:
			type = &idlWString;
			break;

		case IDLN_TYPE_ANY:
			type = &idlAny;
			break;

		case IDLN_TYPE_OBJECT:
			type = &idlObject;
			break;
		case IDLN_TYPE_TYPECODE:
			type = &idlTypeCode;
			break;

		case IDLN_INTERFACE:
			type = new IDLInterface(typespec);
			break;

		case IDLN_TYPE_STRUCT:
			type = new IDLStruct(typespec);
			break;

		case IDLN_TYPE_ENUM:
			type = new IDLEnum(typespec);
			break;

		case IDLN_TYPE_UNION:
			type = new IDLUnion(typespec);
			break;
				
		case IDLN_TYPE_INTEGER:
			if (IDL_TYPE_INTEGER(typespec).f_signed) {
				switch (IDL_TYPE_INTEGER(typespec).f_type) {
				case IDL_INTEGER_TYPE_SHORT:
					type = &idlShort;
					break;
				case IDL_INTEGER_TYPE_LONG:
					type = &idlLong;
					break;
				case IDL_INTEGER_TYPE_LONGLONG:
					type = &idlLongLong;
					break;
				}
			} else {
				switch (IDL_TYPE_INTEGER(typespec).f_type) {
				case IDL_INTEGER_TYPE_SHORT:
					type = &idlUShort;
					break;
				case IDL_INTEGER_TYPE_LONG:
					type = &idlULong;
					break;
				case IDL_INTEGER_TYPE_LONGLONG:
					type = &idlULongLong;
					break;
				}
			}
			break;

		case IDLN_TYPE_FLOAT:
			switch (IDL_TYPE_FLOAT(typespec).f_type) {
			case IDL_FLOAT_TYPE_FLOAT:
				type = &idlFloat;
				break;
			case IDL_FLOAT_TYPE_DOUBLE:
				type = &idlDouble;
				break;
			case IDL_FLOAT_TYPE_LONGDOUBLE:
				type = &idlLongDouble;
				break;
			}
			break;

		case IDLN_IDENT:
			{
				IDLElement *item = 0;
				if (scope != 0)
					item = scope->lookup(idlGetQualIdentifier(typespec));
				// assume item is in an inhibit block
				if (!item) {
					IDL_tree up = IDL_NODE_UP(typespec);
					g_assert(up != 0);
switch_up:
					switch (IDL_NODE_TYPE(up)) {

					case IDLN_IDENT:
						up = IDL_NODE_UP(up);
						goto switch_up;

					case IDLN_LIST:
						up = IDL_NODE_UP(up);
						goto switch_up;

					case IDLN_TYPE_DCL:
						type = new IDLTypedef(typespec);
						break;


					case IDLN_TYPE_ENUM:
					case IDLN_TYPE_UNION:
					case IDLN_TYPE_STRUCT:
					case IDLN_INTERFACE:
					case IDLN_TYPE_BOOLEAN:
					case IDLN_TYPE_CHAR:
					case IDLN_TYPE_WIDE_CHAR:
					case IDLN_TYPE_OCTET:
					case IDLN_TYPE_STRING:
					case IDLN_TYPE_ANY:
					case IDLN_TYPE_OBJECT:
					case IDLN_TYPE_TYPECODE:
					case IDLN_TYPE_INTEGER:
					case IDLN_TYPE_FLOAT:
					case IDLN_TYPE_SEQUENCE:
					case IDLN_TYPE_ARRAY:
						typespec = up;
						goto switch_typespec;

					ORBITCPP_DEFAULT_CASE(up)
					}

					break;
				}

				if (!item->isType())
					throw IDLExTypeIdentifierExpected(typespec,IDL_IDENT(typespec).str);
				
				type = dynamic_cast<IDLType *>(item);
				break;
			}
		case IDLN_TYPE_SEQUENCE:
			{
				// parse the sequence element type
				IDLType *type_seq = parseTypeSpec(scope,IDL_TYPE_SEQUENCE(typespec).simple_type_spec);
				IDLSequence* seq = 0;
				if (IDL_TYPE_SEQUENCE(typespec).positive_int_const == NULL){
					seq = new IDLSequence(*type_seq,0);
				} else {
					string len_str =
						idlTranslateConstant(IDL_TYPE_SEQUENCE(typespec).positive_int_const);
					int length = atoi(len_str.c_str());
					seq = new IDLSequence(*type_seq,length);
				}
				type = seq;
				break;
			}
		case IDLN_TYPE_ARRAY:
		    {
		    		IDL_tree up = IDL_NODE_UP(typespec);
				IDL_tree up2 = IDL_NODE_UP(up);
		    		type = new IDLArray(IDL_TYPE_DCL(up2).type_spec, typespec);
				break;
			}
		case IDLN_MODULE:
			type = new IDLModule(typespec);
			break;

		ORBITCPP_DEFAULT_CASE(typespec)
		}

	if (!type) ORBITCPP_NYI(idlGetNodeTypeString(typespec))

	return type;
}

IDLType*
IDLTypeParser::parseDcl(IDL_tree dcl, IDL_tree typespec, string &id)
{
	IDLType *ret_type = 0;
	
	if (IDL_NODE_TYPE(dcl) == IDLN_IDENT){
		g_assert(dcl != typespec);
		ret_type = IDLTypeParser::parseTypeSpec(0, typespec);
		id = IDL_IDENT(dcl).str;

	} else if (IDL_NODE_TYPE(dcl) == IDLN_TYPE_ARRAY) {
		ret_type = new IDLArray(typespec, dcl);
		id = IDL_IDENT(IDL_TYPE_ARRAY(dcl).ident).str;

	} else 
		ORBITCPP_NYI(" declarators:"+idlGetNodeTypeString(dcl));

	return ret_type;
}

