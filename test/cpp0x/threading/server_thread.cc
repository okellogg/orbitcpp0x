#include <iostream>

#include "server_thread.h"

server_thread::server_thread()
{
	if (!g_thread_supported())
		g_thread_init(0);

	mutex_ = g_mutex_new();
	cond_ = g_cond_new();

	GError* err = 0;
	thread_ = g_thread_create(thread_consumer, this, TRUE, &err);
	g_assert(err == 0);
}

server_thread::~server_thread()
{
	g_thread_join(thread_);
	g_cond_free(cond_);
	g_mutex_free(mutex_);
}


gpointer
server_thread::thread_consumer(gpointer data)
{
	reinterpret_cast<server_thread*>(data)->consumer();
	return data;
}

void server_thread::consumer()
{
	for (;;) {
		test::Worker_var thread_work;

		g_mutex_lock(mutex_);

		while (work_.empty ())
			g_cond_wait(cond_, mutex_);

		thread_work = work_.front ();
		work_.pop ();

		g_mutex_unlock(mutex_);

		std::cerr << "Thread picked up work\n" ;
		g_assert(!CORBA::is_nil(thread_work));
		g_assert(!thread_work->_non_existent());
		// XXX: The call that kills things ...
		thread_work->ping ();
		std::cerr << "Thread finished work.\n";
	}
}


void server_thread::queue (test::Worker_var work)
{
	g_mutex_lock(mutex_);
	work_.push(work);
	g_cond_signal(cond_);
	g_mutex_unlock(mutex_);
}
