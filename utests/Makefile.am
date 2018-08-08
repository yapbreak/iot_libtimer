ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

include $(top_srcdir)/utests/Makefile.test.common

SUBDIRS = fixtures

TESTS = ut_timer.xtest

noinst_PROGRAMS = $(TESTS)

check_PROGRAMS = $(TESTS)

lib_LTLIBRARIES = libtimer_utests.la
libtimer_utests_la_CFLAGS = $(TESTCFLAGS)
libtimer_utests_la_CXXFLAGS = $(TESTCXXFLAGS)
libtimer_utests_la_SOURCES  = ../src/timer.h
libtimer_utests_la_SOURCES += ../src/timer.cpp
libtimer_utests_la_SOURCES += ../src/event.h
libtimer_utests_la_SOURCES += ../src/event.cpp

ut_timer_xtest_SOURCES  = ut_event.h
ut_timer_xtest_SOURCES += ut_timer.h
ut_timer_xtest_SOURCES += ut_timer_c.h
ut_timer_xtest_SOURCES += main.cpp
ut_timer_xtest_CFLAGS = $(TESTCFLAGS)
ut_timer_xtest_CXXFLAGS = $(TESTCXXFLAGS)
ut_timer_xtest_LDFLAGS = $(TESTLDFLAGS)
ut_timer_xtest_LDADD  = $(top_builddir)/utests/libtimer_utests.la
ut_timer_xtest_LDADD += $(top_builddir)/utests/fixtures/libfixtures.la
