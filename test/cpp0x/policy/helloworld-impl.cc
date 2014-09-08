#include "helloworld-impl.h"
#include <iostream>

std::string
hellomodule::Hello_impl::helloWorld(const std::string& greeting) throw(CORBA::SystemException)
{
  std::cout << "Server: Greeting was \"" << greeting << "\"" << std::endl;
  return std::string("Hello client, from server!");
}
