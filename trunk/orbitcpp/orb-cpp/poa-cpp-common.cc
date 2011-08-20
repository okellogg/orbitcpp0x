// -----------------------------------------------------
// generated by orbitcpp, do not edit
// -----------------------------------------------------


#include "poa-cpp-common.h"


	PortableServer::ForwardRequest::ForwardRequest (::CORBA::Object_ptr _par_forward_reference)
{
	forward_reference = _par_forward_reference;
}

	void PortableServer::ForwardRequest::_orbitcpp_set (::CORBA_Environment *ev){
	::CORBA_exception_set (ev, ::CORBA_USER_EXCEPTION, "IDL:PortableServer/ForwardRequest:1.0", _orbitcpp_pack ());
}

	PortableServer_ForwardRequest * PortableServer::ForwardRequest::_orbitcpp_pack () const
{
	PortableServer_ForwardRequest *_c_struct = PortableServer_ForwardRequest__alloc ();

	if (!_c_struct)
		throw CORBA::NO_MEMORY ();

	_orbitcpp_pack (*_c_struct);
return _c_struct;
}

	void PortableServer::ForwardRequest::_orbitcpp_pack (PortableServer_ForwardRequest &_c_struct) const
{
	_c_struct.forward_reference = (!::CORBA::is_nil(forward_reference.in()) ? ::_orbitcpp::duplicate_guarded(forward_reference.in()->_orbitcpp_cobj ()) : CORBA_OBJECT_NIL);

}

	void PortableServer::ForwardRequest::_orbitcpp_unpack (const PortableServer_ForwardRequest &_c_struct)
{
	forward_reference = ::CORBA::Object::_orbitcpp_wrap (::_orbitcpp::duplicate_guarded (_c_struct.forward_reference));
}

