ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

include $(top_srcdir)/Makefile.common

bin_PROGRAMS = timertest

timertest_SOURCES  = ../src/test.cpp
timertest_SOURCES += simulate.cpp
timertest_LDADD = $(top_builddir)/src/libtimer.la
