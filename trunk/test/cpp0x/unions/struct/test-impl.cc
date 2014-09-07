#include "test-impl.h"
#include <iostream>

namespace {
    void print_un (const ::Test::UTst &un)
    {
	std::cerr << "\t_d:\t\t" << un._d () << std::endl;
	switch (un._d ())
	{
	case 0:
	    std::cerr << "\tnum_value:\t" << un.num_value () << std::endl;
	    break;
	case 1:
	    std::cerr << "\tstr_value:\t" << un.str_value () << std::endl;
	    break;
	default:
	    std::cerr << "\tstruct_value.string:\t"
		      << un.struct_value ().str_value () << std::endl;
	    break;
	}
    }
}

void ITst_impl::un_in (const ::Test::UTst &un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::un_in" << std::endl;
    print_un (un);
}

void ITst_impl::un_inout (::Test::UTst &un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::un_inout" << std::endl;
    print_un (un);

    un.str_value ("Foo Bar Baz");
}

void ITst_impl::un_out (::Test::UTst_out un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::un_out" << std::endl;

    ::Test::StTst strct;
    strct.str_value ("String member of StTst");
    strct.num_value (42);
    un.struct_value (strct);
}

::Test::UTst ITst_impl::un_ret ()
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::un_ret" << std::endl;

    ::Test::UTst un;
    //un.bool_value (false);

    return un;
}
