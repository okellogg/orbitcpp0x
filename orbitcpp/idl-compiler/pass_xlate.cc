/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
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
 *  Authors:	Andreas Kloeckner <ak@ixion.net>
 *              Phil Dawes <philipd@users.sourceforge.net>
 *				John K. Luebs <jkluebs@marikarpress.com>
 *
 *  Purpose: idl -> c++ translation pass
 *
 */




#include "orbitcpp/idl-compiler/error.h"
#include "orbitcpp/idl-compiler/pass_xlate.h"
#include <cstdlib>

#include "types/IDLTypedef.h"
#include "orbitcpp/idl-compiler/types/IDLOperation.h"
#include "orbitcpp/idl-compiler/types/IDLAttribAccessor.h"


// IDLPassXlate --------------------------------------------------------------
void 
IDLPassXlate::runPass() {
	m_header << "#ifndef ORBITCPP_IDL_" << idlUpper(m_state.m_basename) << "_COMMON" << endl
		 << "#define ORBITCPP_IDL_" << idlUpper(m_state.m_basename) << "_COMMON" << endl
		 << endl << endl
		 << "#include <orbitcpp/orb-cpp/orbitcpp.h>" << endl
		 << endl
		 << "#include <cstring>" << endl
		 << "#include \"" << m_state.m_basename << ".h\"" << endl;

	m_module << "#include \"" << m_state.m_basename << IDL_CPP_STUB_HEADER_EXT << "\"" << endl
		 << endl << endl;
	
	m_header << endl << endl
		 << "// Type mapping ----------------------------------------" << endl
		 << endl;

	doDefinitionList(m_state.m_rootscope.getNode(),m_state.m_rootscope);
	runJobs();
	
	m_header << endl << "#endif" << endl;
}


void 
IDLPassXlate::doTypedef (IDL_tree  node,
			 IDLScope &scope)
{
	string id;

	bool first_dcl = true;
	
	for (IDL_tree dcl_list = IDL_TYPE_DCL(node).dcls;
	     dcl_list; dcl_list = IDL_LIST(dcl_list).next)
	{
		IDLTypedef& td = *new IDLTypedef(IDL_LIST(dcl_list).data);
		
#if 0 //!!!
		if (first_dcl)
		{
			ORBITCPP_MEMCHECK( new IDLWriteCPPSpecCode(td.getAlias(), m_state, *this) );
			first_dcl = false;
		}
#endif
		const IDLType &targetType = td.getAlias();
		targetType.typedef_decl_write (m_header, indent, m_state, td);

		element_write_typecode (td);
	}
}

void 
IDLPassXlate::doStruct (IDL_tree  node,
			IDLScope &scope)
{
	IDLStruct strct (node);

	// Create forward declaration
	m_header << indent;
	m_header << "struct ";
	m_header << strct.get_cpp_identifier () << ";" << endl << endl;

	// Create _out and _var typedef
	struct_create_typedefs (strct);
	m_header << endl;
	
	// Create the struct itself
	m_header << indent;
	m_header << "struct ";

	++indent;
	struct_create_traits (strct);
	struct_create_members (strct);
	struct_create_converters (strct);
	
	// End struct definition
	m_header << --indent << "};" << endl << endl;
	
	// Create typecode and Any stuff
	struct_create_any (strct);

	m_header << endl;
}

void IDLPassXlate::struct_create_traits (const IDLCompoundSeqElem &strct)
{
	m_header << --indent << "public:" << endl;
	++indent;
	// Typedefs
	m_header << indent << "typedef " << strct.get_cpp_member_typename ()
		 << " value_t;" << endl;
	m_header << indent << "typedef " << strct.get_c_typename ()
		 << " c_value_t;" << endl;

	// C++ -> C packer
	m_header << indent << "static void pack_elem "
		 << "(const value_t &cpp_elem, c_value_t &c_elem);" << endl;

	m_module << "void " << strct.get_cpp_method_prefix ()
		 << "::pack_elem "
		 << "(const value_t &cpp_elem, c_value_t &c_elem)" << endl
		 << "{" << endl;
	strct.member_pack_to_c (m_module, ++mod_indent, "cpp_elem", "c_elem");
	m_module << --mod_indent << "}" << endl << endl;

	// C -> C++ unpacker
	m_header << indent << "static void unpack_elem "
		 << "(value_t &cpp_elem, const c_value_t &c_elem);" << endl;

	m_module << "void " << strct.get_cpp_method_prefix ()
		 << "::unpack_elem "
		 << "(value_t &cpp_elem, const c_value_t &c_elem)" << endl
		 << "{" << endl;
	strct.member_unpack_from_c (m_module, ++mod_indent, "cpp_elem", "c_elem");
	m_module << --mod_indent << "}" << endl << endl;
}

void IDLPassXlate::struct_create_members (const IDLStruct &strct)
{
	strct.write_member_decls (m_header, indent);

	
	// Create default constructor
	m_header << indent << strct.get_cpp_identifier () << "();" << endl;

	m_module << strct.get_cpp_method_prefix ()
		 << "::" << strct.get_cpp_identifier () << "()" << endl
		 << "{" << endl;
	++mod_indent;
	
	for (IDLStruct::const_iterator i = strct.begin (); i != strct.end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;

		member.getType ()->member_init_cpp (m_module, mod_indent,
						    member.get_cpp_identifier ());
	}

	m_module << --mod_indent << "}" << endl << endl;
}

void IDLPassXlate::struct_create_converters (const IDLStruct &strct)
{
	// Create C->C++ constructor
	string construct_arg = "const " + strct.get_c_typename () + " &_c_struct";
	
	m_header << indent << "explicit " << strct.get_cpp_identifier ()
		 << " (" << construct_arg << ");" << endl << endl;

	m_module << strct.get_cpp_method_prefix ()
		 << "::" << strct.get_c_identifier ()
		 << " (" << construct_arg << ")" << endl;
	m_module << "{" << endl;
	++mod_indent;
	for (IDLStruct::const_iterator i = strct.begin (); i != strct.end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;

		member.getType ()->member_init_cpp (m_module, mod_indent,
						    member.get_cpp_identifier ());
	}
	m_module << mod_indent << "_orbitcpp_unpack (_c_struct);" << endl;
	m_module << --mod_indent << "}" << endl << endl;

	// Create packers
	strct.write_packing_decl (m_header, indent);
	strct.write_packing_impl (m_module, mod_indent);
}

