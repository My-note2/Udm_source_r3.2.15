## Makefile.am -- Process this file with automake to produce Makefile.in

ANTLRLIBS=
if ! NO_ANTLR
ANTLRLIBS += ../src/UdmOcl/libudmocl.la \
	../src/UdmOclPat/GOCL/src/libgocl.la
endif

lib_LTLIBRARIES		= libudm.la libUdmSwig.la
if ! NO_XALAN
lib_LTLIBRARIES += libudmxmi.la
endif

libudm_la_SOURCES	=
libudm_la_LIBADD	= \
	../src/Uml/libuml.la \
	../src/UdmPOI/libudmpoi.la \
	../src/UdmBase/libudmbase.la \
	../src/UdmUtil/libudmutil.la \
	../src/UdmDom/libudmdom.la \
	$(XERCESLIBS) $(ANTLRLIBS) $(MINIZIPLIBS) $(DLOPENLIBS)
libudm_la_LDFLAGS	= -version-info 3:1:3
# Force C++ linking
nodist_EXTRA_libudm_la_SOURCES	= dummy.cxx

libUdmSwig_la_SOURCES	=
libUdmSwig_la_LIBADD	= ../src/UdmSwig/libudmswig.la libudm.la
libUdmSwig_la_LDFLAGS	= -version-info 3:1:3
# Force C++ linking
nodist_EXTRA_libUdmSwig_la_SOURCES	= dummy.cxx

if ! NO_XALAN
libudmxmi_la_SOURCES	=
libudmxmi_la_LIBADD	= ../src/UdmXmi/libudmxmi.la libudm.la $(XERCESLIBS) $(XALANLIBS)
libudmxmi_la_LDFLAGS	= -version-info 3:1:3
# Force C++ linking
nodist_EXTRA_libudmxmi_la_SOURCES	= dummy.cxx
endif
