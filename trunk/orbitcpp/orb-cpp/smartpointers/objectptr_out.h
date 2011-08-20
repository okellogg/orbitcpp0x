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

#ifndef __ORBITCPP_SMARTPTR_OBJECTPTR_OUT_HH
#define __ORBITCPP_SMARTPTR_OBJECTPTR_OUT_HH

#include "orbitcpp/orb-cpp/smartpointers/objectptr_var.h"

namespace _orbitcpp
{

// according to spec 20.3.6
template<class O>
class ObjectPtr_out {
	typedef O* O_ptr;
protected:
	O_ptr	&m_objectref;

public:
	ObjectPtr_out(O_ptr &ptr)
		: m_objectref(ptr) {
		ptr = O::_nil();
	}
	ObjectPtr_out(ObjectPtr_var<O> &var)
		: m_objectref(var.out()) {
	}
	ObjectPtr_out(ObjectPtr_out const & out)
		: m_objectref(out.m_objectref) {
	}

	ObjectPtr_out &operator=(ObjectPtr_out &out) {
		m_objectref = out.m_objectref;
		return *this;
	}
	ObjectPtr_out &operator=(ObjectPtr_var<O> &var) {
		m_objectref = O::_duplicate(var.operator O_ptr &());
		return *this;
	}
	ObjectPtr_out &operator=(O_ptr ptr) {
		m_objectref = ptr;
		return *this;
	}
/*
// GCC spews warnings if we include this
	operator O_ptr const &() const {
		return m_objectref;
	}
*/
	operator O_ptr &() {
		return m_objectref;
	}
	operator CORBA_Object *() {
		return static_cast<CORBA_Object*>(&m_objectref);
	}
	operator CORBA_TypeCode *() {
		return static_cast<CORBA_TypeCode*>(&m_objectref);
	}
	O_ptr ptr() const {
		return m_objectref;
	}
	O_ptr operator->() const{
		return m_objectref;
	}

};

} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_OBJECTPTR_OUT_HH
