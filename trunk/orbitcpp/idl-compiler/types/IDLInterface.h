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
 *  Purpose:	IDL compiler type representation
 *
 *
 */


#ifndef ORBITCPP_TYPES_IDLINTERFACE
#define ORBITCPP_TYPES_IDLINTERFACE

#include "IDLUserDefScopeType.h"
#include "IDLCompoundSeqElem.h"
#include "IDLElement.h"
#include "IDLInhibited.h"
#include <set>

class IDLInterfaceBase :
	public virtual IDLTypenameInterface,
	public IDLCompoundSeqElem,
	public virtual IDLType,
	protected IDLDefaultConstructed,
	public IDLStandardUnionable {	// FIXME: this is wrong but hard to fix
public:

	bool is_fixed () const { return false; };
		
	// Creating typedefs
	void typedef_decl_write (ostream          &ostr,
				 Indent           &indent,
				 IDLCompilerState &state,
				 const IDLTypedef &target,
				 const IDLTypedef *active_typedef = 0) const;
	
	// Stub declaration
	string stub_decl_arg_get (const string     &cpp_id,
				  IDL_param_attr    direction,
				  const IDLTypedef *active_typedef = 0) const;
	
	string stub_decl_ret_get (const IDLTypedef *active_typedef = 0) const;
	
	// Stub implementation -- argument
	void stub_impl_arg_pre (ostream          &ostr,
				Indent           &indent,
				const string     &cpp_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef = 0) const;
	
	string stub_impl_arg_call (const string     &cpp_id,
				   IDL_param_attr    direction,
				   const IDLTypedef *active_typedef = 0) const;
	
	void stub_impl_arg_post (ostream          &ostr,
				 Indent           &indent,
				 const string     &cpp_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef = 0) const;

	// Stub implementation -- return value
	void stub_impl_ret_pre (ostream          &ostr,
				Indent           &indent,
				const IDLTypedef *active_typedef = 0) const;
	
	void stub_impl_ret_call (ostream          &ostr,
				 Indent           &indent,
				 const string     &c_call_expression,
				 const IDLTypedef *active_typedef = 0) const;

	void stub_impl_ret_post (ostream          &ostr,
				 Indent           &indent,
				 const IDLTypedef *active_typedef = 0) const;
	
	// Skel declaration
	string skel_decl_arg_get (const string     &c_id,
				  IDL_param_attr    direction,
				  const IDLTypedef *active_typedef = 0) const;
	
	string skel_decl_ret_get (const IDLTypedef *active_typedef = 0) const;
	
	// Skel implementation -- argument
	void skel_impl_arg_pre (ostream          &ostr,
				Indent           &indent,
				const string     &c_id,
				IDL_param_attr    direction,
				const IDLTypedef *active_typedef = 0) const;
	
	string skel_impl_arg_call (const string     &c_id,
				   IDL_param_attr    direction,
				   const IDLTypedef *active_typedef = 0) const;
	
	void skel_impl_arg_post (ostream          &ostr,
				 Indent           &indent,
				 const string     &c_id,
				 IDL_param_attr    direction,
				 const IDLTypedef *active_typedef = 0) const;

	// Skel implementation -- return value
	void skel_impl_ret_pre (ostream          &ostr,
				Indent           &indent,
				const IDLTypedef *active_typedef = 0) const;

	void skel_impl_ret_call (ostream          &ostr,
				 Indent           &indent,
				 const string     &cpp_call_expression,
				 const IDLTypedef *active_typedef = 0) const;
    
	void skel_impl_ret_post (ostream          &ostr,
				 Indent           &indent,
				 const IDLTypedef *active_typedef = 0) const;
	
	// Compound declaration
	string get_cpp_member_typename (const IDLTypedef *active_typedef = 0) const;
	string get_c_member_typename   (const IDLTypedef *active_typedef = 0) const;

	string member_decl_arg_get (const IDLTypedef *active_typedef = 0) const;
	
	void member_impl_arg_copy (ostream          &ostr,
				   Indent           &indent,
				   const string     &cpp_id,
				   const IDLTypedef *active_typedef = 0) const;
	
	// Initialization
	void member_init_c   (ostream          &ostr,
			      Indent           &indent,
			      const string     &c_id,
			      const IDLTypedef *active_typedef = 0) const;

	// Compound conversion: C++ -> C
	void member_pack_to_c (ostream          &ostr,
			       Indent           &indent,
			       const string     &cpp_id,
			       const string     &c_id,
			       const IDLTypedef *active_typedef = 0) const;
	
	// Compound conversion: C -> C++
	void member_unpack_from_c  (ostream          &ostr,
				    Indent           &indent,
				    const string     &cpp_id,
				    const string     &c_id,
				    const IDLTypedef *active_typedef = 0) const;

	string get_cpp_typename_ptr () const;
	string get_cpp_typename_var () const;
	string get_cpp_typename_out () const;
	
	string get_cpp_identifier_ptr () const;
	string get_cpp_identifier_var () const;
	string get_cpp_identifier_out () const;

	virtual string get_cpp_stub_identifier () const;
	virtual string get_cpp_stub_typename () const;
	virtual string get_cpp_stub_method_prefix () const;

};

