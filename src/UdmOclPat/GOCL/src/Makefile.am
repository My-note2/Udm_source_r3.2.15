## Makefile.am -- Process this file with automake to produce Makefile.in

ANTLR			= @antlr@
DLG			= @dlg@
PCCTSDIR		= @pcctsdir@


AM_CPPFLAGS		= -I$(PCCTSDIR) -I$(top_srcdir)/Include

BUILT_SOURCES		= OCLLexer.cpp OCLLexer.h OCLParser.cpp \
			  OCLParser.dlg OCLParser.h ocl.cpp OCLTokens.h

PCCTS_SRCS		= $(PCCTSDIR)/ATokenBuffer.cpp \
			  $(PCCTSDIR)/AParser.cpp \
			  $(PCCTSDIR)/DLexerBase.cpp

SRCS			= OCLCommon.cpp OCLConstraint.cpp OCLContext.cpp \
			  OCLException.cpp OCLFactory.cpp OCLFeature.cpp \
			  OCLFeatureImplementation.cpp OCLObject.cpp \
			  OCLObjectExBasic.cpp OCLParserStatic.cpp \
			  OCLRefCount.cpp OCLSignature.cpp OCLTree.cpp \
			  OCLType.cpp OCLTypeExBasic.cpp

noinst_LTLIBRARIES	= libgocl.la
libgocl_la_SOURCES	= $(SRCS)
nodist_libgocl_la_SOURCES	= $(PCCTS_SRCS) $(BUILT_SOURCES)
libgocl_la_LDFLAGS	= -version-info 1:0:0

EXTRA_DIST		= ocl.g

CLEANFILES		= $(BUILT_SOURCES) sources-stamp


sources-stamp: ocl.g Makefile
	@rm -f sources-temp
	@touch sources-temp
	$(ANTLR) -CC -ft OCLTokens.h -fl OCLParser.dlg -mrhoist off \
		-o `pwd` \
		$(srcdir)/ocl.g
	$(DLG) -C2 -i -CC -cl OCLLexer OCLParser.dlg
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
