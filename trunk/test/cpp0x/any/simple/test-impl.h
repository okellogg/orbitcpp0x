/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t;c-basic-offset: 4 -*- */
#ifndef _ORBIT_CPP_IDL_helloworld_IMPL_HH
#define _ORBIT_CPP_IDL_helloworld_IMPL_HH

#include "test-cpp-skels.h"

namespace Test
{

class TestIface_impl: public virtual POA_Test::TestIface
{
public:
  virtual void test_in (const ::CORBA::Any& val_in)
		throw (CORBA::SystemException);
	virtual void test_inout (::CORBA::Any& val_inout)
		throw (CORBA::SystemException);
	virtual void test_out (::CORBA::Any_out val_out)
		throw (CORBA::SystemException);
	virtual ::CORBA::Any* test_ret ()
		throw (CORBA::SystemException);
};

} // namespace Test


#endif //_ORBIT_CPP_IDL_helloworld_IMPL_HH
