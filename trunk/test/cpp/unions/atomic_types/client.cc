/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */

#include "test-cpp-stubs.h"
#include <iostream>
#include <fstream>
#include <string>
	
namespace {
    void print_un (const ::Test::UFixed &un)
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

    void print_un (const ::Test::UUnfixed &un)
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

void test_fixed (Test::ITst_ptr &test_ptr)
{
	///////////////////////////////////
	// -> IN
	Test::UFixed fixed_in;
	fixed_in.num_value (5);
	std::cerr << "Client::fixed_in" << std::endl;	
	test_ptr->fixed_in (fixed_in);

	///////////////////////////////////
	// <-> INOUT
	std::cerr << "Client::fixed_inout" << std::endl;
	test_ptr->fixed_inout (fixed_in);
	print_un (fixed_in);

	///////////////////////////////////
	// <- OUT
	Test::UFixed fixed_out;
	std::cerr << "Client::fixed_out" << std::endl;
	test_ptr->fixed_out (fixed_out);
	print_un (fixed_out);
	
	///////////////////////////////////
	// <-> INOUT
	std::cerr << "Client::fixed_inout" << std::endl;
	test_ptr->fixed_inout (fixed_in);
	print_un (fixed_in);

	///////////////////////////////////
	// <- RET
	std::cerr << "Client::fixed_ret" << std::endl;
	Test::UFixed fixed_ret = test_ptr->fixed_ret ();
	print_un (fixed_ret);
}

void test_unfixed (Test::ITst_ptr &test_ptr)
{
	///////////////////////////////////
	// -> IN
	Test::UUnfixed unfixed_in;
	unfixed_in.num_value (5);
	std::cerr << "Client::unfixed_in" << std::endl;	
	test_ptr->unfixed_in (unfixed_in);

	///////////////////////////////////
	// <-> INOUT
	std::cerr << "Client::unfixed_inout" << std::endl;
	test_ptr->unfixed_inout (unfixed_in);
	print_un (unfixed_in);

	///////////////////////////////////
	// <- OUT
	Test::UUnfixed_var unfixed_out;
	std::cerr << "Client::unfixed_out" << std::endl;
	test_ptr->unfixed_out (unfixed_out);
	print_un (unfixed_out);
	
	///////////////////////////////////
	// <-> INOUT
	std::cerr << "Client::unfixed_inout" << std::endl;
	test_ptr->unfixed_inout (unfixed_in);
	print_un (unfixed_in);

	///////////////////////////////////
	// <- RET
	std::cerr << "Client::fixed_ret" << std::endl;
	Test::UUnfixed_var unfixed_ret = test_ptr->unfixed_ret ();
	print_un (unfixed_ret);
}

int main(int argc, char *argv[])
{
	CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv);
	
	std::ifstream ior_stream ("test.ior");
	std::string ior;
	ior_stream >> ior;
	
	CORBA::Object_var obj = orb->string_to_object (ior);
	Test::ITst_ptr test_ptr = Test::ITst::_narrow (obj);

	test_fixed   (test_ptr);
	test_unfixed (test_ptr);
	
	return 0;
}
