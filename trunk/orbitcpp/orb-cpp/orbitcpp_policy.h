/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2006 Eugene Sizikov
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
 *  Author: Eugene Sizikov <eugene_beast@mail.ru>
 *
 *  Description: Policy Header file
 */




#ifndef ORBITCPP_POLICY_HH
#define ORBITCPP_POLICY_HH

#include "orbitcpp/orb-cpp/orbitcpp_exception.h"
#include "orbitcpp/orb-cpp/orbitcpp_simple_seq.h"
#include "orbitcpp/orb-cpp/orbitcpp_object.h"

namespace CORBA {

	typedef CORBA_PolicyType PolicyType;
	
	class Policy : public CORBA::Object
	{
	public:
		explicit Policy (CORBA_Policy cobject);
		Policy ();
	
		static CORBA::Policy_ptr _duplicate (CORBA::Policy_ptr obj);
		static CORBA::Policy_ptr _narrow (CORBA::Object_ptr obj);
		static CORBA::Policy_ptr _nil ();
	
		CORBA::PolicyType policy_type ();
	
		Policy_ptr copy ();
		void destroy ();
	};

	typedef ::_orbitcpp::SimpleUnboundedSeq<
		_orbitcpp::seq_traits<
			::CORBA::Policy,
			::CORBA::Policy_var,
			CORBA_Policy,
			CORBA_PolicyList,
			&TC_CORBA_PolicyList_struct > > PolicyList;

	//typedef sequence <PolicyType> PolicyTypeSeq;
	//// InvalidPolicies added by frehberg@gnome-de.org
	//exception InvalidPolicies { sequence <unsigned short> indices; };

	typedef CORBA_PolicyErrorCode PolicyErrorCode;
	//exception PolicyError {PolicyErrorCode reason;};
	
#define P(name) const PolicyErrorCode name = CORBA_##name
	P(BAD_POLICY);
	P(UNSUPPORTED_POLICY);
	P(BAD_POLICY_TYPE);
	P(BAD_POLICY_VALUE);
	P(UNSUPPORTED_POLICY_VALUE);
#undef P
}


namespace _orbitcpp {

template <CORBA_TypeCode_struct const* c_seq_tc>
struct seq_traits<
	CORBA::Policy,
	CORBA::Policy_var,
	CORBA_Policy,
	CORBA_PolicyList,
	c_seq_tc> {

	typedef CORBA::Policy		class_t;
	typedef CORBA::Policy_var	value_t;
	typedef CORBA_Policy		c_value_t;
	typedef CORBA_PolicyList	c_seq_t;

	static CORBA_TypeCode seq_tc() { return TC_CORBA_PolicyList; }

	static c_seq_t* alloc_c ()
	{
	    return CORBA_PolicyList__alloc ();
	}
	
	static c_value_t* alloc_c_buf (size_t length)
	{
	    return CORBA_PolicyList_allocbuf (length);
	}
	
	static void pack_elem (const value_t& cpp_value, c_value_t &c_value)
	{
		c_value = (CORBA_Policy)cpp_value.in ()->_orbitcpp_cobj ();
	}
	
	static void unpack_elem (value_t& cpp_value, const c_value_t &c_value)
	{
		cpp_value.out ()->_orbitcpp_cobj () = (CORBA_Object)c_value;
	}
};

} // namespace _orbitcpp

#endif // ORBITCPP_POLICY_HH
