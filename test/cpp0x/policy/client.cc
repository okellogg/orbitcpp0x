/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */

#include "helloworld-cpp-stubs.h"
#include <iostream>
	
int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage:" << std::endl
				  << "  " << argv[0] << " IOR" << std::endl
				  << std::endl;
		return -1;
	}
	
	try
	{
		// Initialize ORBit
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "orbit-local-orb");

		{
		// Get a reference to the server from the IOR passed on the
		// command line
		CORBA::Object_var obj = orb->string_to_object(argv[1]);
		hellomodule::Hello_var ptr = hellomodule::Hello::_narrow(obj);

		// The argument is a simple string
		const char* message = "Hello server, from client!";

		// The result is stored in a CORBA-aware smartpointer
		std::string reply;

		// Do the actual CORBA call here
		reply = ptr->helloWorld(message);

		// Print reply
		std::cout << "Client: Reply was \"" << reply << "\"" << std::endl;
		}
		
	} catch(const CORBA::Exception& ex) {
		std::cout << "Exception caught. Maybe the server is not running, or the IOR is wrong." << std::endl;
	}
	
	return 0;
}
