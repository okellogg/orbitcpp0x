/* -*- Mode: C++; c-basic-offset: 4 -*- */

#include "test-cpp-stubs.h"
#include <iostream>
#include <fstream>
#include <string>
	
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
			std::cerr << "\tstruct_value:\tstring\t"
					  << un.struct_value ().str_value () << std::endl;
			std::cerr << "\tstruct_value:\tnum\t"
					  << un.struct_value ().num_value () << std::endl;
			break;
		}
    }
}

void test_un (Test::ITst_ptr &test_ptr)
{
	///////////////////////////////////
	// -> IN
	Test::UTst un_in;
	un_in.num_value (5);
	std::cerr << "Client::un_in" << std::endl;	
	test_ptr->un_in (un_in);

	///////////////////////////////////
	// <-> INOUT
	std::cerr << "Client::un_inout" << std::endl;
	test_ptr->un_inout (un_in);
	print_un (un_in);

	///////////////////////////////////
	// <- OUT
	Test::UTst un_out;
	std::cerr << "Client::un_out" << std::endl;
	test_ptr->un_out (un_out);
	print_un (un_out);
	
	///////////////////////////////////
	// <- RET
	std::cerr << "Client::un_ret" << std::endl;
	Test::UTst un_ret = test_ptr->un_ret ();
	print_un (un_ret);
}

int main(int argc, char *argv[])
{
	CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv);
	
	std::ifstream ior_stream ("test.ior");
	std::string ior;
	ior_stream >> ior;
	
	CORBA::Object_var obj = orb->string_to_object (ior);
	Test::ITst_ptr test_ptr = Test::ITst::_narrow (obj);

	test_un (test_ptr);
	
	return 0;
}
