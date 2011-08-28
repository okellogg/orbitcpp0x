#ifndef ORBITCPP_IDLCASESTMT
#define ORBITCPP_IDLCASESTMT

#include <list>
#include <string>
#include <libIDL/IDL.h>
#include "IDLMember.h"
#include "IDLElement.h"
#include "IDLInhibited.h"

/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
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
 *  Purpose: IDL compiler language representation
 *
 */

class IDLScope;

class IDLCaseStmt :
	public IDLElement,
	public virtual IDLNotAType,
	public IDLIdentified {
public:
	typedef std::list<std::string>  LabelList;
	typedef LabelList::const_iterator  const_iterator;
private:
	IDLMember *m_member;
	LabelList m_labels;
	bool m_isDefault;
public:
	// takes ownership of member
	IDLCaseStmt (IDLMember    *member,
		     // scopePrefix is required for the case of enum discriminant
		     // from different scope: In C++ the enum values at "case"
		     // must be prefixed with the (foreign) scope
		     std::string const &scopePrefix,
		     IDL_tree      node,
		     IDLScope     *parentscope = 0);

	~IDLCaseStmt() {
		delete m_member;
	}

	const IDLMember& get_member () const {
		return *m_member;
	}
	const_iterator labelsBegin() const {
		return m_labels.begin();
	}
	const_iterator labelsEnd() const {
		return m_labels.end();
	}
	bool isDefault() const {
		return m_isDefault;
	}
};

#endif  // ORBITCPP_IDLCASESTMT

