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

#ifndef __ORBITCPP_SMARTPTR_SEQUENCE_OUT_HH
#define __ORBITCPP_SMARTPTR_SEQUENCE_OUT_HH

#include "orbitcpp/orb-cpp/smartpointers/sequence_var.h"

namespace _orbitcpp
{

template<class T>
class Sequence_out : public Data_out<T>  {
  protected:
	typedef typename T::elem_t elem_t;
	typedef typename T::index_t index_t;
	typedef Data_out<T>         Super;

  public:
	Sequence_out(T *&data)	: Super(data) {}
	Sequence_out(Data_var<T> &var)	: Super(var) {}
	Sequence_out(Sequence_out const & src)	: Super(src) {}

	// assignment
	Sequence_out &operator= (Sequence_out &src) {
		this->m_data = src.m_data;
		return src;
	}
	Sequence_out &operator= (T *src) {
		this->m_data = src;
		return *this;
	}

	elem_t& operator[] (index_t index) {
		return (*this->m_data)[index];
  	}
	const elem_t& operator[] (index_t index) const {
		return (*this->m_data)[index];
  	}
};
  
} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_SEQUENCE_OUT_HH
