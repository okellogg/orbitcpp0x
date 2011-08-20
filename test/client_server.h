#include <string>
//#include <>

namespace tut
{
	typedef std::pair<std::string, std::string > client_server_return_type;
	
	client_server_return_type
	client_server (const std::string& server_command, const std::string& client_call);
}
