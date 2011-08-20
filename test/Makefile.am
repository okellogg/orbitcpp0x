SUBDIRS = cpp

noinst_INCLUDES = tut/tut.hpp tut/tut_reporter.hpp tut/tut_restartable.hpp \
			client_server.h

if OS_WIN32
else
check_PROGRAMS = test_runner
endif

test_runner_SOURCES = main.cc client_server.cc test_factory.cc test_policy.cc
