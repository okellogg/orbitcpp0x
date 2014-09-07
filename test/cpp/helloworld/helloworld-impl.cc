#include "helloworld-impl.h"
#include <iostream>

#if defined (IDL2CPP0X)
std::string
hellomodule::Hello_impl::helloWorld(const std::string& greeting) throw (CORBA::SystemException)
{
  std::cout << "Server: Greeting was \"" << greeting << "\"" << std::endl;
  return std::string("Hello client, from server!");
}
#else
char*
hellomodule::Hello_impl::helloWorld(const char* greeting) throw(CORBA::SystemException)
{
  std::cout << "Server: Greeting was \"" << greeting << "\"" << std::endl;
  return CORBA::string_dup("Hello client, from server!");
}
#endif
