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


#ifndef ORBITCPP_TYPES_IDLTYPE
#define ORBITCPP_TYPES_IDLTYPE

#include "IDLElement.h"

class IDLTypedef;
class IDLScope;
class IDLCompilerState;
class IDLScope;

class IDLCaseStmt;
class IDLUnion;

class IDLUnionable {
public:
	virtual void create_union_accessors(IDLUnion const&, IDLCaseStmt const&, std::ostream& header, Indent, std::ostream& module, Indent) const = 0;
};

class IDLStandardUnionable :
	public virtual IDLUnionable {
public:
	void create_union_accessors(IDLUnion const&, IDLCaseStmt const&, std::ostream& header, Indent, std::ostream& module, Indent) const;

	static void create_union_setter(IDLUnion const&, IDLCaseStmt const&, std::ostream& header, Indent, std::ostream& module, Indent);

};

class IDLReferentUnionable :
	public virtual IDLUnionable {
public:
	void create_union_accessors(IDLUnion const&, IDLCaseStmt const&, std::ostream& header, Indent, std::ostream& module, Indent) const;
};

class IDLNotUnionable :
	public virtual IDLUnionable {
public:
	void create_union_accessors(IDLUnion const&, IDLCaseStmt const&, std::ostream&, Indent, std::ostream&, Indent) const { g_assert(false); }
};

