/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 1998-2000 Ronald Garcia
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Author: Ronald Garcia <rgarcia4@darwin.helios.nd.edu>
 *
 *  Description: Portable Object Adaptor Header file, with portions
 *  taken from Micos poa.h
 */




#ifndef ORBITCPP_POA_HH
#define ORBITCPP_POA_HH




#include "orbitcpp/orb-cpp/orbitcpp_smartptr.h"
#include "orbitcpp/orb-cpp/orbitcpp_orb.h"
#include "orbitcpp/orb-cpp/orbitcpp_object.h"
#include "orbitcpp/orb-cpp/orbitcpp_policy.h"
#include "orbitcpp/orb-cpp/orbitcpp_exception.h"
#include "orbitcpp/orb-cpp/orbitcpp_simple_seq.h"
#include <orbit/orb-core/corba-defs.h>



namespace PortableServer {
    ORBITCPP_DECLARE_SIMPLE_SEQUENCE(ObjectId, CORBA_octet);

#define P(name, id)                                                            \
    const CORBA::PolicyType id = PortableServer_##id;                          \
    typedef PortableServer_##name##Value name##Value;                          \
    class name;                                                                \
    ORBITCPP_MAKEREFTYPES(name);                                               \
    class name : public CORBA::Policy {                                        \
    public:                                                                    \
        explicit name (PortableServer_##name cobject)                          \
            : CORBA::Policy((CORBA_Policy)cobject) {}                          \
        static name##_ptr _duplicate(name##_ptr obj){                          \
            if (obj == 0) return 0;                                            \
            ::_orbitcpp::CEnvironment ev;                                      \
            PortableServer_##name newpolicy =                                  \
                (PortableServer_##name)CORBA_Object_duplicate(                 \
                    (CORBA_Object)obj->_orbitcpp_cobj(),                       \
                    ev._orbitcpp_cobj());                                      \
            ev.propagate_sysex();                                              \
            return new name(newpolicy);                                        \
        }                                                                      \
        static name##_ptr _narrow(CORBA::Object_ptr obj){                      \
            return _duplicate(static_cast<name##_ptr>(obj)); }                 \
        static name##_ptr _nil(){ return 0; }                                  \
        name##Value value() {                                                  \
            ::_orbitcpp::CEnvironment ev;                                      \
            name##Value value = PortableServer_##name##__get_value(            \
                (PortableServer_##name)this->_orbitcpp_cobj(),                 \
                ev._orbitcpp_cobj());                                          \
            ev.propagate_sysex();                                              \
            return value;                                                      \
        }                                                                      \
    private:                                                                   \
        name();                                                                \
        name(const name &);                                                    \
        void operator=(const name &);                                          \
    }
    
    P(ThreadPolicy, THREAD_POLICY_ID);
    P(LifespanPolicy, LIFESPAN_POLICY_ID);
    P(IdUniquenessPolicy, ID_UNIQUENESS_POLICY_ID);
    P(IdAssignmentPolicy, ID_ASSIGNMENT_POLICY_ID);
    P(ServantRetentionPolicy, SERVANT_RETENTION_POLICY_ID);
    P(RequestProcessingPolicy, REQUEST_PROCESSING_POLICY_ID);
    P(ImplicitActivationPolicy, IMPLICIT_ACTIVATION_POLICY_ID);
#undef P
    
#define P(pname, name) const pname##Value name = PortableServer_##name
    P(ThreadPolicy, ORB_CTRL_MODEL);
    P(ThreadPolicy, SINGLE_THREAD_MODEL);
    P(ThreadPolicy, MAIN_THREAD_MODEL);
    P(LifespanPolicy, TRANSIENT);
    P(LifespanPolicy, PERSISTENT);
    P(IdUniquenessPolicy, UNIQUE_ID);
    P(IdUniquenessPolicy, MULTIPLE_ID);
    P(IdAssignmentPolicy, USER_ID);
    P(IdAssignmentPolicy, SYSTEM_ID);
    P(ServantRetentionPolicy, RETAIN);
    P(ServantRetentionPolicy, NON_RETAIN);
    P(RequestProcessingPolicy, USE_ACTIVE_OBJECT_MAP_ONLY);
    P(RequestProcessingPolicy, USE_DEFAULT_SERVANT);
    P(RequestProcessingPolicy, USE_SERVANT_MANAGER);
    P(ImplicitActivationPolicy, IMPLICIT_ACTIVATION);
    P(ImplicitActivationPolicy, NO_IMPLICIT_ACTIVATION);
#undef P
    