void IDLPassXlate::struct_create_typedefs (const IDLStruct &strct)
{
	const string data_prefix = IDL_IMPL_NS "::Data";
	const string data_var = data_prefix + "_var< " + strct.get_cpp_identifier () + ">";

	m_header << indent << "typedef " << data_var << " "
		 << strct.get_cpp_identifier () << "_var;"
		 << endl;

	if (strct.is_fixed ())
	{
		m_header << indent << "typedef "
			 << strct.get_cpp_identifier () << "& "
			 << strct.get_cpp_identifier () << "_out;"
			 << endl;
	} else {
		const string data_out = data_prefix + "_out< " + strct.get_cpp_identifier () + ">";
		m_header << indent << "typedef " << data_out << " "
			 << strct.get_cpp_identifier () << "_out;"
			 << endl;
	}
}

void IDLPassXlate::struct_create_any (const IDLStruct &strct)
{
	element_write_typecode (strct);

	ORBITCPP_MEMCHECK (new IDLWriteStructAnyFuncs (strct, m_state, *this));
}


void 
IDLPassXlate::doUnion(IDL_tree node,IDLScope &scope)
{
	IDLUnion               un (node);
	
	m_header << indent << "class " << un.get_cpp_identifier () << endl
		 << indent << "{" << endl;
	m_header << indent << "private:" << endl;
	m_header << ++indent << un.get_c_typename () << " " << (un.is_fixed () ? "": "*")
		 << "m_target;" << endl << endl;

	struct_create_traits(un);
	// Create constructors and destructors
	union_create_constructor (un);
	
	// Create internally used methods
	union_create_internal (un);
	
	m_header << --indent << "public:" << endl;
	++indent;
	
	// Create discriminator handler code
	union_create_discr (un);

	// Create member accessors
	union_create_members (un);

	// Create converters
	union_create_converters (un);
	
	m_header << --indent << "};" << endl << endl;
	
#if 0
	if(idlUnion.hasExplicitDefault() == false){
		m_header
		<< indent << "void _default()" << endl
		<< indent << "{" << endl;

		m_header
		<< ++indent << "_clear_member();" << endl
		<< indent << "_d("<< idlUnion.getDefaultDiscriminatorValue() << ");" << endl;
		m_header	
		<< --indent << "}" << endl;
	}
#endif

	// Create smart ptr typedefs
	union_create_typedefs (un);

	union_create_any (un);
}


void
IDLPassXlate::union_create_constructor (const IDLUnion &un)
{
	m_header << --indent << "public: " << endl;
	++indent;
	
	// Create default constructor
	m_header << indent << un.get_cpp_identifier () << "();" << endl;
	m_module << un.get_cpp_method_prefix ()
		 << "::" << un.get_cpp_identifier () << " ()";
	if (un.is_fixed ())
		m_module << endl;
	else {
		m_module << " :" << endl;
		m_module << ++mod_indent << "m_target ("
			 << un.get_c_typename () << "__alloc ())" << endl;
		--mod_indent;
	}

	m_module << "{" << endl << "}" <<
		endl << endl;

	// Destructor
	m_header << indent << "~" << un.get_cpp_identifier ()
		 << " ();" << endl << endl;
	
	m_module << un.get_cpp_method_prefix ()
		 << "::~" << un.get_cpp_identifier () << " ()" << endl
		 << "{" << endl;
	m_module << ++mod_indent << "_clear_member ();" << endl;
	m_module << --mod_indent << "}" << endl << endl;
}

void
IDLPassXlate::union_create_internal (const IDLUnion &un)
{
	// Wrapper constructor
	m_header << indent << "explicit " << un.get_cpp_identifier ()
		 << " (const " << un.get_c_typename () << " &_c_un);"
		 << endl << endl;

	m_module << un.get_cpp_method_prefix ()
		 << "::" << un.get_cpp_identifier ()
		 << " (const " << un.get_c_typename () << " &_c_un)" << endl;
	if (!un.is_fixed ())
	{
		m_module << ++mod_indent << ": m_target ("
			 << un.get_c_typename () << "__alloc ())" << endl;
		--mod_indent;
	}
	m_module << "{" << endl;
	m_module << ++mod_indent << "_orbitcpp_unpack (_c_un);" << endl;
	m_module << --mod_indent << "}" << endl << endl;

	// Assignment constructor
	m_header << indent << un.get_cpp_identifier ()
		 << " (const " << un.get_cpp_identifier () << " &un);"
		 << endl;

	m_module << un.get_cpp_method_prefix ()
		 << "::" << un.get_cpp_identifier ()
		 << " (const " << un.get_cpp_identifier () << " &un)" << endl;
	if (!un.is_fixed ())
	{
		m_module << ++mod_indent << ": m_target ("
			 << un.get_c_typename () << "__alloc ())" << endl;
		--mod_indent;
	}
	m_module << "{" << endl;
	++mod_indent;
	union_create_copier (un);
	m_module << --mod_indent << "}" << endl << endl;

	// Assignment operator
	m_header << indent << un.get_cpp_identifier ()
		 << " & operator= (const " << un.get_cpp_identifier () << " &un);"
		 << endl << endl;

	m_module << un.get_cpp_typename () << " & "
		 << un.get_cpp_method_prefix () << "::operator="
		 << " (const " << un.get_cpp_identifier () << " &un)" << endl;
	m_module << "{" << endl;
	++mod_indent;
	union_create_copier (un);
	m_module << endl;
	m_module << mod_indent << "return *this;" << endl;
	m_module << --mod_indent << "}" << endl << endl;
	
	// _clear_member
	m_header << --indent << "private:" << endl;
	++indent;
	m_header << indent << "void _clear_member ();" << endl << endl;
	
	m_module << "void " << un.get_cpp_method_prefix ()
		 << "::_clear_member ()" << endl
		 << "{" << endl;
	++mod_indent;
	if (un.is_fixed ())
		m_module << mod_indent << "// Do nothing" << endl;
	else
		m_module << mod_indent << un.get_c_method_prefix () << "__freekids ("
			 << "m_target, 0);" << endl;
	m_module << --mod_indent << "}" << endl << endl;
}


