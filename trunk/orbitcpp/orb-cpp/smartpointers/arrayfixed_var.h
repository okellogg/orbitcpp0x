/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *	ORBit-C++: C++ bindings for ORBit.
 *
 *	Copyright (C) 2000-2002 The orbitcpp team
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Library General Public
 *	License as published by the Free Software Foundation; either
 *	version 2 of the License, or (at your option) any later version.
 *
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *	Library General Public License for more details.
 *
 *	You should have received a copy of the GNU Library General Public
 *	License along with this library; if not, write to the Free
 *	Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */


#ifndef __ORBITCPP_SMARTPTR_ARRAYFIXED_VAR_HH
#define __ORBITCPP_SMARTPTR_ARRAYFIXED_VAR_HH

namespace _orbitcpp
{

template <
	class T_slice,
	class C_slice,
	T_slice* (*alloc_fn)(),
	void (*free_fn)(T_slice*), 
	void (*copy_fn)(T_slice*, const T_slice*),
	void (*pack_fn)(C_slice*, T_slice*),
	void (*unpack_fn)(T_slice*, C_slice*)>
struct ArrayProperties {
	static T_slice *alloc() { return (*alloc_fn)(); }
	static void free(T_slice *array) { return (*free_fn)(array); }
	static void copy(T_slice *dest, T_slice const *src)
	{ (*copy_fn)(dest, src); }
	static void pack(C_slice* c_slice, T_slice* t_slice)
	{ (*pack_fn)(c_slice, t_slice); }
	static void unpack(T_slice* t_slice, C_slice* c_slice)
	{ (*unpack_fn)(t_slice, c_slice); }
};

template<class T_slice, CORBA::ULong len, class Properties>
class ArrayFixed_var {
private:
	T_slice *m_data;

public:

	ArrayFixed_var() {
		// This initialization is needed because an ArrayVar may be
		// used as an out-parameter without prior initialization
		// (see example in [16.12])
		m_data = Properties::alloc();
	}
	ArrayFixed_var(T_slice *data) {
		m_data = data;
  	}
	ArrayFixed_var(ArrayFixed_var const &src){
		m_data = Properties::alloc();
		Properties::copy(m_data,src.m_data);
	}

	ArrayFixed_var &operator=(T_slice *data) {
		if (m_data) Properties::free(m_data);
		m_data = data;
		return *this;
	}

	ArrayFixed_var &operator=(ArrayFixed_var const &src) {
		Properties::copy(m_data,src.m_data);
		return *this;
	}

	~ArrayFixed_var() {
		if (m_data) Properties::free(m_data);
	}

	T_slice &operator[](CORBA::ULong index) {
		g_assert (m_data);
		return m_data[index];
  	}

	T_slice const &operator[](CORBA::ULong index) const {
		g_assert (m_data);
		return m_data[index];
  	}

	operator T_slice *() {
		return m_data;
	}

/*
// GCC spews warnings if we include this
	operator T_slice const *() const {
		return m_data;
	}
*/
	// parameter passing conversion
	T_slice const *in() const {
		return m_data;
	}
	T_slice *inout() {
		return m_data;
	}
	T_slice *out() {
		return m_data;
	}
	T_slice *_retn() {
		T_slice *temp = m_data;
		m_data = NULL;
		return temp;
	}
};
  
} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_ARRAYFIXED_VAR_HH
