/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2006 Eugene Sizikov
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
 *  Author: Eugene Sizikov <eugene_beast@mail.ru>
 *
 *  Description: Policy Implementation
 */




#include "orbitcpp/orb-cpp/orbitcpp_policy.h"


using namespace _orbitcpp;

CORBA::Policy::Policy ()
: CORBA::Object () {
}


CORBA::Policy::Policy (CORBA_Policy cobject)
: CORBA::Object ((CORBA_Object)cobject, false) {
}

CORBA::Policy_ptr
CORBA::Policy::_duplicate (CORBA::Policy_ptr _obj) {
	if (_obj == 0) return 0;

	CEnvironment ev;
	CORBA_Policy newpolicy =
		(CORBA_Policy)
	    CORBA_Object_duplicate (
			(CORBA_Object)_obj->_orbitcpp_cobj (), ev._orbitcpp_cobj ());
	ev.propagate_sysex ();

	return new CORBA::Policy (newpolicy);
}


CORBA::Policy_ptr
CORBA::Policy::_narrow (CORBA::Object_ptr obj) {
	return _duplicate (static_cast<CORBA::Policy_ptr > (obj));
}


CORBA::Policy_ptr
CORBA::Policy::_nil () {
	return 0;
}


CORBA::PolicyType
CORBA::Policy::policy_type () {
	CEnvironment ev;
	CORBA_PolicyType policytype =
		CORBA_Policy__get_policy_type (
			(CORBA_Policy)_orbitcpp_cobj (), ev._orbitcpp_cobj ());
	ev.propagate_sysex ();

	return policytype;
}


CORBA::Policy_ptr
CORBA::Policy::copy () {
	CEnvironment ev;
	CORBA_Policy newpolicy =
		CORBA_Policy_copy((CORBA_Policy)_orbitcpp_cobj (), ev._orbitcpp_cobj ());
	ev.propagate_sysex ();

	return new CORBA::Policy (newpolicy);
}


void
CORBA::Policy::destroy () {
	CEnvironment ev;
	CORBA_Policy_destroy((CORBA_Policy)_orbitcpp_cobj (), ev._orbitcpp_cobj ());
	ev.propagate_sysex ();
}

