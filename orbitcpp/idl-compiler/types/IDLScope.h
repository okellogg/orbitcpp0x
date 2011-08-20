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

#ifndef ORBITCPP_TYPES_IDLSCOPE
#define ORBITCPP_TYPES_IDLSCOPE

#include "IDLElement.h"

#include <vector>

struct IDLScopeImplementation {
	enum Type {
		by_type,
		by_namespace
	};
	virtual Type scope_implementation() const = 0;
};

template <IDLScopeImplementation::Type by>
struct IDLScopeImplementationNode : public virtual IDLScopeImplementation {
	virtual Type scope_implementation() const
	{
		return by;
	}
};

typedef IDLScopeImplementationNode<IDLScopeImplementation::by_type> IDLScopeTypeImplementation;
typedef IDLScopeImplementationNode<IDLScopeImplementation::by_namespace> IDLScopeNamespaceImplementation;


class IDLScope : public IDLElement, public virtual IDLScopeImplementation {
public:
	typedef vector<IDLElement *>  ItemList;
	typedef vector<IDLScope *>    ScopeList;

protected:
	typedef IDLElement            Super;
	ItemList*		      m_items;
	ScopeList		      m_scopes;

public:
	typedef ItemList::iterator			iterator;
	typedef ItemList::const_iterator	const_iterator;
	

	IDLScope(string const &id,IDL_tree node,IDLScope *parentscope = NULL, ItemList* items = NULL, ReplaceType update = no_replace)
	:	IDLElement(id,node,parentscope,update),
		m_items(items != 0 ? items : new ItemList())
	{
		if (parentscope) parentscope->m_scopes.push_back(this);
	}

	~IDLScope();
	IDLElement *lookup(string const &id) const;
	IDLElement *lookupLocal(string const &id) const;

	IDLElement *getItem(IDL_tree node) const;
	IDLElement *getItem(string const &id) const;
	IDLScope *getScope (string const &id, int &spos) const;
	IDLScope const *getRootScope() const {
		if (getParentScope()) return Super::getRootScope();
		else return this;
	}
	bool hasTypeChildren() const;

	iterator begin() {
		return m_items->begin();
	}
	const_iterator begin() const {
		return m_items->begin();
	}
	iterator end() {
		return m_items->end();
	}
	const_iterator end() const {
		return m_items->end();
	}
	unsigned int size() const {
		return m_items->size();
	}

	friend class IDLElement;
};

#endif //ORBITCPP_TYPES_IDLSCOPE
