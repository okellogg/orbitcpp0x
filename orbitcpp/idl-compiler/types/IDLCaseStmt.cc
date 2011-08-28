/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2000 Andreas Kloeckner
 *  Copyright (C) 2011 Oliver Kellogg
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

#include "IDLCaseStmt.h"
#include "IDLUnion.h"
#include "base.h"
#include "language.h"

IDLCaseStmt::IDLCaseStmt(IDLMember *member,
			 std::string const &scopePrefix,
			 IDL_tree node,
			 IDLScope *parentscope)
:	IDLElement(member->getIdentifier(), node, parentscope),
	IDLIdentified (IDL_LIST(IDL_MEMBER(IDL_CASE_STMT(node).element_spec).dcls).data), // dont handle multiple dcls
	m_member(member),
	m_isDefault(false)
{

	// labels
	g_assert(IDL_NODE_TYPE(node) == IDLN_CASE_STMT);
	IDL_tree list = IDL_CASE_STMT(node).labels;
	g_assert(IDL_NODE_TYPE(list) == IDLN_LIST);
	while (list) {
		IDL_tree label = IDL_LIST(list).data;
		if (label==NULL){
			m_isDefault=true;
			break;
		}
		m_labels.push_back(scopePrefix + idlTranslateConstant(label));
		list = IDL_LIST(list).next;
	}	
}