    CORBA_char* ObjectId_to_string(ObjectId const&);
    CORBA_wchar* ObjectId_to_wstring(ObjectId const&);
    ObjectId* string_to_ObjectId(CORBA_char const*);
    ObjectId* wstring_to_ObjectId(CORBA_wchar const*);

    //typedef CORBA::Object ServantManager;
    typedef PortableServer_ServantManager ServantManager_ptr;
    typedef PortableServer_ServantManager ServantManager_var;
    
    class ServantBase;
    typedef ServantBase* Servant;
    
    class ServantBase  {
    public:
        // virtual bool _is_a(const char *logical_type_id) throw(CORBA::SystemException);

        virtual ~ServantBase();
        virtual POA_ptr _default_POA();

        // The default implementation of these are empty
        virtual void _add_ref();
        virtual void _remove_ref();
        
        // ORBit-C++ extension
        virtual PortableServer_Servant _orbitcpp_get_c_servant() = 0;

        
    protected:
        ServantBase() {
        }
    
        ServantBase(const ServantBase&);
        ServantBase& operator=(const ServantBase&);
    public: // for now
        // NOTE! This needs to be defined somewhere
    
        static PortableServer_ServantBase__epv epv;
    };

    class RefCountServantBase : public virtual ServantBase
    {
    public:
        virtual ~RefCountServantBase();

        virtual void _add_ref();
        virtual void _remove_ref();

    protected:
        RefCountServantBase() : m_ref_count(1) {}
        RefCountServantBase(const RefCountServantBase&) : m_ref_count(1) {}

        RefCountServantBase& operator=(const RefCountServantBase&);

    private:
        // XXX need to make this thread safe
        CORBA::ULong m_ref_count;
    };
    


    class ServantBase_var
    {
    public:
        ServantBase_var() : _ptr(0) {}
        
        ServantBase_var(ServantBase* p) : _ptr(p) {}
        
        ServantBase_var(const ServantBase_var& b) : _ptr(b._ptr)
        {
            if (_ptr != 0) _ptr->_add_ref();
        }

        ~ServantBase_var() {
            if (_ptr != 0) _ptr->_remove_ref();
        }

        ServantBase_var& operator=(ServantBase* p) {
            if (_ptr != 0) _ptr->_remove_ref();
            _ptr = p;
            return *this;
        }

        ServantBase_var&
        operator=(const ServantBase_var& b){
            if (_ptr != b._ptr) {
                if (_ptr != 0) _ptr->_remove_ref();
                if ((_ptr = b._ptr) != 0)
                    _ptr->_add_ref();
            }
            return *this;
        }

        ServantBase* operator->() const {
            return _ptr;
        }

        ServantBase* in() const {
            return _ptr;
        }
        ServantBase*& inout() {
            return _ptr;
        }
        ServantBase*& out() {
            if (_ptr != 0) _ptr->_remove_ref();
            _ptr = 0;
            return _ptr;
        }

        ServantBase* _retn(){
            ServantBase* retval = _ptr;
            _ptr = 0;
            return retval;
        }

    private:
        ServantBase* _ptr;
    };

    
    class POA : public CORBA::Object
    {
        friend void release(POA_ptr poa);
    
    public:
        explicit POA(PortableServer_POA cobject);
        ~POA() {}

        POA_ptr create_POA(
            const std::string& adapter_name,
            POAManager_ptr a_POAManager,
            const CORBA::PolicyList& policies);
        POA_ptr find_POA(
            const std::string& adapter_name,
            bool activate_it);

