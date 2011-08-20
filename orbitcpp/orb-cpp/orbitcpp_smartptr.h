/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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
 *          Phil Dawes <philipd@users.sourceforge.net>
 *			John K. Luebs <jkluebs@marikarpress.com>
 */




#ifndef ORBITCPP_SMARTPTR_HH
#define ORBITCPP_SMARTPTR_HH


#include <orbitcpp/orb-cpp/orbitcpp_types.h>
#include <orbitcpp/orb-cpp/orbitcpp_tools.h>

#include <orbitcpp/orb-cpp/smartpointers/arrayfixed_forany.h>
#include <orbitcpp/orb-cpp/smartpointers/arrayfixed_var.h>
#include <orbitcpp/orb-cpp/smartpointers/arrayvariable_forany.h>
#include <orbitcpp/orb-cpp/smartpointers/arrayvariable_out.h>
#include <orbitcpp/orb-cpp/smartpointers/arrayvariable_var.h>
#include <orbitcpp/orb-cpp/smartpointers/data_out.h>
#include <orbitcpp/orb-cpp/smartpointers/data_var.h>
#include <orbitcpp/orb-cpp/smartpointers/datavar_out.h>
#include <orbitcpp/orb-cpp/smartpointers/datavar_var.h>
#include <orbitcpp/orb-cpp/smartpointers/objectptr_out.h>
#include <orbitcpp/orb-cpp/smartpointers/objectptr_var.h>
#include <orbitcpp/orb-cpp/smartpointers/sequence_out.h>
#include <orbitcpp/orb-cpp/smartpointers/sequence_var.h>
#include <orbitcpp/orb-cpp/smartpointers/string_out.h>
#include <orbitcpp/orb-cpp/smartpointers/string_out.h>

namespace CORBA {
	class Object;
}


  
namespace CORBA {
	typedef _orbitcpp::String_var<CORBA::Char>			String_var;
	typedef _orbitcpp::String_out<CORBA::Char>			String_out;

	struct String_seq_elem_traits
	{
        void pack_elem (String_var &cpp_value, CORBA_char *&c_value) const {
			c_value = CORBA::string_dup (cpp_value);
		}

        void unpack_elem (String_var &cpp_value, CORBA_char *&c_value) const {
			cpp_value = CORBA::string_dup (c_value);
        }
	};

	typedef _orbitcpp::String_var<CORBA::WChar>			WString_var;
	typedef _orbitcpp::String_out<CORBA::WChar>			WString_out;
}


#define ORBITCPP_MAKEREFTYPES(type) \
	typedef type										*type##_ptr; \
	typedef _orbitcpp::ObjectPtr_var<type>	type##_var; \
	typedef _orbitcpp::ObjectPtr_out<type>	type##_out; \
	typedef type##_ptr									type##Ref;

namespace CORBA { 
	class Environment;
	class Policy;
	class DomainManager;
	class NamedValue;
	class NVList;
	class ExceptionList;
	class ContextList;
	class Request;
	class Context;
	class TypeCode;
	class ORB;
	class Object;


	ORBITCPP_MAKEREFTYPES(Environment)
	ORBITCPP_MAKEREFTYPES(Policy)
	ORBITCPP_MAKEREFTYPES(DomainManager)
	ORBITCPP_MAKEREFTYPES(NamedValue)
	ORBITCPP_MAKEREFTYPES(NVList)
	ORBITCPP_MAKEREFTYPES(ExceptionList)
	ORBITCPP_MAKEREFTYPES(ContextList)
	ORBITCPP_MAKEREFTYPES(Request)
	ORBITCPP_MAKEREFTYPES(Context)
	ORBITCPP_MAKEREFTYPES(TypeCode)
	ORBITCPP_MAKEREFTYPES(ORB)
	ORBITCPP_MAKEREFTYPES(Object)
}




namespace PortableServer {
	class POAManager;
	class AdapterActivator;
	class POA;
	class Current;
	
	ORBITCPP_MAKEREFTYPES(POAManager)
	ORBITCPP_MAKEREFTYPES(AdapterActivator)
	ORBITCPP_MAKEREFTYPES(POA)
	ORBITCPP_MAKEREFTYPES(Current)
}




#endif
