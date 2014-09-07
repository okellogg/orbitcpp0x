/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t;c-basic-offset: 4 -*- */
#ifndef _ORBIT_CPP_IDL_helloworld_IMPL_HH
#define _ORBIT_CPP_IDL_helloworld_IMPL_HH

#include "helloworld-cpp-skels.h"
#include "server_thread.h"


class DualServant :
	public virtual POA_test::Dual {
public:
	void ping () throw(CORBA::SystemException);

	void create_thread ()
		throw(CORBA::SystemException);

	void submit_work (test::Worker_ptr server)
		throw(CORBA::SystemException);

	server_thread* thread;
};


#endif //_ORBIT_CPP_IDL_helloworld_IMPL_HH