void
IDLPassXlate::union_create_copier (const IDLUnion &un)
{
	m_module << mod_indent << "switch (un._d ())" << endl
		 << mod_indent << "{" << endl;
	for (IDLUnion::const_iterator i = un.begin (); i != un.end (); ++i)
	{
		const IDLCaseStmt &case_stmt = static_cast<const IDLCaseStmt&> (**i);
		const IDLMember   &member = case_stmt.get_member ();
		const string member_name = member.get_cpp_identifier ();
		
		if (!case_stmt.isDefault ())
			m_module << mod_indent << "case "
				 << *(case_stmt.labelsBegin ()) << ":" << endl;
		else
			m_module << mod_indent << "default:" << endl;

		++mod_indent;
		m_module << mod_indent << member_name << " ("
			 << "un." << member_name << " ());" << endl
			 << mod_indent << "break;" << endl;
		--mod_indent;
	}
	
	m_module << mod_indent << "}" << endl;
}


void
IDLPassXlate::union_create_discr (const IDLUnion &un)
{
	const IDLUnionDiscriminator  &d = un.get_discriminator ();

	const string d_cpp = d.discr_get_cpp_typename ();
	const string d_c = d.discr_get_c_typename ();
	
	// Get discriminator
	m_header << indent << d_cpp << " _d () const;" << endl;

	m_module << d_cpp << " "
		 << un.get_cpp_method_prefix () << "::_d () const" << endl
		 << "{" << endl;
	++mod_indent;
	if (un.is_fixed ())
		m_module << mod_indent << "return (" << d_cpp << ")" << "m_target._d;" << endl;
	else
		m_module << mod_indent << "return (" << d_cpp << ")" << "m_target->_d;" << endl;
	m_module << --mod_indent << "}" << endl << endl;

	// Set discriminator
	m_header << indent << "void _d (" << d_cpp << " d);" << endl << endl;
	
	m_module << "void " << un.get_cpp_method_prefix ()
		 << "::_d (" << d_cpp << " d)" << endl
		 << "{" << endl;
	++mod_indent;
	if (un.is_fixed ())
		m_module << mod_indent << "m_target._d = (" << d_c << ") d;" << endl;
	else
		m_module << mod_indent << "m_target->_d = (" << d_c << ") d;" << endl;
	m_module << --mod_indent << "}" << endl << endl;
}

void
IDLPassXlate::union_create_members (const IDLUnion &un)
{
	for (IDLUnion::const_iterator i = un.begin (); i != un.end (); ++i)
	{

		const IDLCaseStmt &case_stmt = dynamic_cast<const IDLCaseStmt&> (**i);
		const IDLMember   &member = case_stmt.get_member ();
		member.getType ()->create_union_accessors(un, case_stmt, m_header, indent, m_module, mod_indent);
	}
}

void
IDLPassXlate::union_create_converters (const IDLUnion &un)
{
	string c_type = un.get_c_typename ();
	string method_pref = un.get_cpp_method_prefix ();

	// FIXME: Since we already store an internal C struct, maybe
	// we could short-circuit all these conversions -- Cactus

	// _orbitcpp_pack that returns a newly allocated C structure on heap
	m_header << indent << c_type << "* _orbitcpp_pack () const;" << endl;

	m_module << c_type << "* " << method_pref
		 << "::_orbitcpp_pack () const" << endl
		 << "{" << endl;
	m_module << ++mod_indent << c_type << " *_c_un = "
		 << c_type << "__alloc ();" << endl;
	m_module << mod_indent << "if (!_c_un)" << endl;
	m_module << ++mod_indent << "throw CORBA::NO_MEMORY ();" << endl << endl;
	m_module << --mod_indent << "_orbitcpp_pack (*_c_un);" << endl;
	m_module << mod_indent << "return _c_un;" << endl;
	m_module << --mod_indent << "}" << endl << endl;
	
	// _orbitcpp_pack that works on an existing C struct
	m_header << indent << "void _orbitcpp_pack ("
		 << c_type << " &_c_un) const;" << endl;

	m_module << "void " << method_pref
		 << "::_orbitcpp_pack (" << c_type << " &_c_un) const" << endl
		 << "{" << endl;
	++mod_indent;
	if (un.is_fixed ())
		m_module << mod_indent << "_c_un._d = m_target._d;" << endl << endl;
	else
		m_module << mod_indent << "_c_un._d = m_target->_d;" << endl << endl;
		
	m_module << mod_indent << "switch (_d ())" << endl
		 << mod_indent << "{" << endl;

	for (IDLUnion::const_iterator i = un.begin (); i != un.end (); ++i)
	{
		const IDLCaseStmt &case_stmt = static_cast<const IDLCaseStmt&> (**i);
		const IDLMember   &member = case_stmt.get_member ();

		const string member_cpp = member.get_cpp_identifier () + "()";
		const string member_c = "_c_un._u." + member.get_c_identifier ();

		// Write case branches
		for (IDLCaseStmt::const_iterator j = case_stmt.labelsBegin ();
		     j != case_stmt.labelsEnd (); ++j)
		{
			m_module << mod_indent << "case " << *j << ":" << endl;
		}
		if (case_stmt.isDefault ())
			m_module << mod_indent << "default:" << endl;
		
		// Pack member to C union
		++mod_indent;
		member.getType ()->member_pack_to_c (m_module, mod_indent,
						     member_cpp, member_c);
		m_module << mod_indent << "break;" << endl;
		--mod_indent;
	}
	
	m_module << mod_indent << "}" << endl;
	m_module << --mod_indent << "}" << endl << endl;

	
	// _orbitcpp_unpack for converting a C union back to a C++ one
	m_header << indent << "void _orbitcpp_unpack (const "
		 << c_type << " &_c_un);" << endl << endl;

	m_module << "void " << method_pref
		 << "::_orbitcpp_unpack (const " << c_type << " &_c_un)" << endl
		 << "{" << endl;
	m_module << ++mod_indent << "_clear_member ();" << endl;
	
	// Declare temporary variables
	for (IDLUnion::const_iterator i = un.begin (); i != un.end (); ++i)
	{
		const IDLCaseStmt &case_stmt = static_cast<const IDLCaseStmt&> (**i);
		const IDLMember   &member = case_stmt.get_member ();

		m_module << mod_indent << member.getType ()->get_cpp_member_typename ()
			 << " _cpp_" << member.get_cpp_identifier ()
			 << ";" << endl;
	}
	m_module << endl;
		
	m_module << mod_indent << "switch ("
		 << "(" << un.get_discriminator ().discr_get_cpp_typename () << ")"
		 << "_c_un._d)" << endl
		 << mod_indent << "{" << endl;
	
	for (IDLUnion::const_iterator i = un.begin (); i != un.end (); ++i)
	{
		const IDLCaseStmt &case_stmt = static_cast<const IDLCaseStmt&> (**i);
		const IDLMember   &member = case_stmt.get_member ();

		const string member_cpp = "_cpp_" + member.get_cpp_identifier ();
		const string member_c = "_c_un._u." + member.get_c_identifier ();
		const string set_cpp = member.get_cpp_identifier () + " (" + member_cpp + ")";
		
		// Write case branches
		for (IDLCaseStmt::const_iterator j = case_stmt.labelsBegin ();
		     j != case_stmt.labelsEnd (); ++j)
		{
			m_module << mod_indent << "case " << *j << ":" << endl;
		}
		if (case_stmt.isDefault ())
			m_module << mod_indent << "default:" << endl;

		// Pack member to C union
		++mod_indent;
		member.getType ()->member_unpack_from_c (m_module, mod_indent,
							 member_cpp, member_c);
		m_module << mod_indent << set_cpp << ";" << endl;
		m_module << mod_indent << "break;" << endl;
		--mod_indent;
	}
	
	m_module << mod_indent << "}" << endl;
	m_module << --mod_indent << "}" << endl << endl;
}

