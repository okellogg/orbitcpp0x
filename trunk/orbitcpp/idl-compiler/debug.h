/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2003 The orbitcpp development team.
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
 *  Purpose:	IDL compiler error declarations
 *
 */


#ifndef ORBITCPP_IDL_COMPILER_DEBUG_H
#define ORBITCPP_IDL_COMPILER_DEBUG_H

//This is copied from glibmm:
/* We can't use G_GNUC_PRETTY_FUNCTION because it's always disabled in C++,
 * even though __PRETTY_FUNCTION__ works fine in C++ as well if you use it
 * right (i.e. concatenation with string literals isn't allowed).
 */
#ifdef __GNUC__
#define ORBITCPP_GNUC_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#define ORBITCPP_GNUC_PRETTY_FUNCTION ""
#endif

#ifdef ORBITCPP_DEBUG_ENABLED
  //Print a message, with the current source line next to it.
  //This helps us to see what part of the orbitcpp source code generated what part of the generate file.
  #define ORBITCPP_IDL_COMPILER_DEBUG(outputsream)   \
      outputsream << "//debug: file " << __FILE__ << " line " << __LINE__ << " (" << ORBITCPP_GNUC_PRETTY_FUNCTION << " )" << std::endl;
#else    
  #define ORBITCPP_IDL_COMPILER_DEBUG(outputsream)
#endif //ORBITCPP_DEBUG_ENABLED

#endif //ORBITCPP_IDL_COMPILER_DEBUG_H
