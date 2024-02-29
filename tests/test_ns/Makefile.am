## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir)/include $(CPPUNITINC)

noinst_PROGRAMS		= test_ns
TESTS			= $(noinst_PROGRAMS)

BUILT_SOURCES		= test_ns.cpp test_ns.h test_ns.xsd \
			  test_ns_A.xsd test_ns_B.xsd test_ns_AB.xsd
test_ns_SOURCES		= main.cpp test_ns_test.cpp
nodist_test_ns_SOURCES	= $(BUILT_SOURCES)
test_ns_LDADD		= ../../lib/libudm.la $(CPPUNITLIBS)

CLEANFILES		= $(BUILT_SOURCES) sources-stamp test.xml


sources-stamp: test_ns_udm.xml ../../bin/Udm Makefile
	@rm -f sources-temp
	@touch sources-temp
	../../bin/Udm $< -v -d ../../etc -T
	@mv -f sources-temp $@

$(BUILT_SOURCES): sources-stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
		trap 'rm -rf sources-lock sources-stamp' 1 2 13 15; \
		if mkdir sources-lock 2>/dev/null; then \
## This code is being executed by the first process.
			rm -f sources-stamp; \
			$(MAKE) $(AM_MAKEFLAGS) sources-stamp; \
			rmdir sources-lock; \
		else \
## This code is being executed by the follower processes.
## Wait until the first process is done.
			while test -d sources-lock; do sleep 1; done; \
## Succeed if and only if the first process succeeded.
			test -f sources-stamp; exit $$?; \
		fi; \
	fi
