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
 *
 *  Purpose:	idl -> c++ translation pass
 *
 */

#ifndef ORBITCPP_PASS_XLATE
#define ORBITCPP_PASS_XLATE

#include "pass.h"
#include "pass_idl_it.h"
#include "types/IDLEnum.h"
#include "types/IDLStruct.h"
#include "types/IDLUnion.h"
#include "types/IDLException.h"
#include "types/IDLMember.h"
class IDLMethod;
#include <libIDL/IDL.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>


class IDLPassXlate : public IDLIteratingPass,public IDLOutputPass {
protected:
	typedef IDLIteratingPass		Super;

public:
	IDLPassXlate(IDLCompilerState &state,ostream &header,ostream &module)
		: IDLOutputPass(state,header,module) {
		state.m_pass_xlate = this;
	}

	void runPass();

protected:
	void doCodeFrag(IDL_tree node,IDLScope &scope);
	
	void doTypedef(IDL_tree node,IDLScope &scope);
	
	void doStruct(IDL_tree node,IDLScope &scope);
	void doUnion(IDL_tree node,IDLScope &scope);
	void doEnum(IDL_tree node,IDLScope &scope);
	void doNative(IDL_tree node,IDLScope &scope);
	void doConstant(IDL_tree node,IDLScope &scope);

	void doForwardDcl(IDLInterface &iface);
	void doForwardDcl(IDL_tree node, IDLScope &scope);
	void doInterface(IDL_tree node,IDLScope &scope);
	void doMember(IDLMember &member);
	void doAttribute(IDL_tree node,IDLScope &scope);
	void doOperation(IDL_tree node,IDLScope &scope);
	void doException(IDL_tree node,IDLScope &scope);

	void doModule(IDL_tree node,IDLScope &scope);

	void doInterfaceBase(IDLInterface &iface);
 
	void doInterfaceStaticMethodDeclarations(IDLInterface &iface);
	void doInterfaceStaticMethodDefinitions(IDLInterface &iface);

	void doAttributePrototype(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);
	void doAttributeStub(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);

	void doOperationPrototype(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);
	void doOperationStub(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);

	void doInterfaceDownCall(IDLInterface &iface,IDLInterfaceRight &of);

	void create_method_proto (const IDLMethod &method);
	void create_method_stub (IDLInterface &iface, const IDLMethod &method);
	void enumHook(IDL_tree list,IDLScope &scope);

	void iface_create_traits (const IDLInterface &iface);
	
	// for struct and union
	void struct_create_traits     (const IDLCompoundSeqElem &strct);
	void struct_create_members    (const IDLStruct &strct);
	void struct_create_converters (const IDLStruct &strct);
	void struct_create_typedefs   (const IDLStruct &strct);
	void struct_create_any        (const IDLStruct &strct);

	void union_create_internal    (const IDLUnion &un);
	void union_create_constructor (const IDLUnion &un);
	void union_create_copier      (const IDLUnion &un);
	void union_create_discr       (const IDLUnion &un);
	void union_create_members     (const IDLUnion &un);
	void union_create_converters  (const IDLUnion &un);
	void union_create_typedefs    (const IDLUnion &un);
	void union_create_any         (const IDLUnion &un);

	void exception_create_members      (const IDLException &ex);
	void exception_create_constructors (const IDLException &ex);
	void exception_create_converters   (const IDLException &ex);
	void exception_create_any          (const IDLException &ex);

        void element_write_typecode (const IDLElement &element);
};

class IDLWriteAnyFuncs : public IDLOutputPass::IDLOutputJob {
protected:
	IDLWriteAnyFuncs (IDLCompilerState &state,
			  IDLOutputPass    &pass);

	void writeAnyFuncs (bool          pass_value,
			    const string &cpptype,
			    const string &ctype);
public:
	enum FuncType {
		FUNC_VALUE,
		FUNC_COPY,
		FUNC_NOCOPY
	};
	
	static void writeInsertFunc (ostream      &ostr,
				     Indent       &indent,
				     FuncType      func,
				     string        ident,
				     const string &ctype);
	
	static void writeExtractFunc (ostream      &ostr,
				      Indent       &indent,
				      FuncType      func,
				      string        ident,
				      const string &ctype);
	
	void run() = 0;
};

class IDLWriteEnumAnyFuncs : public IDLWriteAnyFuncs
{
	IDLEnum m_enum;
public:
	IDLWriteEnumAnyFuncs (const IDLEnum    &_enum,
			      IDLCompilerState &state,
			      IDLOutputPass    &pass);
	void run();
};

template <class C>
class IDLWriteCompoundAnyFuncs : public IDLWriteAnyFuncs
{
	C m_element;
public:
	IDLWriteCompoundAnyFuncs (const C& c, IDLCompilerState &state, IDLOutputPass    &pass)
	: 	IDLWriteAnyFuncs (state, pass),
		m_element (c)
	{
	}

	void run()
	{
		writeAnyFuncs (false,
			       m_element.get_cpp_typename (),
			       m_element.get_c_typename ());
	}
};

typedef IDLWriteCompoundAnyFuncs<IDLStruct> IDLWriteStructAnyFuncs;
typedef IDLWriteCompoundAnyFuncs<IDLUnion> IDLWriteUnionAnyFuncs;

class IDLWriteExceptionAnyFuncs : public IDLWriteAnyFuncs
{
	IDLElement const & m_element;
public:
	IDLWriteExceptionAnyFuncs(IDLException const& _exception, IDLCompilerState &state, IDLOutputPass &pass)
		: IDLWriteAnyFuncs(state, pass), m_element(_exception) {}

	void run();
};

class IDLWriteIfaceAnyFuncs : public IDLWriteAnyFuncs
{
	IDLInterface const &m_iface;
public:
	IDLWriteIfaceAnyFuncs (const IDLInterface &iface,
			       IDLCompilerState   &state,
			       IDLOutputPass      &pass);
	void run();
};

class IDLWriteArrayAnyFuncs : public IDLWriteAnyFuncs
{
	IDLArray const &m_array;
	IDLElement const &m_dest;
public:
	IDLWriteArrayAnyFuncs (const IDLArray   &array,
			       const IDLElement &dest, 
			       IDLCompilerState &state,
			       IDLOutputPass    &pass);
	void run();
};

#endif
