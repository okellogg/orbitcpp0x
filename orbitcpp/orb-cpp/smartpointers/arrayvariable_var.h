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

#ifndef __ORBITCPP_SMARTPTR_ARRAYVARIABLE_VAR_HH
#define __ORBITCPP_SMARTPTR_ARRAYVARIABLE_VAR_HH

namespace _orbitcpp
{

template<class T_slice, CORBA::ULong len, class Properties>
class ArrayVariable_out;

template<class T_slice, CORBA::ULong len, class Properties>
class ArrayVariable_var {
private:
	T_slice *m_data;
	friend class ArrayVariable_out<T_slice, len, Properties>;
public:

	ArrayVariable_var() {
		// This initialization is needed because an ArrayVar may be
		// used as an out-parameter without prior initialization
		// (see example in [16.12])
		m_data = Properties::alloc();
	}
	ArrayVariable_var(T_slice *data) {
		m_data = data;
  	}
	ArrayVariable_var(ArrayVariable_var const &src){
		m_data = Properties::alloc();
		Properties::copy(m_data,src.m_data);
	}

	~ArrayVariable_var() {
		if (m_data) Properties::free(m_data);
	}


	ArrayVariable_var &operator=(T_slice *data) {
		if (m_data) Properties::free(m_data);
		m_data = data;
		return *this;
	}

	ArrayVariable_var &operator=(ArrayVariable_var const &src) {
		Properties::copy(m_data,src.m_data);
		return *this;
	}

	T_slice& operator[] (CORBA::UShort index) {
		g_assert (m_data);
		return m_data[index];
  	}
	const T_slice& operator[] (CORBA::UShort index) const {
		g_assert (m_data);
		return m_data[index];
  	}

	T_slice& operator[] (CORBA::Short index) {
		g_assert (m_data);
		return m_data[index];
  	}
	const T_slice& operator[] (CORBA::Short index) const {
		g_assert (m_data);
		return m_data[index];
  	}


	T_slice& operator[] (CORBA::ULong index) {
		g_assert (m_data);
		return m_data[index];
  	}

	const T_slice& operator[] (CORBA::ULong index) const {
		g_assert (m_data);
		return m_data[index];
  	}

	T_slice& operator[] (CORBA::Long index) {
		g_assert (m_data);
		return m_data[index];
  	}
	const T_slice& operator[] (CORBA::Long index) const {
		g_assert (m_data);
		return m_data[index];
  	}

	operator T_slice*& () {
		return out();
	}

	operator const T_slice* () const {
		return m_data;
	}

	operator const T_slice* () {
		return m_data;
	}

	// parameter passing conversion
	T_slice const *in() const {
		return m_data;
	}
	T_slice *inout() {
		return m_data;
	}
	T_slice *&out() {
		if (m_data) {
			Properties::free(m_data);
			m_data = NULL;
		}
		return m_data;
	}
	T_slice *_retn() {
		T_slice *temp = m_data;
		m_data = NULL;
		return temp;
	}
};






} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_ARRAYVARIABLE_VAR_HH
