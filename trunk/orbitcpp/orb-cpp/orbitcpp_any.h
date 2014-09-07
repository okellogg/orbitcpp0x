/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2000 John Luebs
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
 *  Author: John Luebs <jkluebs@marikarpress.com>
 *
 */

#ifndef ORBITCPP_ANY_HH
#define ORBITCPP_ANY_HH

#include <cstdlib>
#include <orbit/orbit.h>
#include "orbitcpp/orb-cpp/orbitcpp_traits.h"
#include "orbitcpp/orb-cpp/orbitcpp_types.h"
#include "orbitcpp/orb-cpp/orbitcpp_object.h"
#include "orbitcpp/orb-cpp/orbitcpp_exception.h"
#include "orbitcpp/orb-cpp/orbitcpp_typecode.h"

namespace CORBA {
	class Any;
	
	typedef _orbitcpp::DataVar_var<Any> Any_var;
	typedef _orbitcpp::DataVar_out<Any> Any_out;
	
	class Any {
	private:
		CORBA_any m_target;
		
		void copy (const Any &in);
		void free ();

	public:		
		// begin ORBitcpp extension
		typedef CORBA_any                c_value_t;


		
		CORBA_any* _orbitcpp_cobj () const {
			return const_cast<CORBA_any*> (&m_target);
		}
		
		static Any _orbitcpp_wrap (const CORBA_any *c_any);
		
		void insert_simple(CORBA::TypeCode_ptr, void*, bool v_copy = true);
		void insert_simple(CORBA::TypeCode_ptr tc,const void* in, bool v_copy = true) {
				insert_simple(tc, const_cast<void*>(in), v_copy);
		}

		template <class T>
		bool extract(CORBA::TypeCode_ptr tc, T& value) const {
			if (m_target._type != tc->_orbitcpp_cobj ())
				return false;
			
			value = *reinterpret_cast<T*>(m_target._value);
			return true;
		}
		
		template <class T>
		bool extract_ptr(CORBA::TypeCode_ptr tc, const T*& value) const {
			if (m_target._type != tc->_orbitcpp_cobj ())
				return false;
			
			value = reinterpret_cast<T*>(m_target._value);
			return true;
		}
		// end ORBitcpp extension

		Any();
		~Any();
		
		Any(const Any& in);
		Any& operator =(const Any& in);
		
		const void *value() const {
			return m_target._value;
		}
				
		struct from_boolean {
			from_boolean(bool b) : val(b) {}
			bool val;
		};
		struct from_octet {
			from_octet(Octet o) : val(o) {}
			Octet val;
		};
		struct from_char {
			from_char(Char c) : val(c) {}
			Char val;
		};
		struct from_wchar {
			from_wchar(WChar wc) : val(wc) {}
			WChar val;
		};
		struct from_string {
			from_string(char *s, ULong b, bool n = false)
				: val(s), bound(b), nocopy(n) {}
			from_string(const char *s, ULong b)
				: val(const_cast<char*>(s)), bound(b), nocopy(0) {}
			char *val;
			ULong bound;
			bool nocopy;
		};
		struct from_wstring {
			from_wstring(WChar *s, ULong b, bool n = false)
				: val(s), bound(b), nocopy(n) {}
			from_wstring(const WChar *s, ULong b)
				: val(const_cast<WChar*>(s)), bound(b), nocopy(0) {}
			WChar *val;
			ULong bound;
			bool nocopy;
		};
		/*
		struct from_fixed {
			from_fixed(const Fixed& f, UShort d, UShort s)
				: val(f), digits(d), scale(s) {}
			const Fixed& val;
			UShort digits;
			UShort scale;
		};
		*/
		
		struct to_boolean {
			to_boolean(bool& b) : ref(b) {}
			bool &ref;
		};
		struct to_octet {
			to_octet(Octet &o) : ref(o) {}
			Octet &ref;
		};
		struct to_char {
			to_char(Char &c) : ref(c) {}
			Char &ref;
		};
		struct to_wchar {
			to_wchar(WChar &wc) : ref(wc) {}
			WChar &ref;
		};
		struct to_string {
			to_string(const char *&s, ULong b)
				: val(s), bound(b) {}
			const char *&val;
			ULong bound;
		};
		struct to_wstring {
			to_wstring(const WChar *&s, ULong b)
				: val(s), bound(b) {}
			const WChar *&val;
			ULong bound;
		};
		/*
		struct to_fixed {
			to_fixed(Fixed& f, UShort d, UShort s)
				: val(f), digits(d), scale(s) {}
			Fixed& val;
			UShort digits;
			UShort scale;
		};
		*/
		struct to_object {
			to_object(Object_ptr& obj) : ref(obj) {}
			Object_ptr &ref;
		};
		
