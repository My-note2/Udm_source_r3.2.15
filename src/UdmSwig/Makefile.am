## Makefile.am -- Process this file with automake to produce Makefile.in

SWIG			= @swig@

SWIG_FLAGS = 
if NO_ANTLR
SWIG_FLAGS += -DNO_ANTLR
endif

# gcc 4 optimizations based on strict aliasing break constructs like
# jresult = *(UdmPseudoObject **)&result; // jresult is not changed
AM_CPPFLAGS		= -I$(top_srcdir)/include -fno-strict-aliasing

noinst_LTLIBRARIES	= libudmswig.la
libudmswig_la_SOURCES	= UdmSwigMain.cpp
nodist_libudmswig_la_SOURCES	= UdmSwig_wrap.cpp
BUILT_SOURCES		= UdmSwig_wrap.cpp

EXTRA_DIST		= UdmSwig.i

CLEANFILES		= $(BUILT_SOURCES) sources-stamp


sources-stamp: UdmSwig.i Makefile
	@rm -f sources-temp
	@touch sources-temp
	mkdir -p ../../judm/src/UdmSwig/edu/vanderbilt/isis/udm/swig
	$(SWIG) $(SWIG_FLAGS) -java -package edu.vanderbilt.isis.udm.swig \
		-outdir ../../judm/src/UdmSwig/edu/vanderbilt/isis/udm/swig \
		-v -c++ -o UdmSwig_wrap.cpp $(srcdir)/UdmSwig.i
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
