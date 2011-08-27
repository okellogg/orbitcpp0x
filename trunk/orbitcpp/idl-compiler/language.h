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
 *  Purpose: IDL compiler language representation
 *
 */


#ifndef ORBITCPP_LANGUAGE
#define ORBITCPP_LANGUAGE

#include <libIDL/IDL.h>
#include "base.h"
#include <vector>
#include <list>
#include <map>
#include <set>


// forward --------------------------------------------------------------------
class IDLType;
class IDLScope;




string idlTranslateConstant(IDL_tree const constant);
IDL_tree simple_dcl_ident(IDL_tree dcl);




#include "types/IDLElement.h"
#include "types/IDLInhibited.h"
#include "types.h"

class IDLConstant :
	public IDLElement,
	public virtual IDLNotAType,
	public IDLIdentified {
protected:
	IDLType	*m_type;
public:
	IDLConstant(IDLType *type,string const &id,IDL_tree node,IDLScope *parentscope = NULL)
	:	IDLElement(id,node,parentscope),
		IDLIdentified (IDL_CONST_DCL(node).ident),
		m_type(type)
	{
	}

	string getValue() const {
		return idlTranslateConstant(IDL_CONST_DCL(getNode()).const_exp);
	}
	IDLType *getType() {
		return m_type;
	}
};




class IDLMember :
	public IDLElement,
	public virtual IDLNotAType,
	public IDLIdentified {
protected:
	IDLType	*m_type;
public:
	IDLMember(IDLType *type,string const &id,IDL_tree node,IDLScope *parentscope = NULL)
	:	IDLElement(id,node,parentscope),
		IDLIdentified (simple_dcl_ident(node)),
		m_type(type)
	{
	}
	IDLType const *getType() const {
		return m_type;
	}
};




class IDLEnumComponent :
	public IDLElement,
	public virtual IDLNotAType,
	public IDLIdentified {
public:
	IDLEnumComponent(string const &id,IDL_tree node,IDLScope *parentscope = NULL)
	:	IDLElement(id,node,parentscope),
		IDLIdentified (node)
	{
	}
	string getNSScopedCTypeName() const {
		return IDL_IMPL_C_NS_NOTUSED + get_c_typename ();
	}
};


class IDLCaseStmt;
class IDLException;



class IDLAttribute :
	public IDLInhibited<EmptyType>,
	public virtual IDLNotAType {

	IDL_tree attr_dcl_;
	IDL_tree simple_dcl_;

	IDLType 		*m_type;
  
public:

	IDLAttribute(IDL_tree attr_dcl, IDL_tree simple_dcl)
	:	IDLInhibited<EmptyType>(simple_dcl_ident(simple_dcl)),
		attr_dcl_ (attr_dcl),
		simple_dcl_ (simple_dcl)
	{
		std::string dummy;
		m_type = IDLTypeParser::parseDcl(simple_dcl, IDL_ATTR_DCL(attr_dcl_).param_type_spec, dummy);
	}

	IDLType *getType() {return m_type;}
	bool isReadOnly() {return IDL_ATTR_DCL(attr_dcl_).f_readonly;}
	
};


// An interface implemented by types that can be used as
// union discriminators
class IDLUnionDiscriminator {
public:
	// retns a default value, given a set of values used to
	// descriminate members of the union
	virtual string get_default_value (std::set<std::string> const &labels) const = 0;
	virtual string discr_get_c_typename () const = 0;
	virtual string discr_get_cpp_typename () const = 0;
};

#endif // ORBITCPP_LANGUAGE
