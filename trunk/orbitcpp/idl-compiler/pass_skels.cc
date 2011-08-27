/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2000 Andreas Kloeckner
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
 *  Author:	Andreas Kloeckner <ak@ixion.net>
 *
 *  Purpose: skeleton generation pass
 *
 */




#include "language.h"
#include "error.h"
#include "pass_skels.h"

#include "types/IDLOperation.h"
#include "types/IDLAttribAccessor.h"
#include "types/IDLException.h"

#include <iostream>


// IDLPassSkels --------------------------------------------------------------
void 
IDLPassSkels::runPass() {
	m_header
	<< indent << "#ifndef ORBITCPP_IDL_" << idlUpper(m_state.m_basename) << "_SKELS" << endl
	<< indent << "#define ORBITCPP_IDL_" << idlUpper(m_state.m_basename) << "_SKELS" << endl
	<< indent << "#include \"" << m_state.m_basename << IDL_CPP_STUB_HEADER_EXT"\"" << endl
	<< indent << endl << endl
	<< indent << "// Skeleton declaration --------------------------------" << endl
	<< indent << endl;

	m_module
	<< mod_indent << "#include \"" << m_state.m_basename << IDL_CPP_SKEL_HEADER_EXT"\"" << endl
	<< mod_indent << endl << endl
	<< mod_indent << "// Skeleton code ---------------------------------------" << endl
	<< mod_indent << endl;

	vector<IDLInterface *>::iterator
	first = m_state.m_interfaces.begin(), last = m_state.m_interfaces.end();
	while (first != last)
		doInterface(*(*first++));
	
	runJobs();
	
	m_header << endl << indent << "#endif" << endl;
}




// operation-scope methods ----------------------------------------------------

void
IDLPassSkels::create_method_skel_proto (const IDLMethod &method)
{
	m_header << indent << "static " << method.skel_decl_proto ()
		 << ';' << endl;
}

/* See doOperationSkel for notes */
void
IDLPassSkels::create_method_skel (const IDLInterface &iface,
				  const IDLInterfaceRight &of,
				  const IDLMethod    &method)
{
	string skel_name = iface.get_cpp_poa_method_prefix () + "::"
		+ method.skel_get_cpp_methodname ();

	// print header
	m_module << mod_indent << method.skel_ret_get () << " "
		 << skel_name << " (" + method.skel_arglist_get () + ") throw ()" << endl
		 << mod_indent << "{" << endl;
	++mod_indent;

	// inherited => call up base "passthru"
	if (&iface != &of)
	{
		m_module << mod_indent << of.get_cpp_poa_typename () << "::_orbitcpp_Servant _fake (ORBITCPP_IMPL(_servant));" << endl;
		m_module << mod_indent << "return "
			 << of.get_cpp_poa_typename () << "::"
			 << "_skel_" << method.get_cpp_methodname ()
			 << " (&_fake, ";

		for (IDLMethod::ParameterList::const_iterator i = method.m_parameterinfo.begin ();
		     i != method.m_parameterinfo.end (); i++)
		{
			m_module << i->id << ", ";
		}

		m_module << "_ev);" << endl;

	}
	else {
		// Create C++ exception context
		m_module << mod_indent++ << "try {" << endl;

		// Prepare parameters and return value container
		method.skel_do_pre (m_module, mod_indent);

		// Do the call
		method.skel_do_call (m_module, mod_indent);
		
		// De-init parameters and return value container,
		method.skel_do_post (m_module, mod_indent);

		m_module << --mod_indent << "}" << endl;

		// Catch CORBA exceptions
		m_module << mod_indent++ << "catch (CORBA::Exception &_ex) {" << endl
		     << mod_indent << "_ex._orbitcpp_set (_ev);" << endl
		     << mod_indent << "typedef " << method.skel_ret_get () << " t;\n"
		     << mod_indent << "return t();\n";
		m_module << --mod_indent << "}" << endl;
	}

	// End of skel implementation
	m_module << --mod_indent << "}" << endl << endl;
}

void
IDLPassSkels::create_method_proto (const IDLMethod &method)
{
	m_header << indent << "virtual " << method.stub_decl_proto () << endl;

	m_header << ++indent << "throw (" IDL_CORBA_NS "::SystemException";

	for (IDLOperation::ExceptionList::const_iterator i = method.m_raises.begin ();
	     i != method.m_raises.end (); i++)
	{
		m_header << ", " << (*i)->get_cpp_typename ();
	}

	m_header << ") = 0;" << endl;
	indent--;
}

