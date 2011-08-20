/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t;c-basic-offset: 4 -*- */
#ifndef TEST_IMPL_HH
#define TEST_IMPL_HH

#include "test-cpp-skels.h"
#include <string>

class ITst_impl: public POA_Test::ITst
{
public:
	void un_in    (const ::Test::UTst &un)     throw (CORBA::SystemException);
	void un_inout (::Test::UTst       &un)     throw (CORBA::SystemException);
	void un_out   (::Test::UTst_out    un_out) throw (CORBA::SystemException);
	
	::Test::UTst * un_ret ()                   throw (CORBA::SystemException);
};

#endif // TEST_IMPL_HH
