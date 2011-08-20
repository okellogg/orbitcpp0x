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

#ifndef __ORBITCPP_SMARTPTR_SEQUENCE_VAR_HH
#define __ORBITCPP_SMARTPTR_SEQUENCE_VAR_HH

namespace _orbitcpp
{

template<class T>
class Sequence_var : public Data_var<T> {
protected:
	typedef typename T::elem_t elem_t;
	typedef typename T::index_t index_t;
	typedef Data_var<T>         Super;

public:
	Sequence_var () {
		}
	Sequence_var (T *data)
		: Super(data) {
  	}
	Sequence_var (Sequence_var const &src)
		: Super(src) {
	}

	Sequence_var &operator= (T *data) {
		if (this->m_data) delete this->m_data;
		this->m_data = data;
		return *this;
	}

	Sequence_var &operator= (Sequence_var const &src) {
		T *copy = new T(*src.m_data);
		if (this->m_data) delete this->m_data;
		this->m_data = copy;
		return *this;
	}

	elem_t& operator[] (index_t index) {
		return this->m_data->operator[](index);
  	}
	const elem_t& operator[] (index_t index) const {
		return this->m_data->operator[](index);
  	}
};

} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_SEQUENCE_VAR_HH
