#ifndef ORBITCPP_IDL_POA_SKELS
#define ORBITCPP_IDL_POA_SKELS

#include <orbitcpp/orb-cpp/poa-cpp-common.h>
#include <orbitcpp/orb-cpp/orbitcpp_poa.h>
#include <string.h>
#include <orbit/poa/poa.h>


// Skeleton declaration --------------------------------

namespace PortableServer {
	typedef void* Cookie;
}

namespace POA_PortableServer
{

class ServantActivator: public virtual PortableServer::ServantBase
{
	// C interface
public:
	typedef _orbitcpp::Servant< POA_PortableServer_ServantActivator,::POA_PortableServer::ServantActivator > _orbitcpp_Servant;
	_orbitcpp_Servant m_target;
protected:
	static ::PortableServer_ServantBase__epv _base_epv;
	static POA_PortableServer_ServantManager__epv _PortableServer_ServantManager_epv;
	static POA_PortableServer_ServantActivator__epv _PortableServer_ServantActivator_epv;
	static POA_PortableServer_ServantActivator__vepv _vepv;
	static void _orbitcpp_fini(::PortableServer_Servant servant, ::CORBA_Environment *ev);
	static PortableServer_Servant _skel_incarnate (::PortableServer_Servant _servant,const PortableServer_ObjectId *oid, PortableServer_POA poa, ::CORBA_Environment *_ev) throw ();
	static void _skel_etherialize (
			::PortableServer_Servant _servant,
			const PortableServer_ObjectId *oid,
			PortableServer_POA poa,
			PortableServer_Servant servant,
			const CORBA_boolean cleanup_in_progress,
			const CORBA_boolean remaining_activations,
			::CORBA_Environment *_ev) throw ();
// C++ interface
public:
	ServantActivator ();
	virtual ~ServantActivator()
	{
	}

		::PortableServer_Servant _orbitcpp_get_c_servant ()
	{
		return reinterpret_cast< ::PortableServer_Servant >(&m_target);
	}

	PortableServer_ServantManager _this()
	{
		return reinterpret_cast<PortableServer_ServantManager>(&m_target);
	}


	virtual ::PortableServer::Servant incarnate (::PortableServer::ObjectId const& oid,::PortableServer::POA_ptr poa)
		throw (CORBA::SystemException, ::PortableServer::ForwardRequest) = 0;
	virtual void etherialize (::PortableServer::ObjectId const& oid,::PortableServer::POA_ptr poa,PortableServer::Servant servant, CORBA::Boolean cleanup_in_progress,CORBA::Boolean remaining_activations)
		throw (CORBA::SystemException) = 0;
};

} //namespace POA_PortableServer



namespace POA_PortableServer
{


class ServantLocator: public virtual PortableServer::ServantBase
{
	// C interface
public:
	typedef _orbitcpp::Servant< POA_PortableServer_ServantLocator,::POA_PortableServer::ServantLocator > _orbitcpp_Servant;
	_orbitcpp_Servant m_target;
protected:
	static ::PortableServer_ServantBase__epv _base_epv;
	static POA_PortableServer_ServantManager__epv _PortableServer_ServantManager_epv;
	static POA_PortableServer_ServantLocator__epv _PortableServer_ServantLocator_epv;
	static POA_PortableServer_ServantLocator__vepv _vepv;
	static void _orbitcpp_fini(::PortableServer_Servant servant, ::CORBA_Environment *ev);
	static PortableServer_Servant _skel_preinvoke (::PortableServer_Servant _servant,const PortableServer_ObjectId *oid, PortableServer_POA poa, CORBA_char *operation, PortableServer_ServantLocator_Cookie *cookie, ::CORBA_Environment *_ev) throw ();
	static void _skel_postinvoke (::PortableServer_Servant _servant,const PortableServer_ObjectId *oid, PortableServer_POA poa, CORBA_char *operation, const PortableServer_ServantLocator_Cookie cookie, PortableServer_Servant servant, ::CORBA_Environment *_ev) throw ();
// C++ interface
public:
	ServantLocator ();
	virtual ~ServantLocator()
	{
	}

		::PortableServer_Servant _orbitcpp_get_c_servant ()
	{
		return reinterpret_cast< ::PortableServer_Servant >(&m_target);
	}

	PortableServer_ServantManager _this()
	{
		return reinterpret_cast<PortableServer_ServantManager>(&m_target);
	}

	virtual ::PortableServer::Servant preinvoke (::PortableServer::ObjectId const& oid,::PortableServer::POA_ptr poa,CORBA_char const *operation,::PortableServer::Cookie& cookie)
		throw (CORBA::SystemException, ::PortableServer::ForwardRequest) = 0;
	virtual void postinvoke (::PortableServer::ObjectId const& oid,::PortableServer::POA_ptr poa,CORBA_char const *operation,::PortableServer::Cookie cookie,::PortableServer::Servant servant)
		throw (CORBA::SystemException) = 0;
};

} //namespace POA_PortableServer




#endif
