/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include "helloworld-impl.h"
#include <iostream>

int main (int argc, char* argv[])
{
  try
  {
 	  // Initialize the CORBA orb
 	  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
	
 	  // Get the root POA
 	  CORBA::Object_var pfobj = orb->resolve_initial_references("RootPOA");

 	  PortableServer::POA_var rootPOA =
         PortableServer::POA::_narrow(pfobj);

	  hellomodule::Locator locator;
	  PortableServer::ServantManager_var serv = locator._this();
	  
	  rootPOA->set_servant_manager(serv);

 	  // Activate the root POA's manager
 	  PortableServer::POAManager_var mgr = rootPOA->the_POAManager();

 	  mgr->activate();

	  std::cerr << "about to enter event loop\n";

 	  // run the server event loop
 	  orb->run();
  }
  catch(const CORBA::Exception& ex)
  {
    std::cout << "Exception caught." << std::endl;
  }

}