void
IDLPassXlate::union_create_typedefs (const IDLUnion &un)
{
	if (un.is_fixed ())
	{
		m_header << indent << "typedef "
			 << un.get_cpp_identifier () << "& "
			 << un.get_cpp_identifier () << "_out;"
			 << endl;
	} else {
		const string data_prefix = IDL_IMPL_NS "::Data";
		const string data_var = data_prefix + "_var< " + un.get_cpp_identifier () + ">";
		const string data_out = data_prefix + "_out< " + un.get_cpp_identifier () + ">";
		
		m_header << indent << "typedef " << data_var << " "
			 << un.get_cpp_identifier () << "_var;"
			 << endl;

		m_header << indent << "typedef " << data_out << " "
			 << un.get_cpp_identifier () << "_out;"
			 << endl;
	}
}

void IDLPassXlate::union_create_any (const IDLUnion &un)
{
	element_write_typecode (un);

	ORBITCPP_MEMCHECK (new IDLWriteUnionAnyFuncs (un, m_state, *this));
}

void 
IDLPassXlate::doEnum (IDL_tree  node,
		      IDLScope &scope)
{
	IDLEnum idlEnum (node);


	m_header << indent << "enum  "
		<< idlEnum.get_cpp_identifier () << " {\n";

	for (IDLEnum::const_iterator i = idlEnum.begin ();
	     i != idlEnum.end (); i++)
	{
		m_header
			<< indent
			<< (*i)->get_cpp_identifier() << " = "
			<< (*i)->get_c_typename() << ",\n ";
	}
	m_header << indent << "};\n";
	
	m_header << indent << "typedef " << idlEnum.get_cpp_identifier () << "& "
		 << idlEnum.get_cpp_identifier () << "_out;" << endl << endl;

	element_write_typecode (idlEnum);

	ORBITCPP_MEMCHECK(new IDLWriteEnumAnyFuncs (idlEnum, m_state, *this));
}


void 
IDLPassXlate::doNative (IDL_tree  node,
			IDLScope &scope)
{
	ORBITCPP_NYI("native")
}




void 
IDLPassXlate::doConstant (IDL_tree  node,
			  IDLScope &scope)
{
	IDLConstant &cns = (IDLConstant &) *scope.getItem(node);

	// undef the C constant #define
	m_header << "#undef " << cns.get_c_identifier () << endl;
	
	m_header << indent;
	if (cns.interface_child())
		m_header  << "static ";

	cns.getType ()->const_decl_write (m_header, m_module, scope, indent,
					  cns.get_cpp_identifier (),
					  cns.getValue ());
}



void 
IDLPassXlate::doAttribute(IDL_tree node,IDLScope &scope) {
}



void 
IDLPassXlate::doOperation(IDL_tree node,IDLScope &scope) {
}




void 
IDLPassXlate::doException (IDL_tree  node,
			   IDLScope &scope)
{
	IDLException &except = (IDLException &) *scope.getItem(node);

#if 0 //!!!
	// spec code must be generated before the exception because the exception "uses" its
	// members
	IDLException::iterator first = except.begin(),last = except.end();
	while (first != last) {
		IDLMember &member = (IDLMember &) **first++;
 		member.getType()->writeCPPSpecCode(m_header,indent,m_state);
	}
#endif

	m_header << indent << "class " << except.get_cpp_identifier ()
		 << " : public " IDL_CORBA_NS "::UserException" << endl
		 << indent << "{" << endl;

	m_header << indent << "public:" << endl;
	indent++;

	exception_create_members (except);

	m_header << endl << indent << "// methods" << endl;

	exception_create_constructors (except);
	
	m_header << indent << "void _raise ()" << endl
		 << indent << "{" << endl;
	m_header << ++indent << "throw *this;" << endl;
	m_header << --indent << '}' << endl << endl;

	m_header << indent << "static " << except.get_cpp_identifier () << " *_narrow "
		 << "(" << IDL_CORBA_NS "::Exception *ex)" << endl
		 << indent << "{" << endl;
	m_header << ++indent << "return dynamic_cast"
		 << "<" << except.get_cpp_identifier () << "*> (ex)"
		 << ';' << endl;
	m_header << --indent << '}' << endl;

	exception_create_converters (except);
	
	m_header << --indent << "};" << endl << endl;

	exception_create_any (except);	
}

void
IDLPassXlate::exception_create_members (const IDLException &ex)
{
	ex.write_member_decls (m_header, indent);
}

