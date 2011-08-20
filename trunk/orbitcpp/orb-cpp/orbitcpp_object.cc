/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 1998 Phil Dawes
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Author: Phil Dawes <philipd@users.sourceforge.co.uk>
 *			Andreas Kloeckner <ak@ixion.net>
 *
 */




#include "orbitcpp/orb-cpp/orbitcpp_object.h"
#include "orbitcpp/orb-cpp/orbitcpp_exception.h"

#include <iostream>



using namespace _orbitcpp;



CORBA_Object& CORBA::Object::_orbitcpp_cobj()
{
	return m_target;
}

CORBA_Object
_orbitcpp::cobj_guarded(CORBA::Object_ptr obj)
{
	return (obj != 0 ? obj->_orbitcpp_cobj() : CORBA_OBJECT_NIL);
}

CORBA_Object
_orbitcpp::duplicate_cobj_guarded(CORBA::Object_ptr obj)
{
	if (obj == 0) return CORBA_OBJECT_NIL;
	return duplicate_guarded(obj->_orbitcpp_cobj());
}


CORBA::Object::Object()
{
}

CORBA::Object::Object(CORBA_Object cobject, bool take_copy /* = true */)
{
	if (take_copy)
	{
		::_orbitcpp::CEnvironment ev;
		cobject = CORBA_Object_duplicate(cobject, ev._orbitcpp_cobj());
	}

	m_target = cobject;
}

CORBA::Object::~Object()
{
	CEnvironment ev;
	CORBA_Object_release(_orbitcpp_cobj(), ev._orbitcpp_cobj());
	ev.propagate_sysex();
}

CORBA::Object* CORBA::Object::_orbitcpp_wrap(CORBA_Object cobject, bool take_copy /* = false */)
{
	return new CORBA::Object (cobject, take_copy);
}

CORBA::Object_ptr CORBA::Object::_duplicate(Object_ptr o)
{
	if (o == CORBA_OBJECT_NIL)
		return CORBA_OBJECT_NIL;
	
	return new CORBA::Object(o->_orbitcpp_cobj());
}

CORBA::Object_ptr CORBA::Object::_narrow(Object_ptr o)
{
	return _duplicate(o);
}

CORBA::Object_ptr CORBA::Object::_nil() {
	return CORBA_OBJECT_NIL;
}


CORBA::Boolean 
CORBA::Object::_is_a(CORBA::RepositoryId const repoid) {
	_orbitcpp::CEnvironment ev;
	Boolean result = CORBA_Object_is_a(_orbitcpp_cobj(), repoid, ev._orbitcpp_cobj());
	ev.propagate_sysex();
	return result;
}




CORBA::Boolean 
CORBA::Object::_non_existent() {
	_orbitcpp::CEnvironment ev;
	Boolean result = CORBA_Object_non_existent(_orbitcpp_cobj(), ev._orbitcpp_cobj());
	ev.propagate_sysex();
	return result;
}




CORBA::Boolean 
CORBA::Object::_is_equivalent(::CORBA::Object_ptr other_object) {
	_orbitcpp::CEnvironment ev;
	Boolean result = CORBA_Object_is_equivalent(_orbitcpp_cobj(), other_object->_orbitcpp_cobj(), ev._orbitcpp_cobj());
	ev.propagate_sysex();
	return result;
}




CORBA::ULong 
CORBA::Object::_hash(::CORBA::ULong maximum) {
	_orbitcpp::CEnvironment ev;
	ULong result = CORBA_Object_hash(_orbitcpp_cobj(), maximum, ev._orbitcpp_cobj());
	ev.propagate_sysex();
	return result;
}




CORBA::Boolean
CORBA::is_nil(Object_ptr o) {
	if (o == CORBA_OBJECT_NIL) return TRUE;
	_orbitcpp::CEnvironment ev;
	Boolean result = CORBA_Object_is_nil(o->_orbitcpp_cobj(), ev._orbitcpp_cobj());
	ev.propagate_sysex();
	return result;
}