class IDLType :
	public virtual IDLIsAType,
	public virtual IDLUnionable
{
public:
	enum Type {
		T_VOID, T_ENUM, T_BOOLEAN, T_INTEGER, T_FLOAT, T_FIXED, T_CHAR,
		T_STRING, T_ANY, T_INTERFACE, T_SEQUENCE, T_ARRAY, T_STRUCT, T_UNION
	};

	IDLType () { m_type = T_VOID; }   // T_VOID really means "not applicable" here
	IDLType (Type t) { m_type = t; }
	virtual ~IDLType () {}

	Type getType() const { return m_type; }

	// if this type is really an alias, this gets the aliased type
	IDLType const &getResolvedType() const;

	virtual bool conversion_required () const { return true; }
	virtual bool is_scalar () const { return false; }
	virtual bool is_fixed () const = 0;

	////////////////////////////////////////////
	// Constants

	virtual void const_decl_write (ostream          &header,
				       ostream		&module,
				       IDLScope const&	scope,
				       Indent           &indent,
				       const string     &cpp_id,
				       const string     &value,
				       const IDLTypedef *active_typedef = 0) const;

	////////////////////////////////////////////
	// Creating typedefs

	virtual void typedef_decl_write (ostream          &ostr,
					 Indent           &indent,
					 IDLCompilerState &state,
					 const IDLTypedef &target,
					 const IDLTypedef *active_typedef = 0) const = 0;
	
	////////////////////////////////////////////
	// Stubs

	// Stub declaration
	virtual string stub_decl_arg_get (const string     &cpp_id,
					  IDL_param_attr    direction,
					  const IDLTypedef *active_typedef = 0) const = 0;
	
	virtual string stub_decl_ret_get (const IDLTypedef *active_typedef = 0) const = 0;
	
	// Stub implementation -- argument
	virtual void stub_impl_arg_pre (ostream          &ostr,
					Indent           &indent,
					const string     &cpp_id,
					IDL_param_attr    direction,
					const IDLTypedef *active_typedef = 0) const = 0;
	
	virtual string stub_impl_arg_call (const string     &cpp_id,
					   IDL_param_attr    direction,
					   const IDLTypedef *active_typedef = 0) const = 0;
	
	virtual void stub_impl_arg_post (ostream          &ostr,
					 Indent           &indent,
					 const string     &cpp_id,
					 IDL_param_attr    direction,
					 const IDLTypedef *active_typedef = 0) const = 0;

	// Stub implementation -- return value
	virtual void stub_impl_ret_pre (ostream          &ostr,
					Indent           &indent,
					const IDLTypedef *active_typedef = 0) const = 0;

	virtual void stub_impl_ret_call (ostream          &ostr,
					 Indent           &indent,
					 const string     &c_call_expression,
					 const IDLTypedef *active_typedef = 0) const = 0;

	virtual void stub_impl_ret_post (ostream          &ostr,
					 Indent           &indent,
					 const IDLTypedef *active_typedef = 0) const = 0;
	
	////////////////////////////////////////////
	// Skels

	// Skel declaration
	virtual string skel_decl_arg_get (const string     &c_id,
					  IDL_param_attr    direction,
					  const IDLTypedef *active_typedef = 0) const = 0;

	virtual string skel_decl_ret_get (const IDLTypedef *active_typedef = 0) const = 0;
	
	// Skel implementation -- argument
	virtual void skel_impl_arg_pre (ostream          &ostr,
					Indent           &indent,
					const string     &c_id,
					IDL_param_attr    direction,
					const IDLTypedef *active_typedef = 0) const = 0;
	
	virtual string skel_impl_arg_call (const string     &c_id,
					   IDL_param_attr    direction,
					   const IDLTypedef *active_typedef = 0) const = 0;
	
	virtual void skel_impl_arg_post (ostream          &ostr,
					 Indent           &indent,
					 const string     &c_id,
					 IDL_param_attr    direction,
					 const IDLTypedef *active_typedef = 0) const = 0;

	// Skel implementation -- return value
	virtual void skel_impl_ret_pre (ostream          &ostr,
					Indent           &indent,
					const IDLTypedef *active_typedef = 0) const = 0;

	virtual void skel_impl_ret_call (ostream          &ostr,
					 Indent           &indent,
					 const string     &cpp_call_expression,
					 const IDLTypedef *active_typedef = 0) const = 0;
    
	virtual void skel_impl_ret_post (ostream          &ostr,
					 Indent           &indent,
					 const IDLTypedef *active_typedef = 0) const = 0;

	////////////////////////////////////////////
	// Members of compound types

	// Compound declaration
	virtual string get_cpp_member_typename (const IDLTypedef *active_typedef = 0) const = 0;
	virtual string get_c_member_typename   (const IDLTypedef *active_typedef = 0) const = 0;

	virtual string get_seq_typename (unsigned int      length,
					 const IDLTypedef *active_typedef = 0) const = 0;
	
	virtual string member_decl_arg_get (const IDLTypedef *active_typedef = 0) const = 0;
	
	virtual void member_impl_arg_copy (ostream          &ostr,
					   Indent           &indent,
					   const string     &cpp_id,
					   const IDLTypedef *active_typedef = 0) const = 0;

	// Initialization
	virtual void member_init_cpp (ostream          &ostr,
				      Indent           &indent,
				      const string     &cpp_id,
				      const IDLTypedef *active_typedef = 0) const = 0;

	virtual void member_init_c   (ostream          &ostr,
				      Indent           &indent,
				      const string     &c_id,
				      const IDLTypedef *active_typedef = 0) const = 0;
	
	// Compound conversion: C++ -> C
	virtual void member_pack_to_c (ostream          &ostr,
				       Indent           &indent,
				       const string     &cpp_id,
				       const string     &c_id,
				       const IDLTypedef *active_typedef = 0) const = 0;

	
	// Compound conversion: C -> C++
	virtual void member_unpack_from_c (ostream          &ostr,
					   Indent           &indent,
					   const string     &cpp_id,
					   const string     &c_id,
					   const IDLTypedef *active_typedef = 0) const = 0;
private:
	Type m_type;
};

class IDLDefaultConstructed : public virtual IDLType {
public:
	virtual void member_init_cpp (ostream          &,
				      Indent           &,
				      const string     &,
				      const IDLTypedef * = 0) const
	{
		// No op, default constructor is fine.
	}
};

#endif //ORBITCPP_TYPES_IDLTYPE


