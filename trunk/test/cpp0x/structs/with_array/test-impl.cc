#include "test-impl.h"
#include <iostream>

void IMaster_impl::struct_in (const ::Test::Test_st &struct_in)
    throw (CORBA::SystemException)
{   
    std::cerr << "IMaster_impl::struct_in" << std::endl;

    std::cerr << "\tnum:\t" << struct_in.num() << std::endl;
    std::cerr << "\t#1:\t" << struct_in.text()[0] << std::endl;
    std::cerr << "\t#2:\t" << struct_in.text()[1] << std::endl;
}

::Test::Test_st IMaster_impl::struct_ret ()
    throw (CORBA::SystemException)
{
    std::cerr << "IMaster_impl::struct_ret" << std::endl;

    ::Test::Test_st retval;

    retval.num (42);
    Test::TestArray arr;
    arr[0] = "Text #1";
    arr[1] = "Text #2";
    retval.text (arr);
    
    return retval;
}