void
IDLPassXlate::exception_create_constructors (const IDLException &ex)
{
	m_header << indent << "// copy ctor, dtor and assignment op will be auto-generated" << endl
		 << indent << ex.get_cpp_identifier () << "() { }" << endl;

	if (ex.size () == 0)
		return;

	// Create member init constructor
	
	string constructor_args;
	IDLException::const_iterator back = ex.end ();
	back--;
	for (IDLException::const_iterator i = ex.begin ();
	     i != ex.end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;

		constructor_args += member.getType ()->member_decl_arg_get ();
		constructor_args += " _par_";
		constructor_args += member.get_cpp_identifier ();

		if (i != back)
			constructor_args += ", ";
	}

	m_header << indent << ex.get_cpp_identifier ()
		 << " (" << constructor_args << ")"
		 << ';' << endl << endl;
	
	m_module << mod_indent << ex.get_cpp_method_prefix ()
		 << "::" << ex.get_cpp_identifier ()
		 << " (" << constructor_args << ")" << endl
		 << mod_indent << '{' << endl;
	++mod_indent;

	for (IDLException::const_iterator i = ex.begin ();
	     i != ex.end (); i++)
	{
		IDLMember &member = (IDLMember &) **i;

		member.getType ()->member_impl_arg_copy (m_module, mod_indent,
							 member.get_cpp_identifier ());
	}
		
	m_module << --mod_indent << '}' << endl << endl;
}

void
IDLPassXlate::exception_create_converters (const IDLException &ex)
{
	m_header << indent << "virtual char const *_orbitcpp_get_repoid() const;\n";
	m_module << mod_indent << "char const* " << ex.get_cpp_method_prefix () << "::_orbitcpp_get_repoid() const\n";
	m_module << mod_indent++ << "{\n";
	m_module << "return \"" << ex.getRepositoryId() << "\";\n";
	m_module << --mod_indent << "}\n";
	

	// _orbitcpp_set
	m_header << indent << "void _orbitcpp_set (::CORBA_Environment *ev);" << endl;
	
	m_module << "void " << ex.get_cpp_method_prefix ()
		 << "::_orbitcpp_set (::CORBA_Environment *ev)"
		 << '{' << endl;
	++mod_indent;
	if (ex.size ())
	{
		m_module << mod_indent <<  "::CORBA_exception_set (ev, ::CORBA_USER_EXCEPTION, "
			 << '"' << ex.getRepositoryId() << '"'
			 << ", _orbitcpp_pack ())"
			 << ';' << endl;
	} else {
		m_module << mod_indent <<  "::CORBA_exception_set (ev, ::CORBA_USER_EXCEPTION, "
			 << '"' << ex.getRepositoryId() << '"'
			 << ", 0)"
			 << ';' << endl;
	}
	m_module << --mod_indent << '}' << endl << endl;

	// _orbitcpp_pack
	ex.write_packing_decl (m_header, indent);
	ex.write_packing_impl (m_module, mod_indent);
}

void
IDLPassXlate::exception_create_any (const IDLException &ex)
{
	element_write_typecode (ex);

	ORBITCPP_MEMCHECK (new IDLWriteExceptionAnyFuncs(ex, m_state, *this));
}


void 
IDLPassXlate::doForwardDcl (IDL_tree  node,
			   IDLScope &scope)
{
	std::string id = IDL_IDENT(IDL_FORWARD_DCL(node).ident).str;
	IDLInterface &iface = (IDLInterface &) *scope.getItem(id);

	doForwardDcl(iface);
}

void
IDLPassXlate::doForwardDcl(IDLInterface &iface)
{
	// Get namespace information
	string ns_iface_begin, ns_iface_end;
	iface.getCPPNamespaceDecl(ns_iface_begin, ns_iface_end);

	bool non_empty_ns = ns_iface_end.size () || ns_iface_begin.size ();

	string ifname = iface.get_cpp_identifier();
	string _ptrname = iface.get_cpp_identifier_ptr();
	
	// Create forward declaration
	m_header << indent << "class " << ifname << ';' << endl;

	// Leave module namespace
	if (non_empty_ns)
		m_header << ns_iface_end;

	// Enter POA namespace
	string ns_poa_begin, ns_poa_end;
	iface.get_cpp_poa_namespace (ns_poa_begin, ns_poa_end);

	// predeclare POA type (necessary for typedef'ing)	
	if (non_empty_ns)
		m_header << ns_poa_begin << endl << endl;
	
	m_header << indent << "class " << iface.get_cpp_poa_identifier () << ';' << endl;
	
	if (non_empty_ns)
		m_header << ns_poa_end;
	
	// Re-enter module namespace
	if (non_empty_ns)
		m_header << ns_iface_begin;

	// Write typedefs
	iface.common_write_typedefs (m_header, indent);
	m_header << endl;
}

