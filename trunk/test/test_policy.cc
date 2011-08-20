#include <tut/tut.hpp>
#include <set>

#include "client_server.h"

namespace tut
{
	struct policy_data
	{ 
	};

	typedef test_group<policy_data> tg_policy_tests;
	typedef tg_policy_tests::object obj_policy_tests;
	tg_policy_tests policy_tests("policy");

	template<>
	template<>
	void obj_policy_tests::test<1>()
	{
		std::stringstream ss;
		ss << "Client: Reply was \"Hello client, from server!\"" << std::endl;
		ensure_equals("client return string",
			client_server("cpp/policy/server", "cpp/policy/client").second,
			ss.str());
	}
}
