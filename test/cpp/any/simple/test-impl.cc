#include "test-impl.h"
#include <iostream>

namespace Test
{
  
void TestIface_impl::test_in(const ::CORBA::Any& val_in) throw (CORBA::SystemException)
{
  CORBA::Long val = 0;
  val_in >>= val;
  
  std::cout << "Server: test_in: received val_in=" << val << std::endl;
}

void TestIface_impl::test_inout(::CORBA::Any& val_inout) throw (CORBA::SystemException)
{
  CORBA::Long val = 0;
  val_inout >>= val;
  
  std::cout << "Server: test_inout: received val_inout=" << val << std::endl;
}

void TestIface_impl::test_out(::CORBA::Any_out val_out) throw (CORBA::SystemException)
{
  val_out = new CORBA::Any;  
  *val_out <<= 2;

  //CORBA::Any_var temp = new CORBA::Any;
  //*temp <<= 2;
  //val_out = temp._retn();
}

::CORBA::Any* TestIface_impl::test_ret() throw (CORBA::SystemException)
{
  CORBA::Any* result = new CORBA::Any();
  *result <<= 2;
  return result;
}

} // namespace Test
