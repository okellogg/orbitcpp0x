/* -*- Mode: C++; c-basic-offset: 4 -*- */

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
 *  Author: Phil Dawes <philipd@parallax.co.uk>
 *          Ron Garcia <rgarcia4@nd.edu>
 */




#include <iostream>
#define ORBIT2_INTERNAL_API
#include "orbit/orb-core/orbit-object.h"
#undef ORBIT2_INTERNAL_API
#include "orbitcpp/orb-cpp/orbitcpp_object.h"
#include "orbitcpp/orb-cpp/orbitcpp_orb.h"
#include "orbitcpp/orb-cpp/orbitcpp_exception.h"




using namespace _orbitcpp;



// public functions -----------------------------------------------------------

CORBA::ORB::ORB(CORBA_ORB cobject)
{
    m_target = cobject;
}

CORBA::ORB_ptr CORBA::ORB_init (int &argc, char **argv, const char *orb_identifier) {
    CEnvironment ev;
    CORBA_ORB o = CORBA_ORB_init (&argc, argv, const_cast<char*> (orb_identifier),
				  ev._orbitcpp_cobj ());
    ev.propagate_sysex ();

    return new ORB (o);
}


CORBA::ORB_ptr
CORBA::ORB::_duplicate (ORB_ptr orb) {
    if (orb == CORBA_OBJECT_NIL) return CORBA_OBJECT_NIL;
	
    return _orbitcpp_new_orb();
}


void
CORBA::release (ORB_ptr orb) {
    delete orb;
}




// ORB ------------------------------------------------------------------------
CORBA::ORB::~ORB ()
{
	try {
		destroy();
	} catch (...) {
		// too dangerous to allow throwing in destructor
	}
}

CORBA::Object_ptr
CORBA::ORB::string_to_object (const char* str)
{
    CEnvironment ev;
    CORBA_Object o = CORBA_ORB_string_to_object (m_target, const_cast<char*> (str),
						 ev._orbitcpp_cobj ());
    ev.propagate_sysex ();
    
    return new CORBA::Object (o, false);
}

CORBA::Object_ptr
CORBA::ORB::string_to_object (const std::string &str)
{
    return string_to_object (str.c_str ());
}


char *
CORBA::ORB::object_to_string (Object_ptr obj) {
    CEnvironment ev;
    char *str = CORBA_ORB_object_to_string (m_target, obj->_orbitcpp_cobj (),
					    ev._orbitcpp_cobj ());
    ev.propagate_sysex ();
    return str;
}




CORBA::Object_ptr
CORBA::ORB::resolve_initial_references (const char* str) {
    CEnvironment ev;
    CORBA_Object obj = CORBA_ORB_resolve_initial_references (m_target, const_cast<char*> (str),
							     ev._orbitcpp_cobj ());
    ev.propagate_sysex ();
	
    return new CORBA::Object (obj, false);
}




CORBA::Boolean 
CORBA::ORB::work_pending () {
    CEnvironment ev;
    Boolean result = CORBA_ORB_work_pending (m_target, ev._orbitcpp_cobj ());
    ev.propagate_sysex ();
    return result;
}
  
  
  
  
void 
CORBA::ORB::perform_work () {
    CEnvironment ev;
    CORBA_ORB_perform_work (m_target, ev._orbitcpp_cobj ());
    ev.propagate_sysex ();
}




void 
CORBA::ORB::shutdown(CORBA::Boolean wait_for_completion) {
	CEnvironment ev;
	CORBA_ORB_shutdown(m_target, wait_for_completion, ev._orbitcpp_cobj());
	ev.propagate_sysex();
}

void 
CORBA::ORB::destroy() {
	CEnvironment ev;
	CORBA_ORB_destroy(m_target, ev._orbitcpp_cobj());
	ev.propagate_sysex();
}



void CORBA::ORB::run() {
	CEnvironment ev;
	CORBA_ORB_run(m_target, ev._orbitcpp_cobj());
	ev.propagate_sysex();
}

CORBA::ORB_ptr
CORBA::ORB::_orbitcpp_new_orb()
{
	// orbit caches the orb, so there's no point in us doing the same
	int argc = 0;
	char* argv = 0;
	// can't check that an orb has already been created,
	// just hope for best
	return CORBA::ORB_init(argc, &argv);
}