void
IDLPassSkels::create_method_tie (const IDLMethod    &method)
{
}


void 
IDLPassSkels::doAttributeSkelPrototype (IDLInterface &iface,
					IDLInterfaceRight &of,
					IDL_tree      node)
{
	IDL_tree simple_dcl = IDL_ATTR_DCL(node).simple_declarations;
	for (; simple_dcl != 0; simple_dcl = IDL_LIST(simple_dcl).next) {
		IDL_tree sd = IDL_LIST(simple_dcl).data;
		IDLAttribute attr (node, sd);

		// Getter
		create_method_skel_proto (IDLAttribGetter (of, attr));

		// Setter
		if (!attr.isReadOnly ())
			create_method_skel_proto (IDLAttribSetter (of, attr));
	}
}

void 
IDLPassSkels::doAttributeSkel(IDLInterface &iface,
			      IDLInterfaceRight &of,
			      IDL_tree      node)
{
	IDL_tree simple_dcl = IDL_ATTR_DCL(node).simple_declarations;
	for (; simple_dcl != 0; simple_dcl = IDL_LIST(simple_dcl).next) {
		IDL_tree sd = IDL_LIST(simple_dcl).data;
		IDLAttribute attr (node, sd);

		// Getter
		create_method_skel (iface, of, IDLAttribGetter (of, attr));

		// Setter
		if (!attr.isReadOnly ())
			create_method_skel (iface, of, IDLAttribSetter (of, attr));
	}
}


void 
IDLPassSkels::doAttributePrototype (IDLInterface &iface,
				    IDL_tree       node)
{
	IDL_tree simple_dcl = IDL_ATTR_DCL(node).simple_declarations;
	for (; simple_dcl != 0; simple_dcl = IDL_LIST(simple_dcl).next) {
		IDL_tree sd = IDL_LIST(simple_dcl).data;
		IDLAttribute attr (node, sd);

		// Getter
		create_method_proto (IDLAttribGetter (iface, attr));

		// Setter
		if (!attr.isReadOnly ())
			create_method_proto (IDLAttribSetter (iface, attr));
	}
}

void 
IDLPassSkels::doAttributeTie (IDLInterface &iface,
			      IDL_tree      node)
{
	IDL_tree simple_dcl = IDL_ATTR_DCL(node).simple_declarations;
	for (; simple_dcl != 0; simple_dcl = IDL_LIST(simple_dcl).next) {
		IDL_tree sd = IDL_LIST(simple_dcl).data;
		IDLAttribute attr (node, sd);

		// Getter
		create_method_tie (IDLAttribGetter (iface, attr));

		// Setter
		if (!attr.isReadOnly ())
			create_method_tie (IDLAttribSetter (iface, attr));
	}
}

void 
IDLPassSkels::doOperationSkelPrototype (IDLInterface &iface,
					IDLInterfaceRight &of,
					IDL_tree      node)
{
	
	IDLOperation op (node, of);

	create_method_skel_proto (op);
	
	if (IDL_OP_DCL(node).context_expr != NULL) ORBITCPP_NYI("contexts");
}



/** @brief      Write to the output stream, the skeleton code to unmarshal a CORBA
 *              method and make the call into the corresponding C++ servant.
 *
 *  @param      iface   The orbitcpp interface object which this method belongs to
 *  @param      of      Document Me
 *  @param      node    The IDL tree node which contains info about this method.
 *
 *  This routine contains considerable complexity to account for the different 
 *  memory management schemes employed by CORBA.
 *  The routine is responsible for writing the implementation of a skeleton routine
 *  starting from the method prototype, including the code to unmarshal the C 
 *  structures passed through ORBit to the skeleton. Then the routine must write code
 *  which makes a call to the appropriate C++ servant method.
 *
 *  Because of the memory management issues associated with fixed and variable length
 *  CORBA types, there are two distinct ways of passing a parameter to the servant 
 *  method.
 *
 *  For in and inout types, the type passed through the C skel can be passed directly 
 *  to the servant. For out types, there is typically a need to construct a proxy 
 *  or manager object, as denote by the _out_type of the CORBA type.
 *  The exception to this is arrays of fixed length objects. In this case the _out is
 *  simply a typedef to a T_slice * type. Because the client is responsible for
 *  allocation of the object this gives all the required access to modify individual 
 *  elements inside the servant.
 *  Because variable lenth arrays require a manager object they fall under the general
 *  case.
 *
 *  In terms of the code which is written, T_out object must be constructed from the C
 *  argument passed in. However, for arrays of fixed length objects, the _out is a typedef
 *  to T_slice * so a pointer assignment is required to make the code syntactically 
 *  correct C++.
 *
 *  To determine if the parameter passed is an Array of fixed length objects requires
 *  first dereferencing any aliasing done through typedefs. Then there are simple methods
 *  to determine if the code is an array (dynamic_cast to IDLArray) and contains fixed 
 *  length types (isVariableLength() ).
 */
