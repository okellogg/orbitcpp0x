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

#ifndef ORBITCPP_TYPES_IDLELEMENT
#define ORBITCPP_TYPES_IDLELEMENT

#include <libIDL/IDL.h>
#include "base.h"
#include <string>
#include <vector>
#include <algorithm>

class IDLScope;

class IDLTypenameInterface {
public:
	virtual ~IDLTypenameInterface() { }
	virtual string get_c_typename   () const = 0;
	virtual string get_cpp_typename () const = 0;
	virtual string get_idl_typename () const = 0;

	virtual string get_c_method_prefix   () const;
	virtual string get_cpp_method_prefix () const;

	virtual string get_c_identifier () const = 0;
	virtual string get_cpp_identifier () const = 0;
	virtual string get_idl_identifier () const = 0;

	virtual void getCPPNamespaceDecl(string &ns_begin,string &ns_end,string const &prefix = "") const = 0;
};

class IDLTypenameUnused :
	public virtual IDLTypenameInterface {
	string get_idl_typename() const { g_assert_not_reached(); return ""; }
	string get_c_identifier() const { g_assert_not_reached(); return ""; }
	string get_idl_identifier () const { g_assert_not_reached(); return ""; }
	void getCPPNamespaceDecl(string&, string&, string const&) const { g_assert_not_reached(); }
};

class IDLTypeDiscrim {
public:
	virtual ~IDLTypeDiscrim() { }
	virtual bool isType() = 0;
};

class IDLNotAType : public virtual IDLTypeDiscrim {
	virtual bool isType() {
		return false;
	}
};

class IDLIsAType : public virtual IDLTypeDiscrim {
	virtual bool isType() {
		return true;
	}
};

class IDLNode {
	class InterfaceAncestor {
		bool b;
	public:
		InterfaceAncestor() : b (false) { }

		InterfaceAncestor& operator++() { return *this; }
		InterfaceAncestor& operator++(int) { return *this; }
		InterfaceAncestor& operator*() { return *this; }
		InterfaceAncestor& operator=(IDL_tree a)
		{
			b = b || is_interface(a);
			return *this;
		}

		operator bool() const { return b; }
	};

public:
	virtual ~IDLNode() { }

	virtual IDL_tree getNode() const = 0;

	static bool is_interface(IDL_tree n)
	{
		return IDL_NODE_TYPE(n) == IDLN_INTERFACE;
	}

	template <class Out>
	Out ancestors(Out out) const
	{
		IDL_tree a = IDL_NODE_UP(getNode());

		for (; a != 0; a = IDL_NODE_UP(a)) {
			*out++ = a;
		}

		return out;
	}

	IDL_tree parent_interface() const
	{
		typedef std::vector<IDL_tree> V;
		V a;
		ancestors(std::back_inserter(a));
		V::iterator i =
			std::find_if(a.begin(), a.end(), &is_interface);
		g_assert(i != a.end());
		return *i;
	}

	bool isInterface() const
	{
		return is_interface(getNode());
	}

	bool interface_child() const
	{
		InterfaceAncestor p;
		return ancestors(p);
	}
};

class IDLElement :
	public virtual IDLTypenameInterface,
	public virtual IDLTypeDiscrim,
	public virtual IDLNode {
protected:
	string			m_identifier;
	IDL_tree		m_node;
	mutable IDLScope		*m_parentscope;

	void mk_parentscope() const;

public:
	enum ReplaceType {
		replace,
		no_replace
	};

	IDLElement(string const &id,IDL_tree node,IDLScope *parentscope = NULL, ReplaceType = no_replace);
	virtual ~IDLElement();

	string getIdentifier() const {
		return m_identifier;
	}
	IDL_tree getNode() const {
		return m_node;
	}
	IDLScope *getParentScope() const;
	virtual IDLScope const *getRootScope() const;
};

#endif //ORBITCPP_TYPES_IDLELEMENT
