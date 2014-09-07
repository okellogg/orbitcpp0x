/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *  ORBit-C++: C++ bindings for ORBit.
 *
 *  Copyright (C) 2000 Sam Couter
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
 *  Author: Sam Couter <sam@topic.com.au>
 */

#include "orbitcpp/orb-cpp/orbitcpp_typecode.h"
#include "orbitcpp/orb-cpp/orbitcpp_exception.h"
#include <orbit/orb-core/corba-typecode.h>


using namespace _orbitcpp;

//See "C++ Language Mapping Specification", section 1.32 "TypeCode"

CORBA::TypeCode_ptr CORBA::_tc_null =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_null);
CORBA::TypeCode_ptr CORBA::_tc_void =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_void);
CORBA::TypeCode_ptr CORBA::_tc_short =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_short);
CORBA::TypeCode_ptr CORBA::_tc_long =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_long);
CORBA::TypeCode_ptr CORBA::_tc_longlong =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_long_long);
CORBA::TypeCode_ptr CORBA::_tc_ushort =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_unsigned_short);
CORBA::TypeCode_ptr CORBA::_tc_ulong =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_unsigned_long);
CORBA::TypeCode_ptr CORBA::_tc_ulonglong =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_unsigned_long_long);
CORBA::TypeCode_ptr CORBA::_tc_float =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_float);
CORBA::TypeCode_ptr CORBA::_tc_double =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_double);
CORBA::TypeCode_ptr CORBA::_tc_longdouble =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_long_double);
CORBA::TypeCode_ptr CORBA::_tc_boolean =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_boolean);
CORBA::TypeCode_ptr CORBA::_tc_char =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_char);
CORBA::TypeCode_ptr CORBA::_tc_wchar =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_wchar);
CORBA::TypeCode_ptr CORBA::_tc_octet =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_octet);
CORBA::TypeCode_ptr CORBA::_tc_any =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_any);
CORBA::TypeCode_ptr CORBA::_tc_TypeCode =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_TypeCode);
/* There doesn't seem to be a TypeCode defined for CORBA_Principal yet... */
#if 0
CORBA::TypeCode_ptr CORBA::_tc_Principal =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_Principal);
#endif
CORBA::TypeCode_ptr CORBA::_tc_Object =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_Object);
CORBA::TypeCode_ptr CORBA::_tc_string =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_string);
CORBA::TypeCode_ptr CORBA::_tc_wstring =
	reinterpret_cast <CORBA::TypeCode_ptr> (TC_CORBA_wstring);

CORBA::TypeCode_ptr
CORBA::TypeCode::_orbitcpp_wrap (CORBA_TypeCode tc, bool take_copy /* = false */)
{
	if (take_copy)
		tc = (CORBA_TypeCode) _orbitcpp::duplicate_guarded(tc);
	return reinterpret_cast <CORBA::TypeCode_ptr> (tc);
}

