/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t;c-basic-offset: 4 -*- */
#ifndef TEST_IMPL_HH
#define TEST_IMPL_HH

#include "test-cpp-skels.h"
#include <string>

class IMaster_impl: public POA_Test::IMaster
{
public:
	void seq_in (const ::Test::str_seq &seq_in)
		throw (CORBA::SystemException);

	void seq_inout (::Test::str_seq &seq_inout)
		throw (CORBA::SystemException);

	void seq_out (::Test::str_seq_out seq_out)
		throw (CORBA::SystemException);		

	::Test::str_seq* seq_ret ()
		throw (CORBA::SystemException);		
};

#endif // TEST_IMPL_HH
