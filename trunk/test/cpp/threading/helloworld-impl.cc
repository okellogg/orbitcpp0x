#include "helloworld-impl.h"
#include "server_thread.h"
#include <iostream>


void DualServant::ping (void)
  throw(CORBA::SystemException)
{
  std::cerr << "I was pinged" << std::endl;
}


void DualServant::create_thread (void)
  throw(CORBA::SystemException)
{
    thread = new server_thread;
}


void DualServant::submit_work (test::Worker_ptr server)
  throw(CORBA::SystemException)
{
  std::cerr << "Request to submit work\n" ;

  thread->queue (test::Worker::_duplicate (server));
}

