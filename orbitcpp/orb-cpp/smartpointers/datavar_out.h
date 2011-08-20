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

#ifndef __ORBITCPP_SMARTPTR_DATAVAR_OUT_HH
#define __ORBITCPP_SMARTPTR_DATAVAR_OUT_HH

#include "orbitcpp/orb-cpp/smartpointers/datavar_var.h"

namespace _orbitcpp
{

template<class T>
class DataVar_out {
protected:
	T *&m_data;

public:
	// constructors and destructors
	DataVar_out(T *&data)
		: m_data(data) {
	}
	DataVar_out(DataVar_var<T> &var)
		: m_data(var.m_data) {
		delete m_data;
		m_data = NULL;
	}
	DataVar_out(DataVar_out const & src)
		: m_data(src.m_data) {
	}
	~DataVar_out() {
	}

	// assignment
	DataVar_out &operator=(DataVar_out &src) {
		m_data = src.m_data;
		return src;
	}
	DataVar_out &operator=(T *src) {
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
	void operator=(DataVar_var<T> &);
};

} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_DATAVAR_OUT_HH
