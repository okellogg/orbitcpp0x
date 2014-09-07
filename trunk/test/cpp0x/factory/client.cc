/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */

#include "factory-cpp-stubs.h"
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
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		{
		// Get a reference to the server from the IOR passed on the
		// command line
		CORBA::Object_var obj = orb->string_to_object(argv[1]);
		Factory::Producer_var ptr = Factory::Producer::_narrow(obj);

		const char* producer_id = "bike";


		// Do the actual CORBA call here
		Factory::AbstractService_var as = ptr->produce(producer_id);

		char const* mesg = "pedal";
		as->doit(mesg);
		as->destroy();

		}

		
	} catch(const CORBA::Exception& ex) {
		std::cout << "Exception caught. Maybe the server is not running, or the IOR is wrong." << std::endl;
	}
	
	return 0;
}
