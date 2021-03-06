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
 *  Purpose:	idl compiler backend main routine
 *
 */

#include <glib.h>
#include <libIDL/IDL.h>
#include <orbit-idl/orbit-idl-backend.h>
#include <stdexcept>
#include <fstream>
#include <string>
#include "pass_gather.h"
#include "pass_xlate.h"
#include "pass_stubs.h"
#include "pass_skels.h"




extern "C" {

	//ORBit-2 looks for this function in the backend libraries:
	gboolean orbit_idl_backend_func (ORBitIDLBackendContext *context);
	
	gboolean idlCPPBackend (IDL_tree tree, ORBitIDLBackendContext *context);
	
	gboolean orbit_idl_backend_func (ORBitIDLBackendContext *context)
	{
		return idlCPPBackend(context->tree, context);
	}
	
}  //extern "C"


void idlWritePreamble(ostream &ostr) {
	ostr
	<< "// -----------------------------------------------------" << endl
	<< "// generated by orbitcpp, do not edit" << endl
	<< "// -----------------------------------------------------" << endl
	<< endl << endl;
}



gboolean idlCPPBackend (IDL_tree tree, ORBitIDLBackendContext *context)
{
	const char *filename = context->filename;

	try {
		// generate base name
		string basename(g_basename(filename));
		string::size_type dotpos = basename.find('.');

		if (dotpos != string::npos) basename.erase(dotpos,basename.size());

		// state
		IDLCompilerState state(basename, tree);
		IDLPassGather pass0(state);
		pass0.runPass();

		if (context->do_common) {
			std::ofstream h_common((basename + IDL_CPP_HEADER_EXT).c_str());
			std::ofstream cc_common((basename + IDL_CPP_MODULE_EXT).c_str());
			idlWritePreamble(h_common);
			idlWritePreamble(cc_common);
			IDLPassXlate pass(state, h_common, cc_common);
			pass.runPass();
		}

		if (context->do_stubs) {
			std::ofstream h_stubs((basename + IDL_CPP_STUB_HEADER_EXT).c_str());
			std::ofstream cc_stubs((basename + IDL_CPP_STUB_MODULE_EXT).c_str());
			idlWritePreamble(h_stubs);
			idlWritePreamble(cc_stubs);
			IDLPassStubs pass(state, h_stubs, cc_stubs);
			pass.runPass();
		}

		if (context->do_skels) {
			std::ofstream h_skels((basename + IDL_CPP_SKEL_HEADER_EXT).c_str());
			std::ofstream cc_skels((basename + IDL_CPP_SKEL_MODULE_EXT).c_str());
			idlWritePreamble(h_skels);
			idlWritePreamble(cc_skels);
			IDLPassSkels pass(state, h_skels, cc_skels);
			pass.runPass();
		}

		// "Pass" four: write include stuff
		{
			std::ofstream header((basename + IDL_CPP_MAIN_HEADER_EXT).c_str());
			std::ofstream module((basename + IDL_CPP_MAIN_MODULE_EXT).c_str());
			idlWritePreamble(header);
			idlWritePreamble(module);

			if (context->do_common) {
				header << "#include \"" << basename << IDL_CPP_HEADER_EXT << "\"" << endl;
				module << "#include \"" << basename << IDL_CPP_MODULE_EXT << "\"" << endl;
			}

			if (context->do_stubs) {
				header << "#include \"" << basename << IDL_CPP_STUB_HEADER_EXT << "\"" << endl;
				module << "#include \"" << basename << IDL_CPP_STUB_MODULE_EXT << "\"" << endl;
			}

			if (context->do_skels) {
				header << "#include \"" << basename << IDL_CPP_SKEL_HEADER_EXT << "\"" << endl;
				module << "#include \"" << basename << IDL_CPP_SKEL_MODULE_EXT << "\"" << endl;
			}
		}

		return TRUE;

	} catch (std::exception &ex) {
		cerr << ex.what() << endl;
	}
	catch (...) {
		cerr << "C++ code generation failed because of an unknown exception" << endl;
	}

	return FALSE;
}
