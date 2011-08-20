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

#ifndef __ORBITCPP_SMARTPTR_ARRAYFIXED_FORANY_HH
#define __ORBITCPP_SMARTPTR_ARRAYFIXED_FORANY_HH

namespace _orbitcpp
{

template<class T_slice, CORBA::ULong len>
class ArrayFixed_forany {
private:
	T_slice *m_data;
	CORBA::Boolean m_nocopy;

public:
	ArrayFixed_forany() {
		m_data = NULL;
		m_nocopy = FALSE;
	}
	ArrayFixed_forany(T_slice *data, CORBA::Boolean nocopy = FALSE) {
		m_data = data;
		m_nocopy = nocopy;
  	}
	ArrayFixed_forany(ArrayFixed_forany<T_slice, len> const &src, CORBA::Boolean nocopy){
		m_data = src.m_data;
		m_nocopy = nocopy;
	}

	ArrayFixed_forany<T_slice, len> &operator=(T_slice *data) {
		m_data = data;
		return *this;
	}

	ArrayFixed_forany<T_slice, len> &operator=(ArrayFixed_forany<T_slice, len> const &src) {
		m_data = src.m_data;
		return *this;
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

	CORBA::Boolean _nocopy() { return m_nocopy; }
};

} //namespace _orbitcpp
#endif //__ORBITCPP_SMARTPTR_ARRAYFIXED_FORANY_HH
