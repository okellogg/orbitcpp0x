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


#ifndef ORBITCPP_TYPES_IDLUSERDEFSIMPLETYPE
#define ORBITCPP_TYPES_IDLUSERDEFSIMPLETYPE

#include "language.h"
#include "IDLSimpleType.h"

// Handy for things like enum (and maybe fixed?)
// since a good default implementation is available in SimpleType
class IDLUserDefSimpleType
: public IDLElement,
  public IDLSimpleType
{
	
public:
	IDLUserDefSimpleType(string const &id,IDL_tree node,IDLScope *parentscope = NULL)
		: IDLElement(id,node,parentscope) {}


	////////////////////////////////////////////
	// Stubs

	// Stub declaration
	virtual string stub_decl_arg_get (const string     &cpp_id,
					  IDL_param_attr    direction,
					  const IDLTypedef *active_typedef = 0) const;
	
	virtual string stub_decl_ret_get (const IDLTypedef *active_typedef = 0) const;
	
	// Stub implementation -- argument
	virtual void stub_impl_arg_pre (ostream          &ostr,
					Indent           &indent,
					const string     &cpp_id,
					IDL_param_attr    direction,
					const IDLTypedef *active_typedef = 0) const;
	
	virtual string stub_impl_arg_call (const string     &cpp_id,
					   IDL_param_attr    direction,
					   const IDLTypedef *active_typedef = 0) const;
	
	virtual void stub_impl_arg_post (ostream          &ostr,
					 Indent           &indent,
					 const string     &cpp_id,
					 IDL_param_attr    direction,
					 const IDLTypedef *active_typedef = 0) const;

	// Stub implementation -- return value
	virtual void stub_impl_ret_pre (ostream          &ostr,
					Indent           &indent,
					const IDLTypedef *active_typedef = 0) const;

	virtual void stub_impl_ret_call (ostream          &ostr,
					 Indent           &indent,
					 const string     &c_call_expression,
					 const IDLTypedef *active_typedef = 0) const;

	virtual void stub_impl_ret_post (ostream          &ostr,
					 Indent           &indent,
					 const IDLTypedef *active_typedef = 0) const;
	
	////////////////////////////////////////////
	// Skels

	// Skel declaration
	virtual string skel_decl_arg_get (const string     &c_id,
					  IDL_param_attr    direction,
					  const IDLTypedef *active_typedef = 0) const;

	virtual string skel_decl_ret_get (const IDLTypedef *active_typedef = 0) const;
	
	// Skel implementation -- argument
	virtual void skel_impl_arg_pre (ostream          &ostr,
					Indent           &indent,
					const string     &c_id,
					IDL_param_attr    direction,
					const IDLTypedef *active_typedef = 0) const;
	
	virtual string skel_impl_arg_call (const string     &c_id,
					   IDL_param_attr    direction,
					   const IDLTypedef *active_typedef = 0) const;
	
	virtual void skel_impl_arg_post (ostream          &ostr,
					 Indent           &indent,
					 const string     &c_id,
					 IDL_param_attr    direction,
					 const IDLTypedef *active_typedef = 0) const;

	// Skel implementation -- return value
	virtual void skel_impl_ret_pre (ostream          &ostr,
					Indent           &indent,
					const IDLTypedef *active_typedef = 0) const;

	virtual void skel_impl_ret_call (ostream          &ostr,
					 Indent           &indent,
					 const string     &cpp_call_expression,
					 const IDLTypedef *active_typedef = 0) const;
    
	virtual void skel_impl_ret_post (ostream          &ostr,
					 Indent           &indent,
					 const IDLTypedef *active_typedef = 0) const;
};

#endif //ORBITCPP_TYPES_IDLUSERDEFSIMPLETYPE

