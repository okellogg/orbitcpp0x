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

#ifndef __ORBITCPP_SMARTPTR_STRING_VAR_HH
#define __ORBITCPP_SMARTPTR_STRING_VAR_HH

namespace _orbitcpp
{

template<class CharT>
struct StringProperties {
};

template<>
struct StringProperties< ::CORBA::Char> {
	typedef CORBA::Char CharT;

	static CharT *alloc(::CORBA::ULong len) {
		return CORBA::string_alloc(len);
	}
	static CharT *dup(CharT const *str) {
		return CORBA::string_dup(str);
	}
	static void free(CharT *str) {
		CORBA::string_free(str);
	}

	static CharT const* convert(char const* s) { return s; }
};

template<>
struct StringProperties< ::CORBA::WChar> {
	typedef CORBA::WChar CharT;

	static CharT *alloc(::CORBA::ULong len) {
		return CORBA::wstring_alloc(len);
	}
	static CharT *dup(CharT const *str) {
		return CORBA::wstring_dup(str);
	}
	static void free(CharT *str) {
		return CORBA::wstring_free(str);
	}
	static CharT* convert(char const* s) 
	{
		return g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
	}
};

template<class CharT>
class String_out;

template<class CharT>
class String_var {
	friend class String_out<CharT>;
protected:
	CharT								*m_data;
	typedef StringProperties<CharT>		Properties;

public:
	String_var()
		: m_data(NULL) {
	}
	String_var(CharT *p)
		: m_data(p) {
	}
	String_var(const CharT *p)
		: m_data(Properties::dup(p)) {
	}
	String_var(const String_var &s)
		: m_data(Properties::dup(s.m_data)) {
	}
	~String_var() {
		Properties::free(m_data);
		m_data = NULL;
		//_orbitcpp::point_to_memhow_none((gpointer*)&m_data);
	}

	String_var &operator=(CharT *p) {
		Properties::free(m_data);
		m_data = p;
  		return *this;
	}
	String_var &operator=(const CharT *p) {
		Properties::free(m_data);
  		m_data = Properties::dup(p);
  		return *this;
  	}
	String_var &operator=(const String_var &s) {
		Properties::free(m_data);
  		m_data = Properties::dup(s.m_data);
  		return *this;
	}
	/*
	operator CharT*() {
		return m_data;
  	}
	*/
	operator const CharT*() const {
		return m_data;
  	}

	const CharT* in() const {
		return m_data;
	}
	CharT *&inout() {
		return m_data;
  	}
	CharT *&out() {
		Properties::free(m_data);
  		m_data = NULL;
	  	return m_data;
	}
	CharT *_retn() {
		CharT *temp = m_data;
		Properties::free(m_data);
  		m_data = NULL;
  		return temp;
	}

	CharT &operator[](CORBA::ULong index) {
		return m_data[index];
  	}
	CharT operator[](CORBA::ULong index) const {
		return m_data[index];
  	}
};

} //namespace _orbitcpp

#endif //__ORBITCPP_SMARTPTR_STRING_VAR_HH
