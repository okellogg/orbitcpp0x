/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */

#include "test-cpp-stubs.h"
#include <iostream>
#include <fstream>
#include <string>
	

void test_longs   (Test::IMaster_ptr master_ptr);
void test_strings (Test::IMaster_ptr master_ptr);
void test_enums (Test::IMaster_ptr master_ptr);
void test_mat (Test::IMaster_ptr master_ptr);

int main (int argc, char *argv[])
{
	CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
	
	std::ifstream ior_stream ("test.ior");
	std::string ior;
	ior_stream >> ior;
	
	CORBA::Object_var obj = orb->string_to_object (ior);
	Test::IMaster_var master_ptr = Test::IMaster::_narrow (obj);

	test_longs (master_ptr);
	test_strings (master_ptr);
	test_enums (master_ptr);
	test_mat (master_ptr);

	return 0;
}


void test_longs (Test::IMaster_ptr master_ptr)
{

	///////////////////////////////////
	// -> IN
	
	Test::long_seq in_seq;
	in_seq.length (5);
	for (CORBA::ULong i = 0; i < 5; i++)
		in_seq[i] = i * 11;

	master_ptr->longseq_in (in_seq);

	///////////////////////////////////
	// <-> INOUT
	
	Test::long_seq inout_seq;
	inout_seq.length (5);
	for (CORBA::ULong i = 0; i < 5; i++)
		inout_seq[i] = (i + 1) * 22;
	
	master_ptr->longseq_inout (inout_seq);
	
	for (CORBA::ULong i = 0; i < inout_seq.length (); i++)
		std::cout << "Client::seq_inout[" << i << "] == "
				  << inout_seq[i] << std::endl;
	std::cout << std::endl;
	
	
	///////////////////////////////////
	// <- OUT
	Test::long_seq_var out_seq;
	master_ptr->longseq_out (out_seq);

	for (CORBA::ULong i = 0; i < out_seq->length (); i++)
		std::cout << "Client::seq_out[" << i << "] == "
				  << out_seq[i] << std::endl;
	std::cout << std::endl;
	

	///////////////////////////////////
	// <- RET
	Test::long_seq_var ret_seq = master_ptr->longseq_ret ();

	for (CORBA::ULong i = 0; i < ret_seq->length (); i++)
		std::cout << "Client::seq_ret[" << i << "] == "
				  << ret_seq[i] << std::endl;
	std::cout << std::endl;
}


void test_strings (Test::IMaster_ptr master_ptr)
{
	///////////////////////////////////
	// -> IN
	
	Test::string_seq in_seq;
	in_seq.length (5);
	in_seq[0] = "String #1 in client sequence";
	in_seq[1] = "String #2 in client sequence";
	in_seq[2] = "String #3 in client sequence";
	in_seq[3] = "String #4 in client sequence";
	in_seq[4] = "String #5 in client sequence";

	master_ptr->strseq_in (in_seq);

	///////////////////////////////////
	// <-> INOUT
	
	Test::string_seq inout_seq;
	inout_seq.length (5);
	inout_seq[0] = "String #1 in client sequence";
	inout_seq[1] = "String #2 in client sequence";
	inout_seq[2] = "String #3 in client sequence";
	inout_seq[3] = "String #4 in client sequence";
	inout_seq[4] = "String #5 in client sequence";
	
	master_ptr->strseq_inout (inout_seq);
	
	for (CORBA::ULong i = 0; i < inout_seq.length (); i++)
		std::cout << "Client::seq_inout[" << i << "] == "
				  << inout_seq[i] << std::endl;
	std::cout << std::endl;
	

	///////////////////////////////////
	// <- OUT
	Test::string_seq_var out_seq;
	master_ptr->strseq_out (out_seq);

	for (CORBA::ULong i = 0; i < out_seq->length (); i++)
		std::cout << "Client::seq_out[" << i << "] == "
				  << out_seq[i] << std::endl;
	std::cout << std::endl;
	

	///////////////////////////////////
	// <- RET
	Test::string_seq_var ret_seq = master_ptr->strseq_ret ();
	
	for (CORBA::ULong i = 0; i < ret_seq->length (); i++)
		std::cout << "Client::seq_ret[" << i << "] == "
				  << ret_seq[i] << std::endl;
	std::cout << std::endl;
}

