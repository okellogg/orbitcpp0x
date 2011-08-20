#include <tut/tut.hpp>
#include <set>

#include "client_server.h"

namespace tut
{
	struct factory_data
	{
	};

	typedef test_group<factory_data > factory_tests_group;
	typedef factory_tests_group::object factory_tests_object;
	factory_tests_group factory_tests("factory");

	template<>
	template<>
	void factory_tests_object::test<1>()
	{
		std::stringstream ss;
		ss << "Bike asked to: pedal" << std::endl
			<< "Bike destroyed." << std::endl;
		ensure_equals("factory: server return string",
			client_server("cpp/factory/server", "cpp/factory/client").first,
			ss.str());
	}
}