void 
IDLPassXlate::doInterface (IDL_tree  node,
			   IDLScope &scope)
{
	IDLInterface &iface = (IDLInterface &) *scope.getItem(node);

	doForwardDcl(iface);

	string base_name = IDL_CORBA_NS "::Object";

	// generate type container
	m_header << indent << "class " << iface.get_cpp_identifier () << " : ";

	for (IDLInterface::BaseList::const_iterator i = iface.m_bases.begin ();
	     i != iface.m_bases.end (); i++)
	{
		m_header << "public " << (*i)->get_cpp_typename () << ", ";
	}

	m_header << "public virtual " << base_name;

	
	m_header << endl << indent <<" {" << endl;
	
	m_header << indent << "public:" << endl;
	indent++;

	iface_create_traits (iface);
	
	Super::doInterface(node, iface);
	doInterfaceStaticMethodDeclarations (iface);

	


	

	// begin orbitcpp internal section
	m_header << indent << "// These should really be private, but we make them protected" << endl;
	m_header << indent << "//  to stop the compiler from generating warnings" << endl;
	m_header << --indent << "protected:" << endl;
	indent++;

	// constructors
	m_header << indent << iface.get_cpp_identifier () << " () { }" << endl;
	m_header << indent << iface.get_cpp_identifier ()
		 << " (" << iface.get_cpp_identifier () << " const &src);" << endl;
	m_header << indent << iface.get_cpp_identifier ()
		 << " (" << iface.get_c_typename () << " cobject, bool take_copy = false); //orbitcpp-specific" << endl << endl;
		
	// begin public section
	m_header << --indent << "public:" << endl;
	indent++;

	// _orbitcpp wrap method:
	m_header << indent << "static " << iface.get_cpp_identifier ()
		 << "* _orbitcpp_wrap (" << iface.get_c_typename () << " cobject, bool take_copy = false);" << endl << endl;

	// end orbitcpp internal section


	// Methods
	for (IDLInterface::BaseList::const_iterator i = iface.m_all_mi_bases.begin ();
	     i != iface.m_all_mi_bases.end (); i++)
	{
		doInterfaceDownCall (iface, *(*i));
	}
	doInterfaceDownCall (iface, iface);

	m_header << --indent << "};" << endl;

	m_header << indent << "const CORBA::TypeCode_ptr _tc_"
		 << iface.get_cpp_identifier () << " = " 
		 << "(CORBA::TypeCode_ptr)TC_" + iface.get_c_typename () + ";" << endl << endl;

	ORBITCPP_MEMCHECK (new IDLWriteIfaceAnyFuncs (iface, m_state, *this));

	// _duplicate() and _narrow implementations:
	// write the static method definitions
	doInterfaceStaticMethodDefinitions(iface);


	//Implementation:
	
	//Constructor implemention:
	m_module << iface.get_cpp_method_prefix () << "::"
		 << iface.get_cpp_identifier ()
		 << " (" << iface.get_c_typename () << " cobject, bool take_copy /*= false */):" << endl;
	m_module << mod_indent << base_name << "(cobject, take_copy)" << endl
		 << mod_indent << "{}" << endl << endl;

	// _orbitcpp wrap method:
	m_module << iface.get_cpp_typename () << " * "
		 << iface.get_cpp_method_prefix () << "::"
		 << "_orbitcpp_wrap"
		 << " (" << iface.get_c_typename () << " cobject, bool take_copy /* = false */)" << endl
		 << "{" << endl;
	m_module << ++mod_indent << "return new " << iface.get_cpp_typename ()
		 << " (cobject, take_copy);" << endl;
	m_module << --mod_indent << "}" << endl << endl;
}

void
IDLPassXlate::iface_create_traits (const IDLInterface &iface)
{
	// Typedefs
	m_header << indent << "typedef " << iface.get_cpp_member_typename ()
		 << " value_t;" << endl;
	m_header << indent << "typedef " << iface.get_c_typename ()
		 << " c_value_t;" << endl;


	// C++ -> C packer
	m_header << indent << "static void pack_elem "
		 << "(const value_t &cpp_elem, c_value_t &c_elem);" << endl;

	m_module << "void " << iface.get_cpp_method_prefix ()
		 << "::pack_elem "
		 << "(const value_t &cpp_elem, c_value_t &c_elem)" << endl
		 << "{" << endl;
	iface.member_pack_to_c (m_module, ++mod_indent, "cpp_elem", "c_elem");
	m_module << --mod_indent << "}" << endl << endl;

	// C -> C++ unpacker
	m_header << indent << "static void unpack_elem "
		 << "(value_t &cpp_elem, const c_value_t &c_elem);" << endl;

	m_module << "void " << iface.get_cpp_method_prefix ()
		 << "::unpack_elem "
		 << "(value_t &cpp_elem, const c_value_t &c_elem)" << endl
		 << "{" << endl;
	iface.member_unpack_from_c (m_module, ++mod_indent, "cpp_elem", "c_elem");
	m_module << --mod_indent << "}" << endl << endl;
}


void
IDLPassXlate::doInterfaceStaticMethodDeclarations (IDLInterface &iface)
{
	string ptr_name = iface.get_cpp_typename_ptr ();

	m_header << --indent << "private:" << endl;
	++indent;
	m_header << indent << "static " << ptr_name
		 << " _dup (CORBA::Object_ptr obj);" << endl;
	m_header << --indent << "public:" << endl;
	++indent;

	m_header << indent << "static " << ptr_name
		 << " _duplicate (" << ptr_name << " obj);" << endl;

	m_header << indent << "static " << ptr_name
		 << " _narrow (CORBA::Object_ptr obj);" << endl;

	m_header << indent << "static " << ptr_name << " _nil()" << endl
		 << indent << '{' << endl;
	m_header << ++indent << "return CORBA_OBJECT_NIL;" << endl;
	m_header << --indent << '}' << endl;
	m_header << indent << "typedef " << ptr_name << " _ptr_type;" << endl;
	string var_name = iface.get_cpp_typename_var ();
	m_header << indent << "typedef " << var_name << " _var_type;" << endl;
}

void IDLPassXlate::doInterfaceStaticMethodDefinitions (IDLInterface &iface)
{
	string if_name = iface.get_cpp_typename ();
	string ptr_name = iface.get_cpp_typename_ptr ();
	//
	// _dup()
	m_module << ptr_name << " "
		 << iface.get_cpp_method_prefix () << "::_dup(CORBA::Object_ptr ptr)" << endl
		 << "{" << endl;
	
	m_module << ++mod_indent << "if (ptr == CORBA_OBJECT_NIL) return CORBA_OBJECT_NIL;" << endl;

 	m_module << mod_indent << iface.get_c_typename ()
		 << " cobj = ptr->_orbitcpp_cobj ();" << endl;
	m_module << mod_indent << "cobj = ::_orbitcpp::duplicate_guarded (cobj);" << endl;
	m_module << mod_indent << "return "
		 << iface.get_cpp_stub_typename () << "::_orbitcpp_wrap (cobj);" << endl;
	
	m_module << --mod_indent << '}' << endl << endl;

	// _duplicate()
	m_module << ptr_name << " "
		 << iface.get_cpp_method_prefix () << "::_duplicate(" << ptr_name << " obj)" << endl
		 << "{" << endl;

	m_module << ++mod_indent << "return _dup(obj);"  << endl;
	
	m_module << --mod_indent << '}' << endl << endl;

	// _narrow()
	m_module << ptr_name << " "
		 << iface.get_cpp_method_prefix () << "::_narrow (CORBA::Object_ptr obj)" << endl
		 << '{' << endl;

	m_module << ++mod_indent << "if (obj == CORBA_OBJECT_NIL || !obj->_is_a(" << iface.get_cpp_typecode_name() << "->id()))" << endl;
	m_module << ++mod_indent << "return CORBA_OBJECT_NIL;" << endl;
	m_module << --mod_indent << "return _dup(obj);"  << endl;

	m_module << --mod_indent << '}' << endl << endl;
}

