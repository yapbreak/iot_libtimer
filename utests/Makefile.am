ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

include $(top_srcdir)/utests/Makefile.test.common

SUBDIRS = fixtures

TESTS = ut_timer.xtest

noinst_PROGRAMS = $(TESTS)

check_PROGRAMS = $(TESTS)

ut_timer_xtest_SOURCES  = ut_event.h
ut_timer_xtest_SOURCES += ut_timer.h
ut_timer_xtest_SOURCES += ut_timer_c.h
ut_timer_xtest_SOURCES += main.cpp
ut_timer_xtest_CFLAGS = $(TESTCFLAGS)
ut_timer_xtest_CXXFLAGS = $(TESTCXXFLAGS)
ut_timer_xtest_LDFLAGS = $(TESTLDFLAGS)
ut_timer_xtest_LDADD  = $(top_builddir)/src/libtimer.la
ut_timer_xtest_LDADD += $(top_builddir)/utests/fixtures/libfixtures.la
