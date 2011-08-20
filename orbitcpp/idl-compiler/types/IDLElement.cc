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

#include "IDLElement.h"

#include "IDLScope.h"
#include "IDLInterface.h"
#include <types.h>

#include <algorithm>
#include <deque>

IDLElement::IDLElement(string const &id,IDL_tree node,IDLScope *parentscope, ReplaceType update)
	: m_identifier(id),m_node(node),m_parentscope(parentscope)
{
	if (m_parentscope) {

		IDLElement *slot = parentscope->getItem(id);

		// I've removed the following check, since forward dcls mean that
		// there can be duplicate identifiers -PD  
		// okay, libIDL should catch all real evil cases for us anyway. -andy
		//if (slot) throw IDLExDuplicateIdentifier(node,*parentscope,m_identifier);

		if (slot && update == replace) {
			// replace the old interface (forward dcl) with this new one
			// (which will have more info)
			std::replace(parentscope->begin(),parentscope->end(),slot,this);
		}
		else
			m_parentscope->m_items->push_back(this);
	} 
}

IDLScope*
IDLElement::getParentScope() const {
	if (m_parentscope == 0) 
		mk_parentscope();

	return m_parentscope;
}

void
IDLElement::mk_parentscope() const
{
	if (m_node == 0)
		return;

	std::deque<IDL_tree> ancestor;
	traverse_up(IDL_NODE_UP(m_node), std::back_inserter(ancestor));

	for (size_t i = 0, sz = ancestor.size(); i < sz; ++i) {
		IDL_tree a = ancestor[i];

		if (IDL_NODE_TYPE(a) == IDLN_IDENT || !IDL_NODE_IS_SCOPED(a)) 
			continue;
		
		m_parentscope = IDLTypeParser::parseScope(0, a);
		break; 
	}
}

IDLElement::~IDLElement()
{
}



string
IDLTypenameInterface::get_cpp_method_prefix () const
{
	string retval = get_cpp_typename ();
	
	// Remove :: from head
	string::iterator i = retval.begin ();
	while (i != retval.end () && *i == ':')
		i = retval.erase (i);

	return retval;
}

string
IDLTypenameInterface::get_c_method_prefix () const
{
    return get_c_typename ();
}

IDLScope const *
IDLElement::getRootScope() const {
	IDLScope const *run = getParentScope();
	IDLScope const *tug = getParentScope();

	while (run) {
		tug = run;
		run = run->getParentScope();
	}
	return tug;
}