void 
IDLPassSkels::doOperationSkel (IDLInterface &iface,
			       IDLInterfaceRight &of,
			       IDL_tree      node)
{
	IDLOperation op (node, of);

	create_method_skel (iface, of, op);

	if (IDL_OP_DCL(node).context_expr != NULL) ORBITCPP_NYI("contexts");
}

void 
IDLPassSkels::doOperationPrototype (IDLInterface &iface,
				    IDL_tree      node)
{
	IDLOperation op (node, iface);

	create_method_proto (op);

	if (IDL_OP_DCL(node).context_expr != NULL) ORBITCPP_NYI("contexts");
}



void 
IDLPassSkels::doOperationTie (IDLInterface &iface,
			      IDL_tree      node)
{
	IDLOperation op (node, iface);

	create_method_tie (op);	
}




// interface-scope methods ----------------------------------------------------
void 
IDLPassSkels::doInterfaceAppServant (IDLInterface &iface)
{
	string c_POA = "POA_" + iface.get_c_typename ();
	string cxx_POA = iface.get_cpp_poa_typename ();

	m_header << indent << "typedef _orbitcpp::Servant< " << c_POA << "," << cxx_POA << " > _orbitcpp_Servant;\n";
	m_header << indent << "_orbitcpp_Servant m_target;\n";

}




void 
IDLPassSkels::doInterfaceEPVs (IDLInterface &iface)
{
	// base_epv decl
	m_header << indent << "static "
		 << "::PortableServer_ServantBase__epv _base_epv;" << endl;

	// base_epv def
	m_module << mod_indent <<  "::"
		 << "PortableServer_ServantBase__epv "
		 << iface.get_cpp_poa_method_prefix () << "::_base_epv = {" << endl;
	++mod_indent;

	m_module << mod_indent << "NULL, // _private" << endl
		 << mod_indent << iface.get_cpp_poa_typename () << "::_orbitcpp_fini," << endl
		 << mod_indent << "NULL, // default_POA" << endl
		 << mod_indent << "NULL, // add_ref" << endl
		 << mod_indent << "NULL, // remove_ref" << endl
		 << mod_indent << "NULL, // get_interface" << endl
		 << mod_indent << "NULL, // is_a" << endl
		 << mod_indent << "NULL  // non_existent" << endl;

	m_module << --mod_indent << "};" << endl << endl;

	// epv handling
	for (IDLInterface::BaseList::const_iterator i = iface.m_allbases.begin ();
	     i != iface.m_allbases.end (); i++)
	{
		declareEPV (iface, **i);
		defineEPV (iface, **i);
	}
	declareEPV (iface, iface);
	defineEPV (iface, iface);

	// vepv decl
	m_header << indent << "static " IDL_IMPL_C_NS_NOTUSED
		 << iface.get_c_poa_vepv () << " _vepv;" << endl;

	// vepv def
	m_module << mod_indent << IDL_IMPL_C_NS_NOTUSED
		 << iface.get_c_poa_vepv() << ' '
		 << iface.get_cpp_poa_method_prefix () << "::_vepv = {" << endl;

	m_module << ++mod_indent << '&' << iface.get_cpp_poa_typename ()
		 << "::_base_epv," << endl;

	for (IDLInterface::BaseList::const_iterator i = iface.m_allbases.begin ();
	     i != iface.m_allbases.end (); i++)
	{
		m_module << mod_indent << "&_"
			 << (*i)->get_c_typename () << "_epv," << endl;
	}

	m_module << mod_indent << "&_"
		 << iface.get_c_typename () << "_epv" << endl;

	m_module << --mod_indent << "};" << endl << endl;
}




