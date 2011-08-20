/* -*- Mode: C++; indent-tabs-mode: t -*- */

/*
 *	ORBit-C++: C++ bindings for ORBit.
 *
 *	Copyright (C) 1998-2000 Phil Dawes
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
 *	Author: Phil Dawes <philipd@users.sourceforge.net>
 *			Andreas Kloeckner <ak@ixion.net>
 *
 */




#ifndef ORBITCPP_EXCEPTION_HH
#define ORBITCPP_EXCEPTION_HH


#include <string.h>
#include <typeinfo>


#include "orbitcpp/orb-cpp/orbitcpp_types.h"
#include "orbitcpp/orb-cpp/orbitcpp_tools.h"



// this is for digital unix 3.2 (possibly others)
#ifdef minor
#undef minor
#endif




#define ORBITCPP_SYSEXCEPTION_FACTORY(name,quotedrepoid) \
	class name : public SystemException { \
	public: \
		name() { \
		} \
		name(name const &src) \
			: SystemException(src) { \
		} \
		name(ULong minor,CompletionStatus status) \
			: SystemException(minor,status)	 { \
		} \
		static name* _narrow(Exception *ex) { \
			return dynamic_cast<name*>(ex); \
		} \
		virtual void _raise() { \
			throw *this; \
		} \
		virtual void _orbitcpp_set(CORBA_Environment *ev) { \
			/* *** FIXME possibly ORBit-specific, and possibly wrong*/ \
			CORBA_exception_set_system(ev,ex_CORBA_##name, \
			(CORBA_completion_status) m_completed); \
		} \
		virtual char const *_orbitcpp_get_repoid() const { \
			return quotedrepoid; \
		} \
	};




// obviously this only produces memberless exceptions.
#define ORBITCPP_USEREXCEPTION_FACTORY(name,repoid) \
	class name : public ::CORBA::UserException { \
	public: \
		name() { \
		} \
		name(name const &src) \
			: CORBA::UserException(src) { \
		} \
		static name* _narrow(Exception *ex) { \
			return dynamic_cast<name*>(ex); \
		} \
		virtual void _raise() { \
			throw *this; \
		} \
		virtual void _orbitcpp_set(CORBA_Environment *ev) { \
			CORBA_exception_set(ev, \
				CORBA_USER_EXCEPTION,repoid,NULL); \
		} \
		virtual char const *_orbitcpp_get_repoid() const { \
			return repoid; \
		} \
	};




namespace CORBA {
	class Exception {
	public: 
		virtual ~Exception() {
		}
		virtual void _raise() = 0;
		virtual CORBA_exception_type _orbitcpp_major() = 0;

		virtual void _orbitcpp_set(CORBA_Environment *ev) = 0;
		
		virtual char const *_orbitcpp_get_repoid() const =0;
	
	  
	protected:
		Exception() {
		}
	};

	enum CompletionStatus { COMPLETED_YES, COMPLETED_NO,COMPLETED_MAYBE }; 
  
	class SystemException : public Exception {
	public: 
		SystemException()
			: m_minor(0),m_completed(COMPLETED_NO) {
		}
		SystemException(ULong minor, CompletionStatus status)
			: m_minor(minor),m_completed(status) {
		}
		~SystemException() {
		}

		CORBA_exception_type _orbitcpp_major() {
			return CORBA_SYSTEM_EXCEPTION;
		}
		ULong minor() const {
			return m_minor;
		}
		void minor(ULong minor) {
			m_minor = minor;
		}

		CompletionStatus completed() const {
			return m_completed;
		}
		void completed(CompletionStatus st) {
			m_completed = st;
		}

		static SystemException* _narrow(Exception *ex) {
			return dynamic_cast<SystemException*>(ex);
		}


	  
	protected:
		ULong				m_minor;
		CompletionStatus	m_completed;
	};
	
	#define P(name,quotedname) ORBITCPP_SYSEXCEPTION_FACTORY(name,quotedname)
	#include "orbitcpp/orb-cpp/orbitcpp_exceptionlist.h"
	#undef P

	class UserException : public Exception {
	public: 
		UserException() {
		}
		~UserException() {
		}
  
		virtual CORBA_exception_type _orbitcpp_major() {
			return CORBA_USER_EXCEPTION;
		}

		static UserException* _narrow(Exception *ex) {
			return dynamic_cast<UserException*>(ex);;
		}


	};

	class UnknownUserException : public UserException {
	public: 
		// *** FIXME add this when any support is here
		// Any &exception();

		UnknownUserException() {
		}
		virtual void _raise() {
			throw *this;
		}
		static UnknownUserException* _narrow(Exception *ex) {
			return dynamic_cast<UnknownUserException*>(ex);
		}
		virtual void _orbitcpp_set(CORBA_Environment*) {
			::_orbitcpp::error("cannot set UnknownUserException.");
		}
		virtual char const *_orbitcpp_get_repoid() const
		{
			// FIXME: need to get at real repoid
			return "UnknownUserException";
		}
	};

}
  
  
  

namespace _orbitcpp
{
	template <class X, class CX>
	void cxx_rethrow(char const* repo_id, void* value, char const* ex_id)
	{
		if (strcmp(repo_id, ex_id) != 0)
			return;

		X ex;
		ex._orbitcpp_unpack(*reinterpret_cast<CX*>(value));
		throw ex;
	}

	class CEnvironment
	{
	  // ha! this is not hungarian notation, it is "C Environment" - Andy
	  // - Yeah Yeah! very funny ;-) - Phil 
	
	public: 
		CEnvironment();
		~CEnvironment();
		
		CORBA_Environment* _orbitcpp_cobj()
		{ return &m_env; }

		CORBA_Environment *operator->() {
			return &m_env;
		}
		CORBA_Environment *get_environment() {
			return &m_env;
		}
  
		void clear();
		inline void propagate_sysex() {
			if (m_env._major == CORBA_SYSTEM_EXCEPTION)
				propagate_sysex_guts();
		}

		inline void propagate_poaex() {
			propagate_sysex();
			if (m_env._major == CORBA_USER_EXCEPTION)
				propagate_poaex_guts();
		}

	private:
		void propagate_sysex_guts();
		void propagate_poaex_guts();

		CORBA_Environment m_env;
	};

} //namespace




#undef ORBITCPP_SYSEXCEPTION_FACTORY
#endif // ORBITCPP_EXCEPTION
