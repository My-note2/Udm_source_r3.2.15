## Makefile.am -- Process this file with automake to produce Makefile.in


OPTPROGRAMS =
if ! NO_XALAN
OPTPROGRAMS += UdmToXmi 
OPTPROGRAMS += XmiToUdm
endif
if ! NO_ANTLR
OPTPROGRAMS += UdmOclPat
endif


bin_PROGRAMS		= Udm UdmBackendDump UdmCopy UdmViz $(OPTPROGRAMS)


Udm_SOURCES		=
Udm_LDADD		= \
	../src/Udm/libudm.la \
	../lib/libudm.la
nodist_EXTRA_Udm_SOURCES	= dummy.cxx


UdmBackendDump_SOURCES	=
UdmBackendDump_LDADD	= \
	../src/UdmBackendDump/libudmbackenddump.la \
	../lib/libudm.la
nodist_EXTRA_UdmBackendDump_SOURCES	= dummy.cxx


UdmCopy_SOURCES	=
UdmCopy_LDADD	= \
	../src/UdmCopy/libudmcopy.la \
	../lib/libudm.la
nodist_EXTRA_UdmCopy_SOURCES	= dummy.cxx


UdmOclPat_SOURCES	=
UdmOclPat_LDADD	= \
	../src/UdmOclPat/libudmoclpat.la \
	../lib/libudm.la
nodist_EXTRA_UdmOclPat_SOURCES	= dummy.cxx


UdmToXmi_SOURCES	=
UdmToXmi_LDADD	= \
	../src/UdmToXmi/libudmtoxmi.la \
	../lib/libudm.la \
	../lib/libudmxmi.la
nodist_EXTRA_UdmToXmi_SOURCES	= dummy.cxx


UdmViz_SOURCES	=
UdmViz_LDADD	= \
	../src/UdmViz/libudmviz.la \
	../lib/libudm.la
nodist_EXTRA_UdmViz_SOURCES	= dummy.cxx


XmiToUdm_SOURCES	=
XmiToUdm_LDADD	= \
	../src/XmiToUdm/libxmitoudm.la \
	../lib/libudm.la \
	../lib/libudmxmi.la
nodist_EXTRA_XmiToUdm_SOURCES	= dummy.cxx