void 
IDLPassSkels::declareEPV (IDLInterface &iface,
			  IDLInterfaceRight &of)
{
	m_header << indent << "static " IDL_IMPL_C_NS_NOTUSED
		 << of.get_c_poa_epv () << " _"
		 << of.get_c_typename () << "_epv;" << endl;
}




void 
IDLPassSkels::defineEPV (IDLInterface &iface,
			 IDLInterfaceRight &of)
{
	m_module << mod_indent << IDL_IMPL_C_NS_NOTUSED
		 << of.get_c_poa_epv () << ' '
		 << iface.get_cpp_poa_method_prefix () << "::"
		 << "_" << of.get_c_typename () << "_epv = {" << endl;
	m_module << ++mod_indent << "0, // _private" << endl;

	IDL_tree body_list = IDL_INTERFACE(of.getNode()).body;
  
	while (body_list) {
		IDLElement *item = 0;
    
		switch (IDL_NODE_TYPE(IDL_LIST(body_list).data)) {
		case IDLN_ATTR_DCL: {
			IDL_tree node = IDL_LIST(body_list).data;
			IDL_tree simple_dcl = IDL_ATTR_DCL(node).simple_declarations;
			for (; simple_dcl != 0; simple_dcl = IDL_LIST(simple_dcl).next) {
				IDL_tree sd = IDL_LIST(simple_dcl).data;
				IDLAttribute attr (node, sd);
				m_module << mod_indent << iface.get_cpp_poa_typename ()
					 << "::" << IDLAttribGetter(of, attr).skel_get_cpp_methodname ()
					 << ',' << endl;
				if (!attr.isReadOnly ())
				{
					m_module << mod_indent << iface.get_cpp_poa_typename ()
						 << "::" << IDLAttribSetter(of, attr).skel_get_cpp_methodname ()
						 << ',' << endl;
				}
			}
		} break;
			
		case IDLN_OP_DCL: {
			IDLOperation op (IDL_LIST(body_list).data, iface);
			m_module << mod_indent << iface.get_cpp_poa_typename ()
				 << "::_skel_" << op.get_cpp_identifier () << "," << endl;
			} break;
		default:
			break;
		}
    
		body_list = IDL_LIST(body_list).next;
	}

	m_module << --mod_indent << "};" << endl << endl;
}




void 
IDLPassSkels::doInterfaceFinalizer (IDLInterface &iface)
{
	m_header << indent << "static void _orbitcpp_fini("
		 <<  "::PortableServer_Servant servant, "
		 <<  "::CORBA_Environment *ev);" << endl;

	m_module << mod_indent << "void " << iface.get_cpp_poa_method_prefix ()
		 << "::_orbitcpp_fini ("
		 <<  "::PortableServer_Servant servant, "
		 <<  "::CORBA_Environment *ev)" << endl
		 << mod_indent << "{" << endl;

	m_module << ++mod_indent << "//Call C _fini():" << endl
		 << mod_indent << IDL_IMPL_C_NS_NOTUSED
		 << iface.get_c_poa_typename () << "__fini (servant, ev);" << endl << endl;
	
	m_module << mod_indent << "//Do C++-specific stuff:" << endl
		 << mod_indent << "ORBITCPP_IMPL(servant)->_remove_ref();" << endl;

	m_module << --mod_indent << '}'<< endl << endl;
}




void 
IDLPassSkels::doInterface (IDLInterface &iface)
{
	string ns_begin,ns_end;
	iface.get_cpp_poa_namespace (ns_begin,ns_end);
	
	if (ns_begin.size()) {
		m_header
		<< indent << ns_begin << endl << endl;
	}

	doInterfaceDerive(iface);
	doInterfaceDelegate(iface);

	if (ns_begin.size())
		m_header << indent << ns_end << endl << endl;		
}




