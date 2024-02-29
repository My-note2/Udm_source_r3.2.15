## Makefile.am -- Process this file with automake to produce Makefile.in


SUBDIRS = Uml UdmBase UdmDom UdmUtil UdmPOI UdmSwig \
	  Udm UdmCopy UdmBackendDump UdmViz 
if ! NO_XALAN
SUBDIRS += UdmXmi UdmToXmi XmiToUdm
endif

if ! NO_ANTLR
SUBDIRS += UdmOcl UdmOclPat
endif