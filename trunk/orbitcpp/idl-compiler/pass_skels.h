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
 *  Purpose:	skeleton generation pass
 *
 */




#ifndef ORBITCPP_PASS_SKELS
#define ORBITCPP_PASS_SKELS




#include <libIDL/IDL.h>
#include "pass.h"
class IDLMethod;
class IDLInterfaceRight;



class IDLPassSkels : public IDLOutputPass {
public:
	IDLPassSkels(IDLCompilerState &state,ostream &header,ostream &module)
		: IDLOutputPass(state,header,module) {
		state.m_pass_skels = this;
	}

	void runPass();

protected:
	void doAttributeSkelPrototype(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);
	void doAttributeSkel(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);
	void doAttributePrototype(IDLInterface &iface,IDL_tree node);
	void doAttributeTie(IDLInterface &iface,IDL_tree node);
	
	void doOperationSkelPrototype(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);
	void doOperationSkel(IDLInterface &iface,IDLInterfaceRight &of,IDL_tree node);
	void doOperationPrototype(IDLInterface &iface,IDL_tree node);
	void doOperationTie(IDLInterface &iface,IDL_tree node);

	void doInterfaceAppServant(IDLInterface &iface);
	void doInterfaceEPVs(IDLInterface &iface);
	void declareEPV(IDLInterface &iface,IDLInterfaceRight &of);
	void defineEPV(IDLInterface &iface,IDLInterfaceRight &of);
	void doInterfaceFinalizer(IDLInterface &iface);

	void doInterface(IDLInterface &iface);

	void doInterfaceDerive(IDLInterface &iface);
	void doInterfaceUpCall(IDLInterface &iface,IDLInterfaceRight &of);
	void doInterfacePrototypes(IDLInterface &iface);
	void doInterfaceDelegate(IDLInterface &iface);

	void create_method_skel_proto (const IDLMethod    &method);
	void create_method_skel       (const IDLInterface &iface,
				       const IDLInterfaceRight &of,
				       const IDLMethod    &method);
	void create_method_proto      (const IDLMethod    &method);
	void create_method_tie        (const IDLMethod    &method);
};




#endif