		void operator<<=(Short in) {
			insert_simple(_tc_short, &in);
		}
		void operator<<=(UShort in) {
			insert_simple(_tc_ushort, &in);
		}
		void operator<<=(Long in) {
			insert_simple(_tc_long, &in);
		}
		void operator<<=(ULong in) {
			insert_simple(_tc_ulong, &in);
		}
		void operator<<=(LongLong in) {
			insert_simple(_tc_longlong, &in);
		}
		void operator<<=(ULongLong in) {
			insert_simple(_tc_ulonglong, &in);
		}
		void operator<<=(Float in) {
			insert_simple(_tc_float, &in);
		}
		void operator<<=(Double in) {
			insert_simple(_tc_double, &in);
		}
		// ORBit does not implement this for good reasons
		/*void operator<<=(LongDouble in) {
			insert_simple(_tc_longdouble, &in);
		}*/
		void operator<<=(const Any & in) {
			insert_simple(_tc_any, &in);
		}
		void operator<<=(Any * in) {
			insert_simple(_tc_any, in, false);
		}
		//void operator<<=(const Exception &);
		//void operator<<=(Exception *);
		void operator<<=(const char * in) {
			insert_simple(_tc_string, &in);
		}
		void operator<<=(const WChar * in) {
			insert_simple(_tc_wstring, &in);
		}
		void operator<<=(from_boolean in) {
			insert_simple(_tc_boolean, &in.val);
		}
		void operator<<=(from_octet in) {
			insert_simple(_tc_octet, &in.val);
		}
		void operator<<=(from_char in) {
			insert_simple(_tc_char, &in.val);
		}
		void operator<<=(from_wchar in) {
			insert_simple(_tc_wchar, &in.val);
		}
		void operator<<=(from_string in);
		void operator<<=(from_wstring in);
		void operator<<=(Object_ptr in) {
			insert_simple(_tc_Object, &in);
		}
		void operator<<=(Object_ptr * in) {
			insert_simple(_tc_Object, &in);
			release(*in);
		}
		//void operator<<=(from_fixed);
		
		bool operator>>=(Short& out) const {
			return extract(_tc_short, out);
		}
		bool operator>>=(UShort& out) const {
			return extract(_tc_ushort, out);
		}			
		bool operator>>=(Long& out) const {
			return extract(_tc_long, out);
		}
		bool operator>>=(ULong& out) const {
			return extract(_tc_ulong, out);
		}
		bool operator>>=(LongLong& out) const {
			return extract(_tc_longlong, out);
		}
		bool operator>>=(ULongLong& out) const {
			return extract(_tc_ulonglong, out);
		}
		bool operator>>=(Float& out) const {
			return extract(_tc_float, out);
		}
		bool operator>>=(Double& out) const {
			return extract(_tc_double, out);
		}
		//void operator>>=(LongDouble&) const;
		bool operator>>=(const char *& out) const {
			return extract(_tc_string, const_cast<char*&>(out));
		}
		bool operator>>=(const WChar *& out) const {
			return extract(_tc_wstring, const_cast<WChar*&>(out));
		}
		
		bool operator>>=(to_boolean out) const {
			return extract(_tc_boolean, out.ref);
		}
		bool operator>>=(to_octet out) const {
			return extract(_tc_octet, out.ref);
		}
		bool operator>>=(to_char out) const {
			return extract(_tc_char, out.ref);
		}
		bool operator>>=(to_wchar out) const {
			return extract(_tc_wchar, out.ref);
		}
		
		bool operator>>=(to_string out) const;
		bool operator>>=(to_wstring out) const;

		bool operator>>=(to_object out) const;
		bool operator>>=(Object_ptr& out) const {
			return extract(_tc_Object, out);
		}
		
		// Begin Orbit-CPP Extension
		bool operator>>=(Any & out) const {
			return extract(_tc_any, out);
		}
		// End Orbit-CPP Extension
		bool operator>>=(const Any *& out) const {
			return extract_ptr(_tc_any, out);
		}
		
	private:
		void operator<<=(unsigned char);
		bool operator>>=(unsigned char&) const;
	};
};

namespace _orbitcpp {

template <CORBA_TypeCode_struct const* c_seq_tc>
struct seq_traits<CORBA::Any, CORBA::Any, CORBA_any, CORBA_sequence_CORBA_any, c_seq_tc> {
	typedef CORBA::Any           value_t;
	typedef CORBA_any                 c_value_t;
	typedef CORBA_sequence_CORBA_any c_seq_t;
	static CORBA_TypeCode seq_tc() { return TC_CORBA_sequence_CORBA_any; }
        static c_seq_t* alloc_c ()
	{
	    return CORBA_sequence_CORBA_any__alloc ();
	}
	
	static c_value_t* alloc_c_buf (size_t length)
	{
	    return CORBA_sequence_CORBA_any_allocbuf (length);
	}
	
	static void pack_elem (const value_t& cpp_value, c_value_t &c_value)
	{
		CORBA_any__copy (&c_value, cpp_value._orbitcpp_cobj());
	}
	
	static void unpack_elem (value_t& cpp_value, const c_value_t &c_value)
	{
		CORBA_any__copy(cpp_value._orbitcpp_cobj(), &c_value);
	}
};

} // namespace _orbitcpp

#endif