void 
IDLPassSkels::doInterfaceDerive(IDLInterface &iface) {
	// class header
	m_header << indent << "class " << iface.get_cpp_poa_identifier () << ": ";

	for (IDLInterface::BaseList::const_iterator i = iface.m_bases.begin ();
	     i != iface.m_bases.end (); i++)
	{
		m_header << "public virtual " << (*i)->get_cpp_poa_typename () << ", ";
	}

	m_header << "public virtual "
		 << IDL_POA_NS "::ServantBase" << endl;
	
	m_header << indent << "{" << endl;

	// C interface
	m_header << ++indent << "// C interface" << endl;
	m_header << --indent << "public:" << endl;
	indent++;
	doInterfaceAppServant(iface);
	
	m_header << --indent << "protected:" << endl;
	indent++;
	doInterfaceEPVs (iface);
	doInterfaceFinalizer (iface);

	// C methods
	for (IDLInterface::BaseList::const_iterator i = iface.m_allbases.begin ();
	     i != iface.m_allbases.end (); i++)
	{
		doInterfaceUpCall (iface, **i);
	}
	doInterfaceUpCall(iface,iface);

	// C++ interface
	m_header << indent << "// C++ interface" << endl
		 << --indent << "public:" << endl;
	indent++;

	// constructor
	m_header << indent << iface.get_cpp_poa_identifier () << " ();" << endl;

	m_module << mod_indent << iface.get_cpp_poa_typename ()  << "::"
		 << iface.get_cpp_poa_identifier () << " ()" << endl
		 << mod_indent << ":\tm_target(&_vepv, this, (_orbitcpp_Servant::init_fn)(&" << iface.get_c_poa_typename () << "__init))\n"
		 << mod_indent++ << "{" << endl;
	
	m_module << --mod_indent << '}' << endl << endl;

	// destructor
	m_header << indent << "virtual ~" << iface.get_cpp_poa_identifier () << "()" << endl
		 << indent << "{" << endl
		 << indent << "}" << endl << endl;

	// C++ methods
	m_header << indent <<  "::PortableServer_Servant _orbitcpp_get_c_servant ()" << endl
		 << indent++ << "{" << endl;
	m_header << indent << "return reinterpret_cast< " 
		 << "::PortableServer_Servant >"
		 << "(&m_target);" << endl;
	m_header << --indent << '}' << endl << endl;

	// _this() method declaration:
	m_header << indent << iface.get_cpp_typename_ptr () << " _this();" << endl << endl;
	
	// _this() method implementation:
	string stub_name = iface.get_cpp_stub_typename ();

	m_module << mod_indent << iface.get_cpp_typename_ptr () << " "
		 << iface.get_cpp_poa_method_prefix () << "::" << "_this()" << endl
		 << mod_indent << "{" << endl;
	++mod_indent;

	m_module << mod_indent << "PortableServer::POA_var rootPOA = _default_POA ();" << endl << endl;
	
	m_module << mod_indent << "CORBA::Object_var object = rootPOA->servant_to_reference (this);" << endl;
	m_module << mod_indent << "return "
		 << stub_name << "::_orbitcpp_wrap (object->_orbitcpp_cobj (), true);" << endl;
	m_module << --mod_indent << "}" << endl << endl;

	
	doInterfacePrototypes(iface);

	// end of class
	m_header << --indent << "};" << endl << endl;
}




void 
IDLPassSkels::doInterfaceUpCall(IDLInterface &iface,IDLInterfaceRight &of) {
	IDL_tree body_list = IDL_INTERFACE(of.getNode()).body;
	while (body_list) {
		switch (IDL_NODE_TYPE(IDL_LIST(body_list).data)) {
		case IDLN_ATTR_DCL:
			doAttributeSkelPrototype(iface,of,IDL_LIST(body_list).data);
			doAttributeSkel(iface,of,IDL_LIST(body_list).data);
			break;
		case IDLN_OP_DCL:
			doOperationSkelPrototype(iface,of,IDL_LIST(body_list).data);
			doOperationSkel(iface,of,IDL_LIST(body_list).data);
			break;
		default:
			break;
		}
		body_list = IDL_LIST(body_list).next;
	}
}




void 
IDLPassSkels::doInterfacePrototypes(IDLInterface &iface) {
	IDL_tree body_list = IDL_INTERFACE(iface.getNode()).body;
	while (body_list) {
		switch (IDL_NODE_TYPE(IDL_LIST(body_list).data)) {
		case IDLN_ATTR_DCL:
			doAttributePrototype(iface,IDL_LIST(body_list).data);
			break;
		case IDLN_OP_DCL:
			doOperationPrototype(iface,IDL_LIST(body_list).data);
			break;
		default:
			break;
		}
		body_list = IDL_LIST(body_list).next;
	}
}




void 
IDLPassSkels::doInterfaceDelegate(IDLInterface &iface) {
}
