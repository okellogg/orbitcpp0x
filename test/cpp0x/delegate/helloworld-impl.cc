#include "helloworld-impl.h"
#include <iostream>

std::string
hellomodule::Hello_impl::helloWorld(const std::string& greeting)
  throw(CORBA::SystemException)
{
  std::cout << "Server: Greeting was \"" << greeting << "\"" << std::endl;

  return std::string("Hello client, from server!");
}


std::string
hellomodule::Hello_impl::helloWorld_delegate (hellomodule::Hello_ptr server,
					    const std::string& greeting)
  throw(CORBA::SystemException)
{
  std::cout << "Send helloWorld greeting onwards ..." << std::endl;

  return server->helloWorld (greeting) ;
}
