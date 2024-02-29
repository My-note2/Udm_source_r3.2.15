## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir)/include $(CPPUNITINC)

EXTRA_DIST		= versionCheck.xml

noinst_PROGRAMS		= Test
TESTS			= $(noinst_PROGRAMS)

BUILT_SOURCES		= SBML2.cpp SBML2.h SBML2.dtd SBML2.xsd \
			  SBML2_xsd.h SBML2_sbml_xsd.h SBML2_math_xsd.h \
			  SBML2_math.dtd SBML2_math.xsd \
			  SBML2_sbml.dtd SBML2_sbml.xsd
Test_SOURCES		= main.cpp Test.cpp Test.h
nodist_Test_SOURCES	= $(BUILT_SOURCES)
Test_LDADD	= ../../lib/libudm.la $(CPPUNITLIBS)

CLEANFILES		= $(BUILT_SOURCES) sources-stamp test_out.xml


sources-stamp: SBML2_udm.xml ../../bin/Udm Makefile
	@rm -f sources-temp
	@touch sources-temp
	../../bin/Udm $< \
		-d ../../etc -t -v -g -T \
		-u math=http://www.w3.org/1998/Math/MathML \
		-u sbml=http://www.sbml.org/sbml/level2
	@if test "$(top_srcdir)" != "$(top_builddir)"; then \
		cp -f "$(srcdir)/versionCheck.xml" .; \
	fi
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
