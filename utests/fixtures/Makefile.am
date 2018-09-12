ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

include $(top_srcdir)/utests/Makefile.test.common

check_LTLIBRARIES = libfixtures.la
libfixtures_la_SOURCES  = ArduinoFixtures.cpp
libfixtures_la_SOURCES += ArduinoFixtures.h
libfixtures_la_CFLAGS = $(TESTCFLAGS)
libfixtures_la_CXXFLAGS = $(TESTCXXFLAGS)
libfixtures_la_LDFLAGS = $(TESTLDFLAGS)
libfixtures_la_LIBADD = $(top_builddir)/src/libtimer.la
