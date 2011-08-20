/* -*- Mode: C++; indent-tabs-mode: t -*- */

/*
 *	ORBit-C++: C++ bindings for ORBit.
 *
 *	Copyright (C) 2000 Andreas Kloeckner
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
 *	Author: Andreas Kloeckner <ak@ixion.net>
 *
 */




#ifndef ORBITCPP_TOOLS_HH
#define ORBITCPP_TOOLS_HH


#include <orbit/orbit.h>

#include "orbitcpp/orb-cpp/orbitcpp_config.h"
#include "orbitcpp/orb-cpp/orbitcpp_types.h"





// tool functions -------------------------------------------------------------
namespace _orbitcpp {


	// These functions are for a nasty memory hack.
	// Basically, sequences require the operator new[] is called so
	// that nested types have their constructors called.
	// Unfortunately operator new[] overwrites the orbit mem info struct,
	// so it must be stored before, and then put back afterwards.	
	void save_meminfo(void *mem);
	void restore_meminfo(void *mem);


	// If you don't want CORBA_free() to attempt to free mem pointed
	// to by your pointer (ie. if the memory has been freed already),
	// then this function will make it point to a block of memory preceeded
	// by HOWNONE. ( see ORBit/doc/orbit-mem2.txt)
	void point_to_memhow_none(gpointer *ptr);



  
	CORBA_Object duplicate_guarded(CORBA_Object obj);

	void release_guarded(CORBA_Object obj);

	// convenience method so that TC pseudo objects can be treated as objects
	inline CORBA_TypeCode duplicate_guarded(CORBA_TypeCode tc) {return (CORBA_TypeCode) duplicate_guarded((CORBA_Object)tc);}  
	inline void release_guarded(CORBA_TypeCode tc) {release_guarded((CORBA_Object)tc);}  

  
	void error(const char *text);
}




#endif