typedef IDLInhibited<IDLInterfaceBase> IDLInterfaceInhibited;

enum DoRecurse {
	recurse,
	no_recurse
};

IDL_tree uninhibit(IDL_tree iface);

template <class OutIter>
void
traverse_inheritance_spec(IDL_tree iface, DoRecurse do_recurse, OutIter out, std::set<IDL_tree>& closed)
{
	iface = uninhibit(iface);
	IDL_tree base = IDL_INTERFACE(iface).inheritance_spec;

	for (; base != 0; base = IDL_LIST(base).next) {
		IDL_tree b = uninhibit(IDL_LIST(base).data);

		if (closed.count(b))
			continue;
		else
			closed.insert(b);

		if (do_recurse == recurse)
			traverse_inheritance_spec(b, do_recurse, out, closed);

		*out++ = b;
	}
}

template <class OutIter>
void
traverse_inheritance_spec(IDL_tree iface, DoRecurse do_recurse, OutIter out)
{
	std::set<IDL_tree> closed;
	traverse_inheritance_spec(iface, do_recurse, out, closed);
}

template <class OutIter>
void
traverse_up(IDL_tree node, OutIter out)
{
	for (; node != 0; node = IDL_NODE_UP(node)) {
		*out++ = node;
	}
}

// interface class the way it should be.
class IDLInterfaceRight : public IDLInterfaceInhibited {
	IDL_tree iface_;

public:
	IDL_tree getNode() { return iface_; }
	IDLElement *getItem(IDL_tree node) const { g_assert(false); return NULL; }

	typedef vector<IDLInterfaceRight*> BaseList;

	class PushNew {
		BaseList& base_;
	public:
		PushNew(BaseList& base) : base_ (base) { }

		PushNew& operator++() { return *this; }
		PushNew& operator++(int) { return *this; }
		PushNew& operator*() { return *this; }
		PushNew& operator=(IDL_tree iface)
		{
			base_.push_back(new IDLInterfaceRight(iface));
			return *this;
		}
	};

	class PushNewNotIn {
		BaseList& base_;
		std::set<IDL_tree> closed_;
	public:
		PushNewNotIn(BaseList& base, IDL_tree closed) 
		:	base_ (base)
		{
			traverse_inheritance_spec(closed, recurse, std::inserter(closed_, closed_.end()));
		}

		PushNewNotIn& operator++() { return *this; }
		PushNewNotIn& operator++(int) { return *this; }
		PushNewNotIn& operator*() { return *this; }
		PushNewNotIn& operator=(IDL_tree iface)
		{
			if (closed_.count(iface) == 0)
				base_.push_back(new IDLInterfaceRight(iface));
			return *this;
		}
	};

	IDLInterfaceRight(IDL_tree node)
	:	IDLInterfaceInhibited(node != 0 ? IDL_INTERFACE(node).ident : 0),
		iface_ (node)
	{
		if (iface_ == 0)
			return;

		traverse_inheritance_spec(iface_, no_recurse, PushNew(m_bases));
		traverse_inheritance_spec(iface_, recurse, PushNew(m_allbases));

		if (!m_bases.empty())
			traverse_inheritance_spec(iface_, recurse, PushNewNotIn(m_all_mi_bases, m_bases[0]->iface_));
	}

	BaseList m_bases;
	BaseList m_allbases;
	BaseList m_all_mi_bases;

	string get_cpp_poa_identifier () const;
	string get_cpp_poa_typename () const;
	string get_cpp_poa_method_prefix () const;


	string get_c_poa_typename () const;
	string get_c_poa_epv  () const;
	string get_c_poa_vepv () const;

	void common_write_typedefs (ostream &ostr, Indent  &indent) const;
};

class IDLInterface :
	public IDLUserDefScopeType,
	public IDLInterfaceRight {
public:
	enum DclDefnType {
		forward_dcl,
		definition
	};

	IDLInterface(string const &id,IDL_tree node,IDLScope *parentscope = NULL, DclDefnType dcl = definition)
	:	IDLUserDefScopeType(id,node,parentscope, (dcl == definition) ? replace : no_replace),
		IDLInterfaceRight(node)
	{
	}

	IDLInterface(IDL_tree node)
	:	IDLUserDefScopeType(IDL_IDENT(IDL_INTERFACE(node).ident).str,node, 0, replace),
		IDLInterfaceRight(node)
	{
	}

	bool isVariableLength() const {
		return true;
	}

	// ambiguous where IDLInterfaceRight is correct
	string get_c_typename() const { return IDLInterfaceRight::get_c_typename(); }
	string get_cpp_typename() const { return IDLInterfaceRight::get_cpp_typename(); }
	string get_cpp_identifier() const { return IDLInterfaceRight::get_cpp_identifier(); }
	IDL_tree getNode() { return IDLInterfaceRight::getNode(); }


	// ambiguous where IDLUserDefScopeType is correct
	IDLElement *getItem(IDL_tree node) const { return IDLUserDefScopeType::getItem(node); }
};

#endif //ORBITCPP_TYPES_IDLINTERFACE