void
IDLPassXlate::create_method_proto (const IDLMethod &method)
{
	m_header << indent << method.stub_decl_proto () << ";" << endl;
}

void
IDLPassXlate::create_method_stub (IDLInterface    &iface,
				  const IDLMethod &method)
{
	// print signature
	m_module << method.stub_decl_impl (iface) << endl
		 << "{" << endl;

	// Prepare parameters and return value container
	method.stub_do_pre (m_module, ++mod_indent);
	m_module << endl;
	
	// Do the call
	method.stub_do_call (m_module, mod_indent);
	m_module << endl;
	
	// De-init parameters and return value container,
	// FIXME: handle return value
	method.stub_do_post (m_module, mod_indent);
	
	// End of stub implementation
	m_module << --mod_indent << "}" << endl << endl;
}

void 
IDLPassXlate::doAttributePrototype (IDLInterface &iface,
				    IDLInterfaceRight &of,
				    IDL_tree      node)
{
	IDL_tree simple_dcl = IDL_ATTR_DCL(node).simple_declarations;
	for (; simple_dcl != 0; simple_dcl = IDL_LIST(simple_dcl).next) {
		IDL_tree sd = IDL_LIST(simple_dcl).data;

		IDLAttribute attr (node, sd);

		// Getter
		create_method_proto (IDLAttribGetter (of, attr));

		// Setter
		if (!attr.isReadOnly ())
			create_method_proto (IDLAttribSetter (of, attr));
		
		m_header << endl;
	}
}

void 
IDLPassXlate::doAttributeStub (IDLInterface &iface,
			       IDLInterfaceRight &of,
			       IDL_tree      node)
{
	IDL_tree simple_dcl = IDL_ATTR_DCL(node).simple_declarations;
	for (; simple_dcl != 0; simple_dcl = IDL_LIST(simple_dcl).next) {
		IDL_tree sd = IDL_LIST(simple_dcl).data;

		IDLAttribute attr (node, sd);

		// Getter
		create_method_stub (iface, IDLAttribGetter (of, attr));

		// Setter
		if (!attr.isReadOnly ())
			create_method_stub (iface, IDLAttribSetter (of, attr));
	}
}




void 
IDLPassXlate::doOperationPrototype (IDLInterface &iface,
				    IDLInterfaceRight &of,
				    IDL_tree      node)
{
	IDLOperation op (node, of);

	create_method_proto (op);

	if (IDL_OP_DCL(node).context_expr != NULL) ORBITCPP_NYI("contexts");
}

void 
IDLPassXlate::doOperationStub (IDLInterface &iface,
			       IDLInterfaceRight &of,
			       IDL_tree      node)
{
	IDLOperation op (node, of);

	create_method_stub (iface, op);

	if (IDL_OP_DCL(node).context_expr != NULL) ORBITCPP_NYI("contexts");
}




void 
IDLPassXlate::doInterfaceDownCall(IDLInterface &iface,IDLInterfaceRight &of) {
	IDL_tree body_list = IDL_INTERFACE(of.getNode()).body;
	while (body_list) {
		switch (IDL_NODE_TYPE(IDL_LIST(body_list).data)) {
		case IDLN_ATTR_DCL:
			doAttributePrototype(iface,of,IDL_LIST(body_list).data);
			doAttributeStub(iface,of,IDL_LIST(body_list).data);
			break;
		case IDLN_OP_DCL:
			doOperationPrototype(iface,of,IDL_LIST(body_list).data);
			doOperationStub(iface,of,IDL_LIST(body_list).data);
			break;
		default:
			break;
		}
		body_list = IDL_LIST(body_list).next;
	}
}

void 
IDLPassXlate::doModule (IDL_tree  node,
			IDLScope &scope)
{
	IDLScope *module = (IDLScope *) scope.getItem(node);

	string id = module->get_cpp_identifier ();
	m_header << indent << "namespace " << id << endl
		 << indent << '{' << endl;
	++indent;

	Super::doModule(node, *module);

	m_header << --indent << "} //namespace " << id << endl << endl;
}

void IDLPassXlate::enumHook(IDL_tree next,IDLScope &scope) {
	if (!scope.interface_child())
		runJobs(IDL_EV_TOPLEVEL);
}

void IDLPassXlate::element_write_typecode (const IDLElement &element)
{
	if (element.interface_child())
	{
		m_header << indent << "static const ::CORBA::TypeCode_ptr "
			 << "_tc_" << element.get_cpp_identifier ()
			 << ';' << endl << endl;
		IDLInterfaceRight parent (element.parent_interface());
		m_module << mod_indent << "const ::CORBA::TypeCode_ptr "
			 << parent.get_cpp_method_prefix()
			 << "::_tc_" << element.get_cpp_identifier () << " = "
			 << "(::CORBA::TypeCode_ptr)TC_" << element.get_c_typename ()
			 << ';' << endl << endl;
	}
	else
	{
		m_header << indent << "const ::CORBA::TypeCode_ptr "
			 << "_tc_" << element.get_cpp_identifier () << " = "
			 << "(::CORBA::TypeCode_ptr)TC_" << element.get_c_typename ()
			 << ';' << endl << endl;
	}

}


// IDLWriteAnyFuncs -------------------------------------------------------
IDLWriteAnyFuncs::IDLWriteAnyFuncs (IDLCompilerState &state,
				    IDLOutputPass    &pass):
	IDLOutputJob ("", state, pass)
{
}


void IDLWriteAnyFuncs::writeAnyFuncs (bool          pass_value,
				      const string &cpptype, 
				      const string &ctype)
{
	if (pass_value)
	{
		writeInsertFunc (m_header, indent, FUNC_VALUE, cpptype, ctype);
		writeExtractFunc (m_header, indent, FUNC_VALUE, cpptype, ctype);
	} else {
		writeInsertFunc (m_header, indent, FUNC_COPY, cpptype, ctype);
		writeInsertFunc (m_header, indent, FUNC_NOCOPY, cpptype, ctype);
		writeExtractFunc (m_header, indent, FUNC_NOCOPY, cpptype, ctype);
	}
}

