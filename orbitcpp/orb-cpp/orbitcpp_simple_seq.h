// -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*-
/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 1999 Phil Dawes
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
 *  Authors: Phil Dawes <philipd@users.sourceforge.net>
 *           Andreas Kloeckner <ak@ixion.net>
 *           Gergõ Érdi <cactus@cactus.rulez.org>
 */

#ifndef ORBITCPP_SIMPLE_SEQUENCE_HH
#define ORBITCPP_SIMPLE_SEQUENCE_HH

#include "orbitcpp/orb-cpp/orbitcpp_sequence.h"
#include "orbitcpp/orb-cpp/orbitcpp_any.h"
#include <orbit/orb-core/corba-defs.h>

namespace _orbitcpp {

    template <class Traits>
    class SimpleUnboundedSeq: public SequenceBase<Traits>
    {
    public: // public requirement may be a GCC 3.3 bug
	typedef SequenceBase<Traits>    Super;
	
	typedef SimpleUnboundedSeq value_t;
	
	typedef typename Super::size_t   size_t;
	typedef typename Super::index_t  index_t;
	typedef typename Super::buffer_t buffer_t;
	typedef Super                    super_t;

	typedef Sequence_var<SimpleUnboundedSeq> _var_type;
	typedef Sequence_out<SimpleUnboundedSeq> _out_type;
	
    public:
	// Empty constructor
	explicit SimpleUnboundedSeq (size_t max = 0):
	    Super (max)
	    {
	    }
	
	// Create sequence from flat buffer
	SimpleUnboundedSeq (size_t max,
			    size_t length, buffer_t buffer,
			    CORBA::Boolean release = false):
	    Super (max, length, buffer, release)
	    {
	    }
		
	// Copying
	SimpleUnboundedSeq (const super_t &other):
	    Super (other)
	    {
	    }

	// Size requisition
	void _length_set_impl (size_t new_length) {
	    if (new_length > this->_max)
	    {
		buffer_t buffer_tmp = allocbuf (new_length);
		if (!buffer_tmp)
		    throw CORBA::NO_MEMORY ();
		
		for (index_t i = 0; i < this->_length; i++)
		    buffer_tmp[i] = this->_buffer[i];
		
		if (this->_release)
		    freebuf (this->_buffer);
		
		this->_release = true;
		this->_buffer = buffer_tmp;
		this->_max = new_length;
	    }
	    
	    this->_length = new_length;
	}
    };

    template <class Traits, CORBA::ULong max>
    class SimpleBoundedSeq: public SequenceBase<Traits>
    {
    public: // public requirement may be a GCC 3.3 bug
	typedef SequenceBase<Traits>    Super;

	typedef SimpleBoundedSeq value_t;

	typedef typename Super::size_t   size_t;
	typedef typename Super::index_t  index_t;
	typedef typename Super::buffer_t buffer_t;
	typedef Super                    super_t;

	typedef Sequence_out<SimpleBoundedSeq> _out_type;
	typedef Sequence_var<SimpleBoundedSeq> _var_type;
	
    public:
	// Empty constructor
	SimpleBoundedSeq ():
	    Super (max)
	    {
	    }
		
	// Create sequence from flat buffer
	SimpleBoundedSeq (size_t length, buffer_t buffer,
			  CORBA::Boolean release = false):
	    Super (max, length, buffer, release)
	    {
	    }
		
	// Copying
	SimpleBoundedSeq (const super_t &other):
	    Super (other)
	    {
	    }
	
	// Size requisition
	void _length_set_impl (size_t new_length) {
	    g_assert (new_length <= this->_max);
	    
	    this->_length = new_length;
	}
    };

    
} // namespace _orbitcpp



#define ORBITCPP_DECLARE_SIMPLE_SEQUENCE(CPPType, CElem)	\
    typedef ::_orbitcpp::SimpleUnboundedSeq<_orbitcpp::seq_traits_assignable<CElem, CElem, CORBA_sequence_##CElem, &TC_CORBA_sequence_##CElem##_struct> > CPPType; \
    typedef ::_orbitcpp::Sequence_var< CPPType > CPPType##_var;	\
    typedef ::_orbitcpp::Sequence_out< CPPType > CPPType##_out;

#undef ORBITCPP_CREATE_SIMPLE_TRAITS

#endif // !ORBITCPP_SIMPLE_SEQUENCE_HH