        static void destroy( bool etherealize_objects,
                             bool wait_for_completion );
        
#define P(name, funame)                                                        \
        name##_ptr funame(name##Value value) {                                 \
            ::_orbitcpp::CEnvironment ev;                                      \
            name##_ptr newpolicy = new                                         \
                name(PortableServer_POA_##funame(                              \
                    _orbitcpp_cobj(), value, ev._orbitcpp_cobj()));            \
            ev.propagate_sysex();                                              \
            return newpolicy;                                                  \
        }
        
        P(ThreadPolicy, create_thread_policy);
        P(LifespanPolicy, create_lifespan_policy);
        P(IdUniquenessPolicy, create_id_uniqueness_policy);
        P(IdAssignmentPolicy, create_id_assignment_policy);
        P(ImplicitActivationPolicy, create_implicit_activation_policy);
        P(ServantRetentionPolicy, create_servant_retention_policy);
        P(RequestProcessingPolicy, create_request_processing_policy);
#undef P
    
        POAManager_ptr the_POAManager();

//        ServantManager_ptr get_servant_manager();
        void set_servant_manager(ServantManager_ptr);
    
        ObjectId *activate_object (Servant srv);
        void activate_object_with_id(ObjectId const &oid, Servant srv );
        void deactivate_object(ObjectId const &oid);
    
        ObjectId *servant_to_id(Servant srv);
        CORBA::Object_ptr servant_to_reference(Servant srv);
        PortableServer::Servant reference_to_servant(CORBA::Object_ptr obj);
        ObjectId *reference_to_id(CORBA::Object_ptr obj);
        PortableServer::Servant id_to_servant(ObjectId const &oid);
        CORBA::Object_ptr id_to_reference(ObjectId const &oid);

        static POA_ptr _duplicate(POA_ptr obj);
        static POA_ptr _narrow(CORBA::Object_ptr obj);
        static POA_ptr _nil();

        PortableServer_POA _orbitcpp_cobj()
        {
            return reinterpret_cast<PortableServer_POA>(m_target);
        }

        #define P(name,quotedname) ORBITCPP_USEREXCEPTION_FACTORY(name, \
		                                                          quotedname)
        #include "orbitcpp/orb-cpp/orbitcpp_poaexceptionlist.h"
        #undef P
    
    private:
        //PortableServer_POA_type m_target;
        // Copy, assignment, and default constructor not wanted
    
        POA();
        POA(const POA &);
        void operator=(const POA &);
    };
    
    
    class POAManager : public CORBA::Object
    {
        friend class POA;
    public:
        static POAManager_ptr _duplicate( POAManager_ptr obj );
        static POAManager_ptr _narrow( CORBA::Object_ptr obj );
        static POAManager_ptr _nil();
        void activate();
        void hold_requests(bool wait_for_completion);
        void discard_requests(bool wait_for_completion);
        void deactivate(bool etherealize_objects,
                        bool wait_for_completion);    
        virtual ~POAManager()
        {}
        
    protected:
        POAManager(PortableServer_POAManager cobject);
    
    private:
        PortableServer_POAManager _orbitcpp_cobj() {
            return reinterpret_cast<PortableServer_POAManager>(m_target);
        }
    };
    
}

namespace _orbitcpp {

#define ORBITCPP_IMPL2(Servant, servant) (reinterpret_cast<Servant*>(servant)->m_cppimpl)
#define ORBITCPP_IMPL(servant) ORBITCPP_IMPL2(_orbitcpp_Servant, servant)

template <class CServant, class Impl>
struct Servant {
    CServant m_cservant;
    PortableServer::Servant m_cppservant;
    Impl* m_cppimpl;

    typedef void (*init_fn)(PortableServer_Servant, CORBA_Environment*);

    template <class V>
    Servant(V* vepv, Impl* self, init_fn init)
    {
        m_cservant._private = NULL;
        m_cservant.vepv = vepv;
        m_cppservant = self;
        m_cppimpl = self;

        CEnvironment ev;
        (*init)(&m_cservant, ev._orbitcpp_cobj());
        ev.propagate_sysex();
    }

    // For faking base servant.
    explicit Servant(Impl* self)
    {
        m_cservant._private = NULL;
        m_cservant.vepv = NULL;
        m_cppservant = self;
        m_cppimpl = self;
    }
};

// This is a binary compatible to aid extraction of the
// cppservant from C servants. This *must* be kept in sync with what
// the compiler produces.
struct UnknownCServant {
    void* _private;
    void* vepv;
};

typedef Servant<UnknownCServant, PortableServer::ServantBase> UnknownServant;

}




namespace CORBA {
// the mandatory release function
void release(PortableServer::POA_ptr poa);
}




#endif // ORBITCPP_POA_HH