void IDLWriteAnyFuncs::writeInsertFunc (ostream      &ostr,
					Indent       &indent,
					FuncType      func, 
					string        ident,
					const string &ctype)
{
	string any_func, any_arg;
	any_func = "insert_simple";
	any_arg = "&val";
	if (func == FUNC_COPY) {
		ident += " const &";
	}
	else if (func == FUNC_NOCOPY) {
		ident += "*";
		any_arg = "val, CORBA_FALSE";
	}

	ostr << indent << "inline void operator <<= "
	     <<	"(CORBA::Any& the_any, " << ident << " val)" << endl
	     << indent << "{" << endl;
	
	ostr << ++indent << "the_any." << any_func 
	     <<" ((CORBA::TypeCode_ptr)TC_" << ctype << ", "
	     << any_arg << ");" << endl;
	
	ostr << --indent << endl << "}" << endl << endl;
}

void IDLWriteAnyFuncs::writeExtractFunc (ostream      &ostr,
					 Indent       &indent,
					 FuncType      func, 
					 string        ident,
					 const string &ctype)
{
	string any_func, any_arg;
	any_arg = "val";
	if (func == FUNC_VALUE) {
		ident += "&";
		any_func = "extract";
	} else {
		ident += " const *&";
		any_func = "extract_ptr";
	}
	
	ostr << indent << "inline CORBA::Boolean operator >>= "
	     << "(const CORBA::Any& the_any, " << ident << " val)" << endl
	     << indent << "{" << endl;
	
	ostr << ++indent << "return the_any." << any_func
	     << " ((CORBA::TypeCode_ptr)TC_" << ctype << ", "
	     << any_arg << ");" << endl;

	ostr << --indent << endl << "}" << endl << endl;
}

// IDLWriteIFaceAnyFuncs -------------------------------------------------------
IDLWriteIfaceAnyFuncs::IDLWriteIfaceAnyFuncs (const IDLInterface &_iface,
					      IDLCompilerState   &state,
					      IDLOutputPass      &pass) :
	IDLWriteAnyFuncs(state, pass),
	m_iface(_iface)
{
}

void
IDLWriteIfaceAnyFuncs::run()
{
	string cpptype = m_iface.get_cpp_typename ()+ "_ptr";
	string ctype = m_iface.get_c_typename ();
	writeInsertFunc(m_header, indent, FUNC_NOCOPY, cpptype, ctype);
	writeAnyFuncs(true, cpptype, ctype );
}

// IDLWriteEnumAnyFuncs -------------------------------------------------------

IDLWriteEnumAnyFuncs::IDLWriteEnumAnyFuncs (const IDLEnum    &_enum,
					    IDLCompilerState &state,
					    IDLOutputPass    &pass) :
	IDLWriteAnyFuncs (state, pass),
	m_enum(_enum)
{
}

void IDLWriteEnumAnyFuncs::run()
{
	writeAnyFuncs(true,
		      m_enum.get_cpp_typename (),
		      m_enum.get_c_typename ());
}


void
IDLWriteExceptionAnyFuncs::run ()
{
	string tc = "(CORBA::TypeCode_ptr)TC_" + m_element.get_c_typename ();
	string cpp_id = m_element.get_cpp_typename ();
	string c_id = m_element.get_c_typename ();
	
	// Operator <<=
	m_header << indent << "inline void operator <<= "
		 << "(CORBA::Any& the_any, " << cpp_id << " const &val)" << endl;
	m_header << indent++ << "{" << endl;

	m_header << indent << "the_any.insert_simple (" << tc << ", "
		 << "val._orbitcpp_pack(), CORBA_FALSE);" << endl;

	m_header << --indent << endl << "}" << endl << endl;

	// Operator >>=
	m_header << indent << "inline CORBA::Boolean operator >>= "
		 << "(const CORBA::Any& the_any, "<< cpp_id << " &val)" << endl;
	m_header << indent << "{" << endl;
	
	m_header << ++indent << "const " << c_id << " *ex;" << endl;
	m_header << indent << "if (the_any.extract_ptr (" << tc << ", ex))" << endl
		 << indent << "{" << endl;

	m_header << ++indent << "val._orbitcpp_unpack (*ex);"
		 << indent << "return true;" << endl;

	m_header << --indent << "} else {" << endl;
	indent++;
	
	m_header << indent << "return false;" << endl;
	m_header << --indent << "}" << endl;

	m_header << --indent << "}" << endl << endl;
}

// IDLWriteArrayAnyFuncs -------------------------------------------------------
IDLWriteArrayAnyFuncs::IDLWriteArrayAnyFuncs (const IDLArray   &_array,
					      const IDLElement &_dest, 
					      IDLCompilerState &state,
					      IDLOutputPass    &pass) :
	IDLWriteAnyFuncs(state, pass),
	m_array(_array),
	m_dest(_dest)
{
}


void
IDLWriteArrayAnyFuncs::run()
{
	string array_id = m_dest.get_cpp_typename ();
	string slice_id = array_id + "_slice";
	string forany_id = array_id + "_forany";
	string tc =  m_dest.getParentScope ()->get_cpp_typename () +
		"::_tc_" + m_dest.get_cpp_identifier ();

	
	// Operator <<=
	m_header << indent << "inline void operator <<= "
		 << "(CORBA::Any& the_any, " << forany_id << " &_arr)" << endl
		 << indent << "{" << endl;

	m_header << ++indent
		 << "the_any.insert_simple (" << tc << ", "
		 << "(" << slice_id << "*)_arr, !_arr._nocopy());" << endl;

	m_header << --indent << endl << "}" << endl;

	
	// Operator >>=
	m_header << indent << "inline CORBA::Boolean operator >>= "
		 << "(CORBA::Any& the_any, " << forany_id << " &_arr)" << endl
		 << indent << "{" << endl;

	m_header << ++indent << "const " << slice_id << " *tmp;" << endl
		 << indent << "CORBA::Boolean _retval;" << endl << endl;

	m_header << indent << "_retval = the_any.extract_ptr "
		 << "(" << tc << ", tmp);" << endl;
	m_header << indent << "_arr = (" << slice_id << "*)tmp;" << endl;
	m_header << indent << "return _retval;" << endl;

	m_header << --indent << "}" << endl << endl;
}
