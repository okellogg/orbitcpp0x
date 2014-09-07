#include "test-impl.h"
#include <iostream>

IMaster_impl::IMaster_impl()
	: str1("String #1 in server sequence"),
	str2("String #2 in server sequence"),
	str3("String #3 in server sequence")
{

}


void IMaster_impl::longseq_in(const ::Test::long_seq &in_seq)
	throw (CORBA::SystemException)
{
	for (CORBA::ULong i = 0; i < in_seq.length(); i++) {
		std::cout << "Server::in_seq[" << i << "] == "
			<< in_seq[i] << std::endl;
	}
}


void IMaster_impl::longseq_inout(::Test::long_seq &inout_seq)
	throw (CORBA::SystemException)
{
	for (CORBA::ULong i = 0; i < inout_seq.length(); i++) {
		std::cout << "Server::inout_seq[" << i << "] == "
			<< inout_seq[i] << std::endl;
	}

	inout_seq.length(4);
	for (CORBA::ULong i = 0; i < 4; i++) {
		inout_seq[i] = i * 13;
	}
}


void IMaster_impl::longseq_out (::Test::long_seq_out out_seq)
	throw (CORBA::SystemException)
{
	CORBA::Long *buf = ::Test::long_seq::allocbuf(6);
	for (CORBA::ULong i = 0; i < 6; i++) {
		buf[i] = (i * 13) - 12;
	}

	out_seq = new ::Test::long_seq(6, 6, buf, TRUE);
}


::Test::long_seq* IMaster_impl::longseq_ret ()
	throw (CORBA::SystemException)
{
	CORBA::Long *buf = ::Test::long_seq::allocbuf(3);

	for (CORBA::ULong i = 0; i < 3; i++) {
		buf[i] = (i * 2) - 12;
	}

	return new ::Test::long_seq(3, 3, buf, TRUE);
}


void IMaster_impl::strseq_in(const ::Test::string_seq &in_seq)
	throw (CORBA::SystemException)
{
	for (CORBA::ULong i = 0; i < in_seq.length(); i++) {
		std::cout << "server::seq_in[" << i << "] == "
			<< in_seq[i] << std::endl;
	}
}


void IMaster_impl::strseq_inout(::Test::string_seq &inout_seq)
	throw (CORBA::SystemException)
{
	int i = 0;

	for (i = 0; i < inout_seq.length(); i++) {
		std::cout << "server::seq_inout[" << i << "] == "
			<< inout_seq[i] << std::endl;
	}

	inout_seq.length(3);
	inout_seq[0] = CORBA::string_dup(str1);
	inout_seq[1] = CORBA::string_dup(str2);
	inout_seq[2] = CORBA::string_dup(str3);
}


void IMaster_impl::strseq_out(::Test::string_seq_out seq_out)
	throw (CORBA::SystemException)
{
	CORBA::String_var *buf = ::Test::string_seq::allocbuf(3);

	buf[0] = CORBA::string_dup(str1);
	buf[1] = CORBA::string_dup(str2);
	buf[2] = CORBA::string_dup(str3);

	seq_out = new ::Test::string_seq(3, 3, buf, TRUE);
}


::Test::string_seq* IMaster_impl::strseq_ret()
	throw (CORBA::SystemException)
{
	CORBA::String_var *buf = ::Test::string_seq::allocbuf(3);

	buf[0] = CORBA::string_dup(str1);
	buf[1] = CORBA::string_dup(str2);
	buf[2] = CORBA::string_dup(str3);

	return new ::Test::string_seq(3, 3, buf, TRUE);
}

void IMaster_impl::dirseq_in(const ::Test::direction_seq &in_seq)
	throw (CORBA::SystemException)
{
	for (CORBA::ULong i = 0; i < in_seq.length(); i++) {
		std::cout << "server::seq_in[" << i << "] == "
			<< in_seq[i] << std::endl;
	}
}


void IMaster_impl::dirseq_inout(::Test::direction_seq &inout_seq)
	throw (CORBA::SystemException)
{
	int i = 0;

	for (i = 0; i < inout_seq.length(); i++) {
		std::cout << "server::seq_inout[" << i << "] == "
			<< inout_seq[i] << std::endl;
	}

	inout_seq.length(4);
	inout_seq[0] = ::Test::DIR_NORTH;
	inout_seq[1] = ::Test::DIR_EAST;
	inout_seq[2] = ::Test::DIR_SOUTH;
	inout_seq[3] = ::Test::DIR_WEST;
}


void IMaster_impl::dirseq_out(::Test::direction_seq_out seq_out)
	throw (CORBA::SystemException)
{
	::Test::direction_seq* seq = new ::Test::direction_seq();
	seq->length(4);
	(*seq)[0] = ::Test::DIR_NORTH;
	(*seq)[1] = ::Test::DIR_EAST;
	(*seq)[2] = ::Test::DIR_SOUTH;
	(*seq)[3] = ::Test::DIR_WEST;
	seq_out = seq;
}


::Test::direction_seq* IMaster_impl::dirseq_ret()
	throw (CORBA::SystemException)
{
	::Test::direction_seq* seq = new ::Test::direction_seq();
	seq->length(4);
	(*seq)[0] = ::Test::DIR_NORTH;
	(*seq)[1] = ::Test::DIR_EAST;
	(*seq)[2] = ::Test::DIR_SOUTH;
	(*seq)[3] = ::Test::DIR_WEST;
	return seq;
}

void IMaster_impl::mat_in(const ::Test::long_mat &in_seq)
	throw (CORBA::SystemException)
{
	for (CORBA::ULong i = 0; i < in_seq.length(); i++) {
		for (CORBA::ULong j = 0; j < in_seq[i].length(); j++) {
		
			std::cout << "Server::in_seq[" << i << ',' << j << "] == "
				<< in_seq[i][j] << std::endl;
		}
	}
}


void IMaster_impl::mat_inout(::Test::long_mat &inout_seq)
	throw (CORBA::SystemException)
{
	for (CORBA::ULong i = 0; i < inout_seq.length(); i++) {
		for (CORBA::ULong j = 0; j < inout_seq[i].length(); j++) {
			std::cout << "Server::inout_seq[" << i << ',' << j << "] == "
				<< inout_seq[i][j] << std::endl;
		}
	}

	inout_seq.length(4);
	for (CORBA::ULong i = 0; i < 4; i++) {
		inout_seq[i].length(3);
		for (CORBA::ULong j = 0; j < 3; j++) {
			inout_seq[i][j] = 3 * i + j;
		}
	}
}


void IMaster_impl::mat_out (::Test::long_mat_out out_seq)
	throw (CORBA::SystemException)
{
	::Test::long_mat* mat = new ::Test::long_mat();
	mat->length(6);
	for (CORBA::ULong i = 0; i < 6; i++) {
		(*mat)[i].length(4);
		for (CORBA::ULong j = 0; j < 4; j++) {
			(*mat)[i][j] = 4 * i + j;
		}
	}

	out_seq = mat;
}


::Test::long_mat* IMaster_impl::mat_ret ()
	throw (CORBA::SystemException)
{
	::Test::long_mat* mat = new ::Test::long_mat();
	mat->length(6);
	for (CORBA::ULong i = 0; i < 6; i++) {
		(*mat)[i].length(4);
		for (CORBA::ULong j = 0; j < 4; j++) {
			(*mat)[i][j] = 4 * i + j;
		}
	}

	return mat;
}
