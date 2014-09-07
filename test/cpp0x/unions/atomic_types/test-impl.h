/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t;c-basic-offset: 4 -*- */
#ifndef TEST_IMPL_HH
#define TEST_IMPL_HH

#include "test-cpp-skels.h"
#include <string>

class ITst_impl: public POA_Test::ITst
{
public:
	void fixed_in    (const ::Test::UFixed &un)     throw (CORBA::SystemException);
	void fixed_inout (::Test::UFixed       &un)     throw (CORBA::SystemException);
	void fixed_out   (::Test::UFixed       &un_out) throw (CORBA::SystemException);
	::Test::UFixed fixed_ret ()                     throw (CORBA::SystemException);

	void unfixed_in    (const ::Test::UUnfixed &un)     throw (CORBA::SystemException);
	void unfixed_inout (::Test::UUnfixed       &un)     throw (CORBA::SystemException);
	void unfixed_out   (::Test::UUnfixed_out    un_out) throw (CORBA::SystemException);
	::Test::UUnfixed* unfixed_ret ()                    throw (CORBA::SystemException);
};

#endif // TEST_IMPL_HH
