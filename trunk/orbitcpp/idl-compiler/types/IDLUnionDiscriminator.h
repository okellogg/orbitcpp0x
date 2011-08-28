/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2000 Andreas Kloeckner
 *  Copyright (C) 2011 Oliver Kellogg
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
 *  Purpose: IDL compiler language representation
 *
 */

#ifndef ORBITCPP_IDLUNIONDISCRIMINATOR
#define ORBITCPP_IDLUNIONDISCRIMINATOR

#include "base.h"

// An interface implemented by types that can be used as
// union discriminators
class IDLUnionDiscriminator {
public:
	// retns a default value, given a set of values used to
	// descriminate members of the union
	virtual std::string get_default_value (StringSet const &labels) const = 0;
	virtual std::string discr_get_c_typename () const = 0;
	virtual std::string discr_get_cpp_typename () const = 0;
};

#endif // ORBITCPP_IDLUNIONDISCRIMINATOR

