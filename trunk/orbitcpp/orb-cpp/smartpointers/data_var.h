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

#ifndef __ORBITCPP_SMARTPTR_DATA_VAR_HH
#define __ORBITCPP_SMARTPTR_DATA_VAR_HH

namespace _orbitcpp
{

template<class T>
class Data_out;

template<class T>
class Data_var {
	friend class Data_out<T>;
protected:
	T		*m_data;

public:
	// construction and destruction
	Data_var()
		: m_data(NULL) {
	}
	Data_var(T *data)
		: m_data(data) {
	}
	Data_var(Data_var const &src)
		: m_data(new T(*src.m_data)) {
	}
	~Data_var() {
		if (m_data) delete m_data;
	}

	// assignment
	Data_var &operator=(T *data) {
		if (m_data) delete m_data;
		m_data = data;
		return *this;
	}
	Data_var &operator=(Data_var const &src) {
		T *copy = new T(*src.m_data);
		if (m_data) delete m_data;
		m_data = copy;
		return *this;
	}

	// access
	T *operator->() {
		return m_data;
	}
	T const *operator->() const {
		return m_data;
	}

	operator T &() {
		return *m_data;
	}

	// GCC spews warnings if we include this.
	// But it is needed for const correctness with in arguments.
	operator T const &() const {
		return *m_data;
	}
	operator T *() const {
		return m_data;
	}

	// parameter passing conversion
	T const &in() const {
		return *m_data;
	}
	T &inout() {
		return *m_data;
	}
	T &out() {
		if (m_data) {
			delete m_data;
			m_data = NULL;
		}
		return *m_data;
	}
	T *_retn() {
		T *temp = m_data;
		m_data = NULL;
		return temp;
	}
};

} //namespace _orbitcpp


#endif //__ORBITCPP_SMARTPTR_DATA_VAR_HH
