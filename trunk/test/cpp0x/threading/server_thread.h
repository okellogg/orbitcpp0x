#ifndef _SERVER_THREAD_H
#define _SERVER_THREAD_H

#include <queue>
/* Yes, it would be much nicer to use glibmm. But it is probably best
 * not make orbitcpp depend on it.
 */
#include <glib.h>

#include "helloworld-cpp-stubs.h"

class server_thread {
	GThread* thread_;
	GMutex* mutex_;
	GCond* cond_;
	std::queue<test::Worker_var> work_;

	static gpointer thread_consumer(gpointer);
	void consumer();

	// disallowed
	server_thread(server_thread const&);
	server_thread& operator=(server_thread const&);

public:
	server_thread ();
	~server_thread ();
	void queue (test::Worker_var work);
};

#endif
