## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir)/include $(CPPUNITINC)

noinst_PROGRAMS		= test_CrossLinks
TESTS			= $(noinst_PROGRAMS)

BUILT_SOURCES		= CL.cpp CL.h CL.xsd \
			  CLPackage.cpp CLPackage.h CLPackage.xsd \
			  Package1.cpp Package1.h Package1.xsd \
			  Package2.cpp Package2.h Package2.xsd
test_CrossLinks_SOURCES	= CrossLinkTester.cpp CrossLinkTester.h \
			  test_CrossLinks.cpp
nodist_test_CrossLinks_SOURCES	= $(BUILT_SOURCES)
test_CrossLinks_LDADD	= ../../lib/libudm.la $(CPPUNITLIBS)

CLEANFILES		= $(BUILT_SOURCES) sources-stamp


sources-stamp: CL_udm.udm ../../bin/Udm Makefile
	@rm -f sources-temp
	@touch sources-temp
	../../bin/Udm $< -d ../../etc -T
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
