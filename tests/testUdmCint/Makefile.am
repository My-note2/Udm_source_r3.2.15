## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir)/include $(CPPUNITINC)

EXTRA_DIST		= versionCheck.xml

noinst_PROGRAMS		= testUdmCint
TESTS			= $(noinst_PROGRAMS)

GeneTF_BUILT_SOURCES		= GeneTF.cpp GeneTF.h GeneTF.dtd GeneTF.xsd \
	GeneTF_xsd.h GeneTF_GeneTF_xsd.h \
	GeneTF_GeneTF.dtd GeneTF_GeneTF.xsd
SBML2Ex_BUILT_SOURCES		= SBML2Ex.cpp SBML2Ex.h \
	SBML2Ex_xsd.h SBML2Ex_bc_xsd.h SBML2Ex_dbi_xsd.h SBML2Ex_flux_xsd.h \
	SBML2Ex_jd_xsd.h SBML2Ex_jigcell_xsd.h SBML2Ex_math_xsd.h \
	SBML2Ex_sbml_xsd.h SBML2Ex_unigene_xsd.h SBML2Ex_xhtml_xsd.h \
	SBML2Ex_bc.xsd SBML2Ex_dbi.xsd SBML2Ex_flux.xsd SBML2Ex_jd.xsd \
	SBML2Ex_jigcell.xsd SBML2Ex_math.xsd SBML2Ex_sbml.xsd \
	SBML2Ex_unigene.xsd SBML2Ex_xhtml.xsd SBML2Ex.xsd

BUILT_SOURCES		= $(GeneTF_BUILT_SOURCES) $(SBML2Ex_BUILT_SOURCES)

testUdmCint_SOURCES	= main.cpp Test.h testReadSBML2Ex.cpp
nodist_testUdmCint_SOURCES	= $(BUILT_SOURCES)
testUdmCint_LDADD	= ../../lib/libudm.la ../../lib/libUdmSwig.la \
	$(CPPUNITLIBS)
testUdmCint_LDFLAGS	= -Wl,-zmuldefs

CLEANFILES		= $(BUILT_SOURCES) sources-stamp


sources-stamp: GeneTF_udm.xml SBML2Ex_udm.xml ../../bin/Udm Makefile
	@rm -f sources-temp
	@touch sources-temp
	../../bin/Udm GeneTF_udm.xml \
		-d ../../etc -t -v -g -T \
		-u GeneTF=http://kutykuruty.khm.edu
	../../bin/Udm SBML2Ex_udm.xml \
		-d ../../etc -g -v -e -T -q jigcell -i xhtml \
		-u math=http://www.w3.org/1998/Math/MathML \
		-u sbml=http://www.sbml.org/sbml/level2 \
		-u dbi=http://www.dbi.tju.edu/xmlns/dbi \
		-u unigene=http://www.dbi.tju.edu/xmlns/unigene \
		-u xhtml=http://www.w3.org/1999/xhtml \
		-u jigcell=http://mpf.biol.vt.edu/ns \
		-u bc=http://www.sbml.org/2001/ns/biocharon \
		-u flux=http://arep.med.harvard.edu/fluxns
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
