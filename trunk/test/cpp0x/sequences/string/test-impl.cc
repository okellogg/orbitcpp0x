#include "test-impl.h"
#include <iostream>

void IMaster_impl::seq_in (const ::Test::str_seq &seq_in)
    throw (CORBA::SystemException)
{
    std::cerr << "IMaster::seq_in" << std::endl;
    for (CORBA::ULong i = 0; i < seq_in.length (); i++)
	std::cerr << "\tseq[" << i << "] == " << seq_in[i] << std::endl;
}

void IMaster_impl::seq_inout (::Test::str_seq &seq_inout)
    throw (CORBA::SystemException)
{
    std::cerr << "IMaster::seq_inout" << std::endl;
    for (CORBA::ULong i = 0; i < seq_inout.length (); i++)
	std::cerr << "\tseq[" << i << "] == " << seq_inout[i] << std::endl;

    seq_inout.length (10);
    for (CORBA::ULong i = 0; i < 10; i++)
    {
	char *tmp = g_strdup_printf ("INOUT modified string #%d", i);
	seq_inout[i] = CORBA::string_dup (tmp);
	g_free (tmp);
    }
}

void IMaster_impl::seq_out (::Test::str_seq_out seq_out)
    throw (CORBA::SystemException)
{
    seq_out = new Test::str_seq;
    
    seq_out->length (10);
    for (CORBA::ULong i = 0; i < 10; i++)
    {
	char *tmp = g_strdup_printf ("OUT string #%d", i);
	seq_out[i] = CORBA::string_dup (tmp);
	g_free (tmp);
    }
}

::Test::str_seq* IMaster_impl::seq_ret ()
	    throw (CORBA::SystemException)
{
    Test::str_seq *seq_ret = new Test::str_seq;
    
    seq_ret->length (5);
    for (CORBA::ULong i = 0; i < 5; i++)
    {
	char *tmp = g_strdup_printf ("RET string #%d", i);
	(*seq_ret)[i] = CORBA::string_dup (tmp);
	g_free (tmp);
    }

    return seq_ret;
}
