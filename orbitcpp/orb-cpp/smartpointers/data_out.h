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

#ifndef __ORBITCPP_SMARTPTR_DATA_OUT_HH
#define __ORBITCPP_SMARTPTR_DATA_OUT_HH

#include "orbitcpp/orb-cpp/smartpointers/data_var.h"

namespace _orbitcpp
{

template<class T>
class Data_out {
protected:
	T *&m_data;

public:
	// constructors and destructors
	Data_out(T *&data)
		: m_data(data) {
	}
	Data_out(Data_var<T> &var)
		: m_data(var.m_data) {
		delete m_data;
		m_data = NULL;
	}
	Data_out(Data_out const & src)
		: m_data(src.m_data) {
	}
	~Data_out() {
	}

	// assignment
	Data_out &operator=(Data_out &src) {
		m_data = src.m_data;
		return src;
	}
	Data_out &operator=(T *src) {
		m_data = src;
		return *this;
	}

	// access
	operator T *&() {
		return m_data;
	}
	T *operator->() {
		return m_data;
	}
	T *&ptr() {
		return m_data;
	}
private: // forbidden
	void operator=(Data_var<T> &);
};

} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_DATA_OUT_HH
