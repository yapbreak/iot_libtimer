ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

include $(top_srcdir)/Makefile.common

lib_LTLIBRARIES = libtimer.la
libtimer_la_SOURCES  = timer.h
libtimer_la_SOURCES += timer.cpp
libtimer_la_SOURCES += event.cpp
libtimer_la_SOURCES += event.h

include_HEADERS = timer.h event.h


bin_PROGRAMS = timertest

timertest_SOURCES  = test.cpp
timertest_SOURCES += simulate.cpp
timertest_LDADD = libtimer.la
