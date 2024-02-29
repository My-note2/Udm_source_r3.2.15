## Makefile.am -- Process this file with automake to produce Makefile.in

SUBDIRS			= doc etc samples src
AM_MAKEFLAGS		= -j1

EXTRA_DIST		= build.xml

OBJS = build/build_linux/GeneTF/GeneTF.jar \
	build/build_linux/GeneTF/GeneTF-test.jar \
	build/build_linux/GeneTRE/GeneTRE.jar \
	build/build_linux/GeneTRE/GeneTRE-test.jar \
	build/build_linux/SBML2Ex/SBML2Ex.jar \
	build/build_linux/SBML2Ex/SBML2Ex-test.jar \
	build/build_linux/TimeSeries/TimeSeries.jar \
	build/build_linux/TimeSeries/TimeSeries-test.jar \
	build/build_linux/judm/judm.jar

all: $(OBJS)

clean: build.xml
	ant -buildfile $(srcdir)/build.xml \
		-Dudm.dir=$(top_builddir) \
		-Dudm.src.dir=$(srcdir)/.. \
		-Dbasedir=$(top_builddir)/judm \
		-Dsrcdir=$(srcdir) \
		clean
	@rm -f sources-stamp

check: $(OBJS)
	UDM_PATH=`pwd`/.. ant \
		-buildfile $(srcdir)/build.xml \
		-Dudm.dir=$(top_builddir) \
		-Dudm.src.dir=$(srcdir)/.. \
		-Dbasedir=$(top_builddir)/judm \
		-Dsrcdir=$(srcdir) \
		test


sources-stamp: build.xml ../bin/Udm Makefile
	@rm -f sources-temp
	@touch sources-temp
	ant -buildfile $(srcdir)/build.xml \
		-Dudm.dir=$(top_builddir) \
		-Dudm.src.dir=$(srcdir)/.. \
		-Dbasedir=$(top_builddir)/judm \
		-Dsrcdir=$(srcdir) \
		rebuild
	@mv -f sources-temp $@

$(OBJS): sources-stamp
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
