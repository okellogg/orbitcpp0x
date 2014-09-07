/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */

#include "helloworld-cpp-stubs.h"
#include <iostream>
	
int main (int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage:" << std::endl
				  << "  " << argv[0] << " IOR1 IOR2" << std::endl
				  << std::endl;
		return -1;
	}
	
	try
	{
		// Initialize ORBit
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		{
		// Get a reference to the server from IOR1 passed on the
		// command line
		CORBA::Object_var obj1 = orb->string_to_object(argv[1]);
		test::Master_var ptr1 = test::Master::_narrow(obj1);

		CORBA::Object_var obj2 = orb->string_to_object(argv[2]);
		test::Worker_var ptr2 = test::Worker::_narrow(obj2);

		ptr1->create_thread ();
		ptr1->submit_work (ptr2);

		}

		
	} catch(const CORBA::Exception& ex) {
		std::cout << "Exception caught. Maybe the server is not running, or the IOR is wrong." << std::endl;
	}
	
	return 0;
}
