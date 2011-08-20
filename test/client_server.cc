#include <sstream>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "client_server.h"

namespace tut
{
	client_server_return_type
	client_server (const std::string& server_command, const std::string& client_command)
	{
		const int buf_size = 8192;
		std::auto_ptr<char > ior (new char [buf_size]);
		
		FILE *server = popen (server_command.c_str (), "r");
		if (!server)
			throw std::runtime_error (strerror (errno));
		if (!fgets (ior.get (), buf_size, server))
			throw std::runtime_error (strerror (errno));
			
		FILE *client = popen ((client_command + " " + ior.get ()).c_str (), "r");
		if (!client)
			throw std::runtime_error (strerror (errno));

		/// get client's output
		std::auto_ptr<char > client_return_buf (new char [buf_size]);
		std::stringstream client_return;
		while (fgets (client_return_buf.get (), buf_size, client))
			client_return << client_return_buf.get ();
		/// close client process
		if (pclose (client) < 0)
			throw std::runtime_error(errno ?
						strerror (errno) :
						"client process pclose() returned -1");
		
		/// terminate server process
		sighandler_t sig_term_handler = signal (SIGTERM, SIG_IGN);
		kill (0, SIGTERM);
		signal (SIGTERM, sig_term_handler);
		/// get server's output
		std::auto_ptr<char > server_return_buf (new char [buf_size]);
		std::stringstream server_return;
		while (fgets (server_return_buf.get (), buf_size, server))
			server_return << server_return_buf.get ();
		/// close server process
		if (pclose (server) < 0)
			throw std::runtime_error(errno ?
						strerror (errno) :
						"server process pclose() returned -1");
			
		return client_server_return_type (server_return.str (), client_return.str ());
	}
}
