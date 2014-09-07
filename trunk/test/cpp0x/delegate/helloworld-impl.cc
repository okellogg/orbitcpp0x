#include "helloworld-impl.h"
#include <iostream>

char*
hellomodule::Hello_impl::helloWorld(const char* greeting)
  throw(CORBA::SystemException)
{
  std::cout << "Server: Greeting was \"" << greeting << "\"" << std::endl;

  return CORBA::string_dup("Hello client, from server!");
}


char*
hellomodule::Hello_impl::helloWorld_delegate (hellomodule::Hello_ptr server,
					    const char* greeting)
  throw(CORBA::SystemException)
{
  std::cout << "Send helloWorld greeting onwards ..." << std::endl;

  return server->helloWorld (greeting) ;
}
