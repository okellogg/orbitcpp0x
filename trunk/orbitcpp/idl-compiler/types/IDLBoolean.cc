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
 *
 */

#include "IDLBoolean.h"

std::string
IDLBoolean::get_c_typename () const
{
	return "CORBA_boolean";
}

std::string
IDLBoolean::get_cpp_typename () const
{
	return IDL_CORBA_NS "::Boolean";
}

std::string
IDLBoolean::get_default_value (StringSet const &labels) const
{
	std::string val = "";

	if (labels.find ("1") == labels.end()        // non-standard: should be "TRUE"
	    && labels.find ("TRUE") == labels.end())
		val = "true";
	else if (labels.find ("0") == labels.end ()  // non-standard: should be "FALSE"
	    && labels.find ("FALSE") == labels.end())
		val = "false";

	return val;
}
