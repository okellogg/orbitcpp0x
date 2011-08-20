/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include "helloworld-impl.h"
#include <iostream>

int main (int argc, char* argv[])
{
  try
  {
 	  // Initialize the CORBA orb
 	  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv /*, "orbit-io-thread"*/);
	
 	  // Get the root POA
 	  CORBA::Object_var pfobj = orb->resolve_initial_references("RootPOA");

 	  PortableServer::POA_var rootPOA =
         PortableServer::POA::_narrow(pfobj);

 	  // Activate the root POA's manager
 	  PortableServer::POAManager_var mgr = rootPOA->the_POAManager();

 	  mgr->activate();

 	  // Create a Servant and explicitly create a CORBA object
 	  DualServant servant;
 	  CORBA::Object_var object = servant._this();

 	  // Here we get the IOR for the Hello server object.
 	  // Our "client" will use the IOR to find the object to connect to
 	  CORBA::String_var ref = orb->object_to_string( object );

 	  // print out the IOR
 	  std::cout << ref << std::endl;

 	  // run the server event loop
 	  orb->run();

	  std::cout << "exiting normally\n";

	  return 0;
  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "CORBA Exception caught." << std::endl;
  }
  catch (...) {
    std::cerr << "Exception caught." << std::endl;
  }

  return 1;
}
