/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include "test-cpp-stubs.h"
#include <iostream>
#include <fstream>
#include <string>
  
int main(int argc, char *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "orbit-local-orb");

    std::ifstream ior_stream ("test.ior");
    std::string ior;
    ior_stream >> ior;

    {
    CORBA::Object_var obj = orb->string_to_object (ior);  
    Test::TestIface_var master_ptr = Test::TestIface::_narrow (obj);

    {
       CORBA::Any a;
       Test::OctetSeq os;
       os.length(1);
       os[0] = 1;
       a <<= os;
       a >>= os;
    }
    
    if(master_ptr)
    {
      //in:
      {
        CORBA::Any val_in;
        val_in <<= (CORBA::Long)1;
        master_ptr->test_in(val_in);
      }

      //inout:
      {
        CORBA::Any val_inout;
        val_inout <<= (CORBA::Long)1;
        master_ptr->test_inout(val_inout);
        
        CORBA::Long val = 0;
        val_inout >>= val;
        std::cout << "test_inout: val_inout = " << val << std::endl;
      }

      //out:
      {
        CORBA::Any_var val_out;
        master_ptr->test_out(val_out);

        CORBA::Long val = 0;
        *val_out >>= val;
        std::cout << "test_out: val_out = " << val << std::endl;
      }

      //return value:
      {
        CORBA::Any_var val_ret = master_ptr->test_ret();

        CORBA::Long val = 0;
        *val_ret >>= val;
        std::cout << "test_out: val_ret = " << val << std::endl;
      }
    }
    
    }

  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "Exception caught." << std::endl;
  }
  
  return 0;
}
