/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 *	ORBit-C++: C++ bindings for ORBit.
 *
 *	Copyright (C) 2000-2002 The orbitcpp team
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Library General Public
 *	License as published by the Free Software Foundation; either
 *	version 2 of the License, or (at your option) any later version.
 *
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *	Library General Public License for more details.
 *
 *	You should have received a copy of the GNU Library General Public
 *	License along with this library; if not, write to the Free
 *	Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __ORBITCPP_SMARTPTR_STRING_OUT_HH
#define __ORBITCPP_SMARTPTR_STRING_OUT_HH

#include "orbitcpp/orb-cpp/smartpointers/string_var.h"

namespace _orbitcpp
{

template<class CharT>
class String_out {
protected:
	CharT								*&m_data;
	typedef StringProperties<CharT>		Properties;

public:
	String_out(CharT *&p)
		: m_data(p) {
  		m_data = NULL;
  	}
	String_out(String_var<CharT> &p)
		: m_data(p.m_data) {
  		Properties::free(m_data);
  		m_data = NULL;
  	}
	String_out(String_out const & s)
		: m_data(s.m_data) {
  	}

	String_out &operator=(String_out const &s) {
		m_data = s.m_data;
  		return *this;
  	}
	String_out &operator=(CharT *p) {
		m_data = p;
  		return *this;
	}
	String_out &operator=(const CharT *p) {
		m_data = Properties::dup(p);
  		return *this;
	}

	operator CharT *&() {
		return m_data;
  	}
	operator CharT **() { // nonstandard
		return &m_data;
	}
	CharT *&ptr() {
		return m_data;
  	}

  private:
	// assignment from String_var disallowed
	void operator=(const String_var<CharT> &s);
};
  
} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_STRING_OUT_HH
