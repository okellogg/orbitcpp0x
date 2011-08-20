/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2003 Bowie Owens
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
 *  Author:	Bowie Owens <bowie.owens@csiro.au>
 *
 *  Purpose:	Common base class for types that need to be in Compound{Un,}BoundedSeq
 *
 *
 */


#ifndef ORBITCPP_TYPES_IDLCOMPOUNDSEQELEM
#define ORBITCPP_TYPES_IDLCOMPOUNDSEQELEM

#include "IDLType.h"
#include "IDLElement.h"

class IDLCompoundSeqElem : public virtual IDLType, public virtual IDLTypenameInterface
{
public:
	string get_seq_typename (unsigned int      length,
				 const IDLTypedef *active_typedef = 0) const;
};

#endif
