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
 *  Purpose:	IDL compiler type representation
 *
 *  Remarks:
 *    It is convention that the C struct is available by reference as "_cstruct"
 *    in the writeCPPStructPacker/Unpacker() contexts.
 *
 */




#ifndef ORBITCPP_TYPES
#define ORBITCPP_TYPES


#include <libIDL/IDL.h>
#include <string>
#include <vector>



class IDLTypedef;
class IDLType;
class IDLScope;
class IDLTypeDiscrim;

class IDLTypeParser
{
	static IDLTypeDiscrim *parse(IDLScope* scope, IDL_tree typespec);
public:
	static IDLType *parseTypeSpec(IDLScope* scope, IDL_tree typespec);
	static IDLScope *parseScope(IDLScope*, IDL_tree typespec);
	static IDLType *parseDcl(IDL_tree dcl, IDL_tree typespec, std::string &id);
};




#endif //ORBITCPP_TYPES
