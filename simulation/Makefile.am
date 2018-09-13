ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

include $(top_srcdir)/Makefile.common

bin_PROGRAMS = timertest

timertest_CFLAGS = -DSIMULATE $(AM_CFLAGS)
timertest_CXXFLAGS = -DSIMULATE $(AM_CXXFLAGS)
timertest_SOURCES  = test.cpp
timertest_SOURCES += simulate.cpp
timertest_LDADD = $(top_builddir)/src/libtimer.la $(ARDSIM_LIBS)
