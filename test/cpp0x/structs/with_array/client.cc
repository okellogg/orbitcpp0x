/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include "test-cpp-stubs.h"
#include <iostream>
#include <fstream>
#include <string>
	
int main (int argc, char *argv[])
{
	CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "orbit-local-orb");
	
	std::ifstream ior_stream ("test.ior");
	std::string ior;
	ior_stream >> ior;
	
	CORBA::Object_var obj = orb->string_to_object (ior);
	Test::IMaster_var master_ptr = Test::IMaster::_narrow (obj);

	///////////////////////////////////
	// -> IN
	Test::Test_st in_struct;

	in_struct.num (10);
	Test::TestArray arr;
	arr[0] = "IN string #1";
	arr[1] = "IN string #2";
	in_struct.text (arr);
	
	master_ptr->struct_in (in_struct);
	
	
	///////////////////////////////////
	// <- RET
	Test::Test_st struct_ret = master_ptr->struct_ret ();
	
	std::cerr << "Client::RET: num\t" << struct_ret.num() << std::endl;
	std::cerr << "Client::RET: #1\t\t" << struct_ret.text()[0] << std::endl;
	std::cerr << "Client::RET: #1\t\t" << struct_ret.text()[1] << std::endl;


	return 0;
}