void test_enums (Test::IMaster_ptr master_ptr)
{

	///////////////////////////////////
	// -> IN
	
	Test::direction_seq in_seq;
	in_seq.length (5);
	for (CORBA::ULong i = 0; i < 5; i++)
		in_seq[i] = (Test::Direction) (i % Test::nb_directions);

	master_ptr->dirseq_in (in_seq);

	///////////////////////////////////
	// <-> INOUT
	
	Test::direction_seq inout_seq;
	inout_seq.length (5);
	for (CORBA::ULong i = 0; i < 5; i++)
		inout_seq[i] = (Test::Direction) (i % Test::nb_directions);
	
	master_ptr->dirseq_inout (inout_seq);
	
	for (CORBA::ULong i = 0; i < inout_seq.length (); i++)
		std::cout << "Client::seq_inout[" << i << "] == "
				  << inout_seq[i] << std::endl;
	std::cout << std::endl;
	
	
	///////////////////////////////////
	// <- OUT
	Test::direction_seq_var out_seq;
	master_ptr->dirseq_out (out_seq);

	for (CORBA::ULong i = 0; i < out_seq->length (); i++)
		std::cout << "Client::seq_out[" << i << "] == "
				  << out_seq[i] << std::endl;
	std::cout << std::endl;
	

	///////////////////////////////////
	// <- RET
	Test::direction_seq_var ret_seq = master_ptr->dirseq_ret ();

	for (CORBA::ULong i = 0; i < ret_seq->length (); i++)
		std::cout << "Client::seq_ret[" << i << "] == "
				  << ret_seq[i] << std::endl;
	std::cout << std::endl;
}

void test_mat (Test::IMaster_ptr master_ptr)
{

	///////////////////////////////////
	// -> IN
	
	Test::long_mat in_seq;
	in_seq.length (5);
	for (CORBA::ULong i = 0; i < 5; i++) {
		in_seq[i].length(2);
		for (CORBA::ULong j = 0; j < 2; j++) {
			in_seq[i][j] = 2 * i + j;
		}
	}

	master_ptr->mat_in (in_seq);

	///////////////////////////////////
	// <-> INOUT
	
	Test::long_mat inout_seq;
	inout_seq.length (5);
	for (CORBA::ULong i = 0; i < 5; i++) {
		inout_seq[i].length(3);
		for (CORBA::ULong j = 0; j < 3; j++) {
			inout_seq[i][j] = 3 * i + j;
		}
	}
	
	master_ptr->mat_inout (inout_seq);
	
	for (CORBA::ULong i = 0; i < inout_seq.length (); i++)
		for (CORBA::ULong j = 0; j < inout_seq[i].length (); j++)
			std::cout << "Client::seq_inout[" << i << ',' << j << "] == "
					  << inout_seq[i][j] << std::endl;
	std::cout << std::endl;
	
	
	///////////////////////////////////
	// <- OUT
	Test::long_mat* out_seq;
	// this causes a seg. fault
	//Test::long_mat_var out_seq;
	master_ptr->mat_out (out_seq);

	for (CORBA::ULong i = 0; i < (*out_seq).length (); i++)
		for (CORBA::ULong j = 0; j < (*out_seq)[i].length (); j++)
			std::cout << "Client::seq_out[" << i << ',' << j << "] == "
					  << (*out_seq)[i][j] << std::endl;
	std::cout << std::endl;
	

	///////////////////////////////////
	// <- RET
	Test::long_mat* ret_seq = master_ptr->mat_ret ();
	// this causes a seg. fault
	//Test::long_mat_var ret_seq = master_ptr->mat_ret ();

	for (CORBA::ULong i = 0; i < (*ret_seq).length (); i++)
		for (CORBA::ULong j = 0; j < (*ret_seq)[i].length (); j++)
			std::cout << "Client::seq_ret[" << i << ',' << j << "] == "
					  << (*ret_seq)[i][j] << std::endl;
	std::cout << std::endl;
}
