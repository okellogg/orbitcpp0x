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

#ifndef __ORBITCPP_SMARTPTR_OBJECTPTR_VAR_HH
#define __ORBITCPP_SMARTPTR_OBJECTPTR_VAR_HH

#include "orbitcpp/orb-cpp/orbitcpp_object.h"

namespace CORBA {
class Object;
void release(Object*);
class TypeCode;
void release(TypeCode*);
class ORB;
void release(ORB*);
}

namespace _orbitcpp
{

// according to spec 20.3.7
class Dummy_var { };

template<class O>
class ObjectPtr_var : public Dummy_var {
public:
	typedef O* O_ptr;
private:
	O_ptr	m_objectref;
  
public:
	ObjectPtr_var()
		: m_objectref(O::_nil()) {
	}
	ObjectPtr_var(O_ptr const ptr)
		: m_objectref(ptr) {
	}

	ObjectPtr_var(ObjectPtr_var const &objectref)
		: m_objectref(O::_duplicate(objectref.m_objectref)) {
	}

	~ObjectPtr_var() {
		free();
	}

	ObjectPtr_var &operator=(O_ptr const ptr) {
		reset(ptr);
		return *this;
	}
	ObjectPtr_var &operator=(ObjectPtr_var const &objectref_var) {
		if (this == &objectref_var) return *this;
		reset(O::_duplicate(const_cast<ObjectPtr_var &>(objectref_var)));
		return *this;
	}

	O_ptr in() const {
		return m_objectref;
	}
	O_ptr &inout() {
		return m_objectref;
	}
	O_ptr &out() {
		reset(O::_nil());
		return m_objectref;
	}
	O_ptr _retn() {
		O_ptr old = m_objectref;
		m_objectref = O::_nil();
		return old;
	}

	operator O_ptr () const {
		return m_objectref;
	}

	O_ptr operator->() const {
		return m_objectref;
	}


private:

	void free() {
		if (m_objectref != O::_nil ())
			::CORBA::release(m_objectref);
		m_objectref = O::_nil();
	}


	void reset(O_ptr ptr) {
		free();
		m_objectref = ptr;
	}
	void operator=(Dummy_var const &oops);
	// not to be implemented
};
  
} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_OBJECTPTR_VAR_HH
