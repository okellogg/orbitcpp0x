/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t;c-basic-offset: 4 -*- */
#ifndef _ORBIT_CPP_IDL_helloworld_IMPL_HH
#define _ORBIT_CPP_IDL_helloworld_IMPL_HH

#include "helloworld-cpp-skels.h"


namespace hellomodule
{

//Inherit from abstract Skeleton:
class Hello_impl : public POA_hellomodule::Hello
{
public:
	//Implement pure virtual method:
	std::string helloWorld(const std::string& greeting) throw (CORBA::SystemException);
};

}; // namespace hellomodule


#endif //_ORBIT_CPP_IDL_helloworld_IMPL_HH
