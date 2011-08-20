// -*- Mode: C++; c-basic-offset: 4 -*-
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2004 Bowie Owens
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
 *  Authors: Bowie Owens <bowie.owens@csiro.au>
 */

#ifndef ORBITCPP_TRAITS_HH
#define ORBITCPP_TRAITS_HH

namespace _orbitcpp {

template <class C, class CSeq, CORBA_TypeCode_struct const* c_seq_tc>
struct seq_traits__ {
	typedef C c_value_t; 
	typedef CSeq c_seq_t; 

	static CORBA_TypeCode seq_tc()
	{
		return const_cast<CORBA_TypeCode>(c_seq_tc);
	} 

	static c_seq_t* alloc_c ()
	{ 
		return static_cast<c_seq_t*>(ORBit_small_alloc (seq_tc())); 
	} 

	static c_value_t* alloc_c_buf (CORBA::ULong l)
	{ 
		return static_cast<c_value_t*>(ORBit_small_allocbuf(seq_tc(), l)); 
	} 
};

template <class CXX, class CXX_value, class C, class CSeq, CORBA_TypeCode_struct const* c_seq_tc>
struct seq_traits : seq_traits__<C, CSeq, c_seq_tc> {
	typedef CXX_value value_t; 
	typedef C c_value_t; 

	static void pack_elem (const value_t &cpp_elem, c_value_t &c_elem) 
	{ 
		CXX::pack_elem(cpp_elem, c_elem); 
	} 

        static void unpack_elem (value_t &cpp_elem, const c_value_t &c_elem) 
        { 
		CXX::unpack_elem(cpp_elem, c_elem); 
        } 
};

template <class CXX, class C, class CSeq, CORBA_TypeCode_struct const* c_seq_tc, class props>
struct seq_traits_array : seq_traits__<C, CSeq, c_seq_tc> {
	typedef CXX value_t; 
	typedef C c_value_t; 

	static void pack_elem (const value_t &cpp_elem, c_value_t &c_elem) 
	{ 
		props::pack(cpp_elem, c_elem);
	} 
        static void unpack_elem (value_t &cpp_elem, const c_value_t &c_elem) 
        { 
		props::unpack(cpp_elem, c_elem);
        } 
};

template <class CXX, class C, class CSeq, CORBA_TypeCode_struct const* c_seq_tc>
struct seq_traits_assignable : seq_traits__<C, CSeq, c_seq_tc> {
	typedef CXX value_t; 
	typedef C c_value_t; 
	
	static void pack_elem (const value_t &cpp_elem, c_value_t &c_elem) 
	{ 
		c_elem = (c_value_t) cpp_elem; 
	} 
        static void unpack_elem (value_t &cpp_elem, const c_value_t &c_elem) 
        { 
		cpp_elem = (value_t) c_elem; 
        } 
};

} // namespace _orbitcpp

#endif
