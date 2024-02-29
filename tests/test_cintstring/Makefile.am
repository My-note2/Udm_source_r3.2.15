## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir)/include $(CPPUNITINC)

noinst_PROGRAMS		= test_cintstring
TESTS			= $(noinst_PROGRAMS)

test_cintstring_SOURCES	= cintstringTest.cpp cintstringTest.h \
			  test_cintstring.cpp
test_cintstring_LDADD	= ../../lib/libudm.la $(CPPUNITLIBS)
