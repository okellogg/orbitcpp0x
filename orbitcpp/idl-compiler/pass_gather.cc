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
 *  Purpose: information gathering pass
 *
 */




#include "error.h"
#include "pass_gather.h"

#include "types/IDLCaseStmt.h"
#include "types/IDLEnum.h"
#include "types/IDLException.h"
#include "types/IDLOperation.h"
#include "types/IDLStruct.h"
#include "types/IDLTypedef.h"
#include "types/IDLUnion.h"
#include "types/IDLAttribute.h"
#include "types/IDLConstant.h"
#include "types/IDLMember.h"
#include <map>


// IDLPassGather -------------------------------------------------------------
void 
IDLPassGather::runPass ()
{
	doDefinitionList(m_state.m_rootscope.getNode(),m_state.m_rootscope);
	runJobs();
}




void 
IDLPassGather::doTypedef (IDL_tree  node,
			  IDLScope &scope)
{
	Super::doTypedef (node, scope);
}



void 
IDLPassGather::doStruct (IDL_tree  node,
			 IDLScope &scope)
{
}



void 
IDLPassGather::doUnion(IDL_tree node,IDLScope &scope) {

}







void 
IDLPassGather::doNative(IDL_tree node,IDLScope &scope) {
	ORBITCPP_NYI("native")
}







void 
IDLPassGather::doAttribute (IDL_tree  node,
			    IDLScope &scope)
{
	for(IDL_tree item = IDL_ATTR_DCL(node).simple_declarations;
		item; item = IDL_LIST(item).next)  {
		IDL_tree dcl = IDL_LIST(item).data;
		new IDLAttribute(node, dcl);
	}
}

void 
IDLPassGather::doConstant(IDL_tree  node,
			  IDLScope &scope)
{
	string id;
	IDLType *type =
		m_state.m_typeparser.parseDcl(IDL_CONST_DCL(node).ident,IDL_CONST_DCL(node).const_type,id);
	
	ORBITCPP_MEMCHECK(new IDLConstant(type,id,node,&scope));
}

void 
IDLPassGather::doSequence (IDL_tree node,
			   IDLScope &scope)
{
	IDL_tree member_type_node = IDL_TYPE_SEQUENCE (node).simple_type_spec;
        IDLType *type = m_state.m_typeparser.parseTypeSpec (&scope, member_type_node);

	//int seq_bound = IDL_INTEGER (IDL_TYPE_SEQUENCE (node).positive_int_const).value;

	IDLSequence seq (*type, 0);
	m_state.m_seqs.register_seq (seq);
	
	Super::doSequence (node, scope);
}


void 
IDLPassGather::doEnum (IDL_tree  node,
		       IDLScope &scope)
{
}

void 
IDLPassGather::doOperation (IDL_tree  node,
			    IDLScope &scope)
{
}




void 
IDLPassGather::doMember (IDL_tree  node,
			 IDLScope &scope)
{
	string id;

	IDL_tree dcl_list = IDL_MEMBER(node).dcls;
	while (dcl_list) {
		IDLType *type = m_state.m_typeparser.parseDcl(IDL_LIST(dcl_list).data,IDL_TYPE_DCL(node).type_spec,id);
		
		IDLElement *member = new IDLMember(type,id,IDL_LIST(dcl_list).data,&scope);
		ORBITCPP_MEMCHECK(member)

		dcl_list = IDL_LIST(dcl_list).next;
	}
}

void 
IDLPassGather::doCaseStmt (IDL_tree  node,
			   IDLScope &scope)
{
	string id;

	// member
	IDL_tree member = IDL_CASE_STMT(node).element_spec;
	g_assert(IDL_NODE_TYPE(member) == IDLN_MEMBER);
	
	IDL_tree dcl = IDL_LIST(IDL_MEMBER(member).dcls).data;   // dont handle multiple dcls	
	g_assert(IDL_NODE_TYPE(dcl) == IDLN_IDENT);

	IDLType *type = 
		m_state.m_typeparser.parseDcl(dcl,IDL_TYPE_DCL(member).type_spec,id);
	
	IDLMember *themember = new IDLMember(type,id,dcl);  // don't attach this to the scope
	new IDLCaseStmt(themember,id,node,&scope);  // attach the case stmt instead
	// case stmt takes ownership of member
}

void 
IDLPassGather::doException (IDL_tree node,
			    IDLScope &scope)
{
	IDLException *except = new IDLException (
		IDL_IDENT(IDL_EXCEPT_DCL(node).ident).str, node, &scope);
	ORBITCPP_MEMCHECK(except);

	Super::doException (node, *except);
}




void 
IDLPassGather::doInterface (IDL_tree  node,
			    IDLScope &scope)
{
	string ident = IDL_IDENT(IDL_INTERFACE(node).ident).str;
	IDLInterface *iface = new IDLInterface(ident,node,&scope, IDLInterface::definition);
	ORBITCPP_MEMCHECK(iface)
	m_state.m_interfaces.push_back(iface);

	Super::doInterface(node,*iface);
}




void
IDLPassGather::doForwardDcl (IDL_tree  node,
			     IDLScope &scope)
{
	string ident = IDL_IDENT(IDL_INTERFACE(node).ident).str;
	IDLInterface *iface = new IDLInterface(ident,node,&scope, IDLInterface::forward_dcl);
	ORBITCPP_MEMCHECK(iface)
	// Don't put this interfaces into the m_state.m_interfaces, or the declaration
	// will be written twice!
	// m_state.m_interfaces.push_back(iface);
}




void 
IDLPassGather::doModule (IDL_tree  node,
			 IDLScope &scope)
{
	char const* id = IDL_IDENT(IDL_MODULE(node).ident).str;
	typedef std::map<std::string, IDLScope::ItemList> Map;
	static Map item;

	IDLScope *module = new IDLModule (
		id, node, &scope, &item[id]
		);
	ORBITCPP_MEMCHECK(module)

	Super::doModule(node,*module);
}




