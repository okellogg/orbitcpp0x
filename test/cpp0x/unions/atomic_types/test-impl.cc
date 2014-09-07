#include "test-impl.h"
#include <iostream>

namespace {
    void print_fixed (const ::Test::UFixed &un)
    {
	std::cerr << "\t_d:\t\t" << un._d () << std::endl;
	switch (un._d ())
	{
	case 0:
	    std::cerr << "\tnum_value:\t" << un.num_value () << std::endl;
	    break;
	default:
	    std::cerr << "\tbool_value:\t" << (un.bool_value () ? "True" : "False") << std::endl;
	}
    }

    void print_unfixed (const ::Test::UUnfixed &un)
    {
	std::cerr << "\t_d:\t\t" << un._d () << std::endl;
	switch (un._d ())
	{
	case 0:
	    std::cerr << "\tnum_value:\t" << un.num_value () << std::endl;
	    break;
	default:
	    std::cerr << "\tstr_value:\t" << un.str_value () << std::endl;
	}
    }
}

void ITst_impl::fixed_in (const ::Test::UFixed &un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::fixed_in" << std::endl;
    print_fixed (un);
}

void ITst_impl::fixed_inout (::Test::UFixed &un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::fixed_inout" << std::endl;
    print_fixed (un);

    un.bool_value (true);
}

void ITst_impl::fixed_out (::Test::UFixed &un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::fixed_out" << std::endl;

    un.num_value (42);
}

::Test::UFixed ITst_impl::fixed_ret ()
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::fixed_ret" << std::endl;

    ::Test::UFixed un;
    //un.bool_value (false);

    return un;
}



void ITst_impl::unfixed_in (const ::Test::UUnfixed &un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::unfixed_in" << std::endl;
    print_unfixed (un);
}

void ITst_impl::unfixed_inout (::Test::UUnfixed &un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::unfixed_inout" << std::endl;
    print_unfixed (un);

    un.str_value ("Hello client -- via inout argument");
}

void ITst_impl::unfixed_out (::Test::UUnfixed_out un)
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::unfixed_inout" << std::endl;

    un.num_value (42);
}

::Test::UUnfixed ITst_impl::unfixed_ret ()
    throw (CORBA::SystemException)
{
    std::cerr << "ITst_impl::unfixed_ret" << std::endl;

    ::Test::UUnfixed un;
    un.str_value ("Hello client -- via return value");

    return un;
}
