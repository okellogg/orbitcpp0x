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

#include "IDLScope.h"

#include "orbitcpp/idl-compiler/error.h"

IDLScope::~IDLScope() {
	// Can't clean up items because modules share item lists.
}

IDLElement *
IDLScope::lookup(string const &id) const {
	const IDLScope *scope = this;
	string::size_type first = 0;

	if (id.substr(0,2) == "::") {
		scope = getRootScope();
		first += 2;
	}

	while (scope) {
		IDLElement *item = scope->lookupLocal(id.substr(first));
		if (item) return item;
		scope = scope->getParentScope();
	}
	return NULL;
}

IDLElement *
IDLScope::lookupLocal(string const &id) const {
	const IDLScope *scope = this;
	string::size_type first = 0;

	string::size_type nextscopeq = id.find("::",first);
	if (nextscopeq == string::npos) {
		return scope->getItem(id.substr(first));
	} else {
		int spos = 0;
		IDLScope *nextscope = NULL;
		while ((nextscope = scope->getScope(id.substr(first,nextscopeq-first),spos)) != NULL) {
			spos++;
			IDLElement* element = nextscope->lookupLocal(id.substr(nextscopeq+2,id.length()-nextscopeq-2));
			if (element != NULL) return element;
		}
	}

	return NULL;
}

IDLElement *
IDLScope::getItem(IDL_tree node) const {
	ItemList::const_iterator first = m_items->begin(),last = m_items->end();

	while (first != last) {
		if (node == (*first)->getNode()) return *first;
		first++;
	}
	g_assert(false);
	return NULL;
}

IDLElement *
IDLScope::getItem (string const &id) const
{
	for (ItemList::const_iterator i = m_items->begin ();
	     i != m_items->end (); i++)
	{
		if ((*i)->get_idl_identifier () == id) return *i;
	}
	
	return 0;
}

IDLScope *
IDLScope::getScope (string const &id,
		    int          &spos) const
{
	int pos_counter = 0;

	for (ScopeList::const_iterator i = m_scopes.begin ();
	     i != m_scopes.end (); i++, pos_counter++)
	{
		if ((*i)->get_idl_identifier () == id && pos_counter >= spos)
		{
			spos = pos_counter;
			return *i;
		}
	}

	return 0;
}

bool
IDLScope::hasTypeChildren() const {
	const_iterator first = begin(), last = end();
	
	while (first != last) {
		if ((*first)->isType()) return true;
		first++;
	}
	return false;
}
