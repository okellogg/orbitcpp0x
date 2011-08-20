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
 *  		Bowie Owens <bowie.owens@csiro.au>
 *
 *  Purpose:	IDL compiler type representation for types within an
 *  		inhibit block.
 *
 *
 */
#ifndef IDLInhibited_h_
#define IDLInhibited_h_

#include <string>
#include <libIDL/IDL.h>
#include "IDLElement.h"


struct EmptyType { };


template <class Base>
class IDLInhibited :
	virtual public Base {

	std::string ident_;
	std::string qual_ident_;
	std::string qual_c_ident_;
	std::string qual_idl_ident_;
	std::string qual_tc_ident_;

	std::vector<std::string> ns_;

	static std::string handle_keyword(std::string const& s)
	{
		if (idlIsCPPKeyword(s))
			return IDL_CPP_KEY_PREFIX + s;

		return s;
	}
public:
	IDLInhibited(IDL_tree ident)
	{
		if (ident == 0)
			return;

		ident_ = IDL_IDENT (ident).str;
		IDL_tree ns_ident = IDL_IDENT_TO_NS(ident);

		if (ns_ident == 0)
			return;

		g_assert (IDL_NODE_TYPE (ns_ident) == IDLN_GENTREE);

		IDL_tree l = IDL_ns_qualified_ident_new (ns_ident);
		bool first = true;
		for (IDL_tree q = l; q != 0; q = IDL_LIST (q).next) {
			g_assert (IDL_NODE_TYPE (q) == IDLN_LIST);
			IDL_tree i = IDL_LIST (q).data;
			g_assert (IDL_NODE_TYPE (i) == IDLN_IDENT);
			g_assert(IDL_IDENT (i).str != NULL);

			if (strlen(IDL_IDENT (i).str) == 0)
				continue;

			if (!first) {
				qual_tc_ident_.append("::");
				qual_idl_ident_.append("::");
				qual_ident_.append("::");
				qual_c_ident_.append("_");
			}
			first = false;

			if (IDL_LIST(q).next == 0) { // last
				qual_tc_ident_.append("_tc_");
			} else {
				ns_.push_back(handle_keyword(IDL_IDENT (i).str));
			}

			qual_idl_ident_.append(IDL_IDENT (i).str);
			qual_ident_.append(handle_keyword(IDL_IDENT (i).str));
			qual_tc_ident_.append(handle_keyword(IDL_IDENT (i).str));
			qual_c_ident_.append(IDL_IDENT (i).str);
		}
	}

	std::string get_idl_typename() const { return qual_idl_ident_; }
	std::string get_c_typename() const { return qual_c_ident_; }
	std::string get_cpp_typename() const { return qual_ident_; }

	std::string get_c_identifier() const { return ident_; }
	std::string get_idl_identifier () const { return ident_; }
	std::string get_cpp_identifier() const { return handle_keyword(ident_); }
	std::string get_cpp_typecode_name() const { return qual_tc_ident_; }

	void getCPPNamespaceDecl(string& begin, string& end, string const& pref = "") const
	{
		for (size_t i = 0, sz = ns_.size(); i < sz; ++i) {
			std::string n = ns_[i];
			if (i == 0)
				n = pref + n;
			begin.append("namespace " + n + " {\n");
			end.insert(0, "} // namespace " + n + "\n");
		}
	}

	void get_cpp_poa_namespace (string &ns_begin, string &ns_end) const
	{
		getCPPNamespaceDecl(ns_begin, ns_end, "POA_");
	}
};

typedef IDLInhibited<IDLTypenameInterface> IDLIdentified;



#endif

