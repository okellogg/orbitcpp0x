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

#ifndef __ORBITCPP_SMARTPTR_ARRAYVARIABLE_OUT_HH
#define __ORBITCPP_SMARTPTR_ARRAYVARIABLE_OUT_HH

#include "orbitcpp/orb-cpp/smartpointers/arrayvariable_var.h"

namespace _orbitcpp
{

template<class T_slice, CORBA::ULong len, class Properties>
class ArrayVariable_out {
private:
	T_slice *&m_data;
public:
	ArrayVariable_out(T_slice *&data)
		: m_data(data){
	}
	typedef ArrayVariable_var<T_slice, len, Properties> AV_var;
	ArrayVariable_out(AV_var &var)
		: m_data(var.m_data) {
		Properties::free(m_data);
		m_data = NULL;
	}

	ArrayVariable_out(ArrayVariable_out const &src)
		: m_data(src.m_data) {
  	}

	// assignment
	ArrayVariable_out &operator=(ArrayVariable_out &src) {
		m_data = src.m_data;
		return src;
	}
	ArrayVariable_out &operator=(T_slice *src) {
		m_data = src;
		return *this;
	}

	// index
	T_slice &operator[](CORBA::ULong index) {
		g_assert (m_data);
		return m_data[index];
  	}
	T_slice const &operator[](CORBA::ULong index) const {
		g_assert (m_data);
		return m_data[index];
  	}

	// access
	operator T_slice *&() {
		return m_data;
	}
	T_slice *&ptr() {
		return m_data;
	}
private: // forbidden
	void operator=(AV_var &);

};

} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_ARRAYVARIABLE_OUT_HH
