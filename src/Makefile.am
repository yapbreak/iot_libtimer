ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

include $(top_srcdir)/Makefile.common

lib_LTLIBRARIES = libtimer.la
libtimer_la_SOURCES  = timer.h
libtimer_la_SOURCES += timer.cpp
libtimer_la_SOURCES += event.cpp
libtimer_la_SOURCES += event.h

include_HEADERS = timer.h event.h
