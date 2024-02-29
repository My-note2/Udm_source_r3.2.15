## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir)/include

BUILT_SOURCES		= CL.cpp CL.h CL.xsd \
			CD1.cpp CD1.h CD1.xsd \
			CD2.cpp CD2.h CD2.xsd \
			CD3.cpp CD3.h CD3.xsd

noinst_PROGRAMS		= cross_links
cross_links_SOURCES	= cross_links.cpp
nodist_cross_links_SOURCES	= $(BUILT_SOURCES)
cross_links_LDADD	= ../../lib/libudm.la

CLEANFILES	= $(BUILT_SOURCES) sources-stamp sample.udm sm1.xml sm2.xml sm3.xml

samplesdir		= $(UDM_SAMPLES_PATH)/cross_links
samples_DATA		= $(cross_links_SOURCES) ../CL_udm.udm Makefile.sample

install-data-hook:
	mv -f $(DESTDIR)$(samplesdir)/Makefile.sample \
		$(DESTDIR)$(samplesdir)/Makefile


sources-stamp: ../CL_udm.udm ../../bin/Udm Makefile
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
