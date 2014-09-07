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
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "orbit-local-orb");

		{
		// Get a reference to the server from IOR1 passed on the
		// command line
		CORBA::Object_var obj1 = orb->string_to_object(argv[1]);
		hellomodule::Hello_var ptr1 = hellomodule::Hello::_narrow(obj1);

		CORBA::Object_var obj2 = orb->string_to_object(argv[2]);
		hellomodule::Hello_var ptr2 = hellomodule::Hello::_narrow(obj2);

		// The result is stored in a CORBA-aware smartpointer
		CORBA::String_var reply;

		// Do the actual CORBA call here
		reply = ptr1->helloWorld ("Hello world to server #1");

		// Print reply
		std::cout << "Client1: Reply was \"" << reply << "\"" << std::endl;

		// Do the actual CORBA call here
		reply = ptr2->helloWorld("Hello world to server #2");

		// Print reply
		std::cout << "Client2: Reply was \"" << reply << "\"" << std::endl;

		reply = ptr1->helloWorld_delegate (ptr2, 
										 "Hello world to server #2 via #1");
		}

		
	} catch(const CORBA::Exception& ex) {
		std::cout << "Exception caught. Maybe the server is not running, or the IOR is wrong." << std::endl;
	}
	
	return 0;
}
