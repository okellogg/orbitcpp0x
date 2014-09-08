#ifndef factory_impl_h_
#define factory_impl_h_

#include <factory-cpp-skels.h>
#include <iostream>

namespace f {

class Bike : public virtual POA_Factory::AbstractService {
	void doit (const std::string& mesg)
		throw (CORBA::SystemException)
	{
		std::cout << "Bike asked to: " << mesg << std::endl;
	}

	void destroy ()
		throw (CORBA::SystemException)
	{
		std::cout << "Bike destroyed." << std::endl;
	}
};

class Producer : public virtual POA_Factory::Producer {
	Factory::AbstractService_ptr produce (const std::string& id)
		throw (CORBA::SystemException)
	{
		if (id == "bike") {
			Bike* bike = new Bike(); // never reclaimed
			return bike->_this();
		}

		g_assert_not_reached();
	}
};

} // namespace f

#endif
