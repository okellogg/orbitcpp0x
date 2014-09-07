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
#include <string>

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
			    bool release = false):
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
			  bool release = false):
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

    // Only for use by IDL2CPP0X
    class StringUnboundedSeq
    {
    public:
	typedef StringUnboundedSeq& _out_type;

	typedef StringUnboundedSeq self_t;
	typedef std::string elem_t;
	typedef CORBA_string c_elem_t;
	typedef CORBA_sequence_CORBA_string c_seq_t;
	typedef elem_t* buffer_t;

	typedef CORBA::ULong  size_t;
	typedef CORBA::ULong  index_t;

    protected:
	size_t   _max;
	size_t   _length;
	buffer_t _buffer;
	bool     _release;

    public:
	// Empty constructor
	explicit StringUnboundedSeq (size_t max = 0):
	    _max (max),
	    _length (0),
	    _buffer (max ? new elem_t[max] : 0),
	    _release (true)
	    {
	    }

	// Create sequence from flat buffer
	StringUnboundedSeq (size_t max,
		      size_t length, buffer_t buffer,
		      bool release = false):
	    _max (max),
	    _length (length),
	    _buffer (buffer),
	    _release (release)
	    {
		g_assert (length <= max);
	    }

	// Memory managment
	static buffer_t allocbuf (size_t num_elems) { return new elem_t[num_elems]; };
	static void     freebuf  (buffer_t buffer)  { delete[] buffer;               };

	// Copy constructor
	StringUnboundedSeq (const self_t &src):
	    _max (src._max),
	    _length (src._length),
	    _release (true)
	    {
		_buffer = allocbuf (_max);
		if (!_buffer)
		    throw CORBA::NO_MEMORY ();

		for (index_t i = 0; i < _length; i++)
		    _buffer[i] = src._buffer[i];
	    }

	// Copy operator
	self_t & operator= (const self_t &src) {
	    buffer_t buffer_tmp = 0;
	    
	    if (src._max != 0) {
		buffer_tmp = allocbuf (src._max);
		if (!buffer_tmp)
		    throw CORBA::NO_MEMORY ();
	    }
	    
	    for (index_t i = 0; i < src._length; i++)
		buffer_tmp[i] = src._buffer[i];
	    
	    _length = src._length;
	    _max = src._max;

	    if (_release)
		freebuf (_buffer);
	    _buffer = buffer_tmp;
	    
	    return *this;
	}

	// Destructor
	virtual ~StringUnboundedSeq () {
	    if (_release)
		freebuf (_buffer);
	}

	// Size information
	size_t maximum () const { return _max;    };
	size_t length () const  { return _length; };
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
	void length (size_t new_length) { _length_set_impl (new_length); };

	// Element access
	elem_t& operator[] (index_t index)             { return _buffer[index]; };
	const elem_t& operator[] (index_t index) const { return _buffer[index]; };
	const elem_t* get_buffer() const { return _buffer; }
	elem_t* get_buffer(bool orphan = 0)
	{
		g_assert(!orphan);
		return _buffer;
	}

	static void pack_elem (const std::string& cpp_value, CORBA_string& c_value)
	{
	    const char *ptr = cpp_value.c_str ();
	    c_value = CORBA_string_dup (ptr);
	}

	static void unpack_elem (std::string& cpp_value, const CORBA_string& c_value)
	{
	    cpp_value = std::string (c_value);
	}

	// ORBit2/C++ extension: create C sequence
	c_seq_t* _orbitcpp_pack () const {
	    c_seq_t *retval = CORBA_sequence_CORBA_string__alloc ();
	    _orbitcpp_pack (*retval);
	    return retval;
	}

	// ORBit2/C++ extension: fill C sequence
	void _orbitcpp_pack (c_seq_t &c_seq) const {
	    c_seq._length = _length;
	    if (c_seq._release)
		CORBA_free (c_seq._buffer);
	    c_seq._buffer = CORBA_sequence_CORBA_string_allocbuf (_length);
	    c_seq._release = 1;
	    
	    for (index_t i = 0; i < _length; i++)
		pack_elem (_buffer[i], c_seq._buffer[i]);
	}

	// ORBit2/C++ extension: fill C++ sequence from C sequence
	void _orbitcpp_unpack (const c_seq_t &c_seq) {
	    length (c_seq._length);
	    for (index_t i = 0; i < c_seq._length; i++)
		unpack_elem (_buffer[i], c_seq._buffer[i]);
	}

	static void pack_elem (const self_t &cpp_value, c_seq_t &c_value)
	{
		cpp_value._orbitcpp_pack(c_value);
	}

	static void unpack_elem (self_t &cpp_value, const c_seq_t &c_value)
	{
		cpp_value._orbitcpp_unpack(c_value);
	}

    };

    
} // namespace _orbitcpp



#define ORBITCPP_DECLARE_SIMPLE_SEQUENCE(CPPType, CElem)	\
    typedef ::_orbitcpp::SimpleUnboundedSeq<_orbitcpp::seq_traits_assignable<CElem, CElem, CORBA_sequence_##CElem, &TC_CORBA_sequence_##CElem##_struct> > CPPType; \
    typedef ::_orbitcpp::Sequence_var< CPPType > CPPType##_var;	\
    typedef ::_orbitcpp::Sequence_out< CPPType > CPPType##_out;

#undef ORBITCPP_CREATE_SIMPLE_TRAITS

#endif // !ORBITCPP_SIMPLE_SEQUENCE_HH

