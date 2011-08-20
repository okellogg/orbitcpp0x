// -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*-
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 1999 Phil Dawes
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
 *  Authors: Phil Dawes <philipd@users.sourceforge.net>
 *           Andreas Kloeckner <ak@ixion.net>
 *           Gergõ Érdi <cactus@cactus.rulez.org>
 */

#ifndef ORBITCPP_STRING_SEQUENCE_HH
#define ORBITCPP_STRING_SEQUENCE_HH

#include "orbitcpp/orb-cpp/orbitcpp_sequence.h"
#include "orbitcpp/orb-cpp/orbitcpp_smartptr.h"
#include <orbit/orb-core/corba-defs.h>

namespace _orbitcpp {

template <class CharT, class CSeq, CORBA_TypeCode_struct const* c_seq_tc>
struct seq_traits<String_var<CharT>, String_var<CharT>, CharT*, CSeq, c_seq_tc> {
	typedef String_var<CharT>           value_t;
	typedef CharT*                 c_value_t;
	typedef CSeq c_seq_t;
	static CORBA_TypeCode seq_tc() { return const_cast<CORBA_TypeCode>(c_seq_tc); }

	typedef StringProperties<CharT> char_traits_t;

        static c_seq_t* alloc_c ()
	{
	    return (c_seq_t *)ORBit_small_alloc (seq_tc()); 
	}
	
	static c_value_t* alloc_c_buf (size_t l)
	{
	    return (c_value_t *)ORBit_small_allocbuf(seq_tc(), l); 
	}
	
	static void pack_elem (const value_t& cpp_value, c_value_t &c_value)
	{
	    c_value = char_traits_t::dup (cpp_value);
	}
	
	static void unpack_elem (value_t& cpp_value, const c_value_t &c_value)
	{
	    cpp_value = char_traits_t::dup (c_value);
	}
};

} // namespace _orbitcpp

#endif // !ORBITCPP_STRING_SEQUENCE_HH