bool
CORBA::TypeCode::equal(TypeCode_ptr tc) const
{
	bool retval = false;
	CEnvironment ev;

	retval = CORBA_TypeCode_equal(const_cast <CORBA_TypeCode>(&m_target),
		reinterpret_cast <CORBA_TypeCode>(tc), ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

bool
CORBA::TypeCode::equivalent(TypeCode_ptr tc) const
{
	bool retval = false;
	CEnvironment ev;

	/* not implemented by ORBit */
	ev.propagate_sysex();

	return(retval);
}

CORBA::TCKind
CORBA::TypeCode::kind() const
{
	CORBA::TCKind retval = CORBA::tk_null;
	CEnvironment ev;

	/* compiler warnings force me to use a C-style cast. Bad. */
	retval = (CORBA::TCKind)
		(CORBA_TypeCode_kind(const_cast <CORBA_TypeCode>(&m_target), ev._orbitcpp_cobj()));
	ev.propagate_sysex();

	return(retval);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::get_compact_typecode() const
{
	TypeCode_ptr retval = _tc_null;
	CEnvironment ev;

	/* not implemented by ORBit */
	ev.propagate_sysex();

	return(retval);
}

// for tk_objref, tk_struct, tk_union, tk_enum, tk_alias and tk_except
const char*
CORBA::TypeCode::id() const
{
	const char *retval = NULL;
	CEnvironment ev;

	retval = CORBA_TypeCode_id(const_cast <CORBA_TypeCode>(&m_target), ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

// for tk_objref, tk_struct, tk_union, tk_enum, tk_alias and tk_except
const char*
CORBA::TypeCode::name() const
{
	const char *retval = NULL;
	CEnvironment ev;

	retval = CORBA_TypeCode_name(const_cast <CORBA_TypeCode>(&m_target), ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

// for tk_struct, tk_union, tk_enum and tk_except
CORBA::ULong
CORBA::TypeCode::member_count() const
{
	CORBA::ULong retval = 0;
	CEnvironment ev;

	retval = CORBA_TypeCode_member_count(const_cast <CORBA_TypeCode>
											(&m_target), ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

const char*
CORBA::TypeCode::member_name(ULong index) const
{
	const char *retval = NULL;
	CEnvironment ev;

	retval = CORBA_TypeCode_member_name(const_cast <CORBA_TypeCode>(&m_target),
		index, ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

// for tk_struct, tk_union and tk_except
CORBA::TypeCode_ptr
CORBA::TypeCode::member_type(CORBA::ULong index) const
{
	CORBA::TypeCode_ptr retval = CORBA::_tc_null;
	CEnvironment ev;

	retval = reinterpret_cast <CORBA::TypeCode_ptr>
		(CORBA_TypeCode_member_type(const_cast <CORBA_TypeCode>(&m_target),
			index, ev._orbitcpp_cobj()));
	ev.propagate_sysex();

	return(retval);
}

// for tk_union
// Any *member_label(ULong index) const;
CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type() const
{
	CORBA::TypeCode_ptr retval = CORBA::_tc_null;
	CEnvironment ev;

	retval = reinterpret_cast <CORBA::TypeCode_ptr>
		(CORBA_TypeCode_discriminator_type(const_cast <CORBA_TypeCode>
											(&m_target), ev._orbitcpp_cobj()));
	ev.propagate_sysex();

	return(retval);
}

CORBA::Long
CORBA::TypeCode::default_index() const
{
	CORBA::Long retval = 0;
	CEnvironment ev;

	retval = CORBA_TypeCode_default_index(const_cast <CORBA_TypeCode>
											(&m_target), ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

// for tk_string, tk_sequence and tk_array
CORBA::ULong
CORBA::TypeCode::length() const
{
	CORBA::ULong retval = 0;
	CEnvironment ev;

	retval = CORBA_TypeCode_length(const_cast <CORBA_TypeCode>(&m_target), ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

// for tk_sequence, tk_array and tk_alias
CORBA::TypeCode_ptr
CORBA::TypeCode::content_type() const
{
	CORBA::TypeCode_ptr retval = CORBA::_tc_null;
	CEnvironment ev;

	retval = reinterpret_cast <CORBA::TypeCode_ptr>
		(CORBA_TypeCode_content_type(const_cast <CORBA_TypeCode>(&m_target),
			ev._orbitcpp_cobj()));
	ev.propagate_sysex();

	return(retval);
}

// for tk_fixed
CORBA::UShort
CORBA::TypeCode::fixed_digits() const
{
	CORBA::UShort retval = 0;
	CEnvironment ev;

	retval = CORBA_TypeCode_fixed_digits(const_cast <CORBA_TypeCode>
										(&m_target), ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

CORBA::Short
CORBA::TypeCode::fixed_scale() const
{
	CORBA::Short retval = 0;
	CEnvironment ev;

	retval = CORBA_TypeCode_fixed_scale(const_cast <CORBA_TypeCode>(&m_target),
		ev._orbitcpp_cobj());
	ev.propagate_sysex();

	return(retval);
}

#if 0
CORBA::Visibility
CORBA::TypeCode::member_visibility(ULong index) const
{
	CORBA::Visibility retval = ???;
	CEnvironment ev;

	/* not implemented by ORBit */
	ev.propagate_sysex();

	return(retval);
}
#endif

CORBA::ValueModifier
CORBA::TypeCode::type_modifier() const
{
	CORBA::ValueModifier retval = 0;
	CEnvironment ev;

	/* not implemented by ORBit */
	ev.propagate_sysex();

	return(retval);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::concrete_base_type() const
{
	CORBA::TypeCode_ptr retval = CORBA::_tc_null;
	CEnvironment ev;

	/* not implemented by ORBit */
	ev.propagate_sysex();
	return(retval);
}

CORBA_TypeCode
_orbitcpp::cobj_guarded(CORBA::TypeCode_ptr tc)
{
	return (tc != 0 ? tc->_orbitcpp_cobj() : CORBA_OBJECT_NIL);
}

CORBA_TypeCode
_orbitcpp::duplicate_cobj_guarded(CORBA::TypeCode_ptr tc)
{
	if (tc == 0) return CORBA_OBJECT_NIL;
	return duplicate_guarded(tc->_orbitcpp_cobj());
}
