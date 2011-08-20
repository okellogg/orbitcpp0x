#ifndef TEST_IMPL_HH
#define TEST_IMPL_HH

#include "test-cpp-skels.h"

class IMaster_impl: public POA_Test::IMaster
{
	char *str1;
	char *str2;
	char *str3;

public:
	IMaster_impl();

	void longseq_in (const ::Test::long_seq &seq_in)
		throw (CORBA::SystemException);
	void longseq_inout (::Test::long_seq &seq_inout)
		throw (CORBA::SystemException);
	void longseq_out (::Test::long_seq_out seq_out)
		throw (CORBA::SystemException);
	::Test::long_seq* longseq_ret ()
		throw (CORBA::SystemException);

	void strseq_in (const ::Test::string_seq &seq_in)
		throw (CORBA::SystemException);
	void strseq_inout (::Test::string_seq &seq_inout)
		throw (CORBA::SystemException);
	void strseq_out (::Test::string_seq_out seq_out)
		throw (CORBA::SystemException);
	::Test::string_seq* strseq_ret ()
		throw (CORBA::SystemException);

	void dirseq_in (const ::Test::direction_seq &seq_in)
		throw (CORBA::SystemException);
	void dirseq_inout (::Test::direction_seq &seq_inout)
		throw (CORBA::SystemException);
	void dirseq_out (::Test::direction_seq_out seq_out)
		throw (CORBA::SystemException);
	::Test::direction_seq* dirseq_ret ()
		throw (CORBA::SystemException);

	void mat_in (const ::Test::long_mat &seq_in)
		throw (CORBA::SystemException);
	void mat_inout (::Test::long_mat &seq_inout)
		throw (CORBA::SystemException);
	void mat_out (::Test::long_mat_out seq_out)
		throw (CORBA::SystemException);
	::Test::long_mat* mat_ret ()
		throw (CORBA::SystemException);
};

#endif
