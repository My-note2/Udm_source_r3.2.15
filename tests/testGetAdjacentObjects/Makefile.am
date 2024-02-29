## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir)/include $(CPPUNITINC)

noinst_PROGRAMS		= TestGetAdjacentObjects
TESTS			= $(noinst_PROGRAMS)

BUILT_SOURCES			= PackageA.cpp PackageA.h PackageA.xsd \
				  PackageB.cpp PackageB.h PackageB.xsd \
				  testAssociationScenarios.cpp \
				  testAssociationScenarios.h \
				  testAssociationScenarios.xsd \
				  LampDiagram.cpp LampDiagram.h LampDiagram.xsd
TestGetAdjacentObjects_SOURCES	= main.cpp TestGetAdjacentObjects.cpp \
				  TestGetAdjacentObjects.h
nodist_TestGetAdjacentObjects_SOURCES	= $(BUILT_SOURCES)
TestGetAdjacentObjects_LDADD	= ../../lib/libudm.la $(CPPUNITLIBS)

CLEANFILES		= $(BUILT_SOURCES) sources-stamp testmodel.udm


sources-stamp: testAssociationScenarios_udm.udm ../../samples/LampDiagram_udm.xml ../../bin/Udm Makefile
	@rm -f sources-temp
	@touch sources-temp
	../../bin/Udm testAssociationScenarios_udm.udm -d ../../etc -T
	../../bin/Udm ../../samples/LampDiagram_udm.xml -d ../../etc -T
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
