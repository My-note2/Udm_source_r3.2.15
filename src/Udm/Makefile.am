## Makefile.am -- Process this file with automake to produce Makefile.in

SUBDIRS	= File2Code JavaAPIGen PythonAPIGen

AM_CPPFLAGS		= -I$(top_srcdir)/include -I$(top_srcdir)/src $(MINIZIPINC)

noinst_LTLIBRARIES	= libudm.la
libudm_la_SOURCES	= Udm.h Udm.cpp UdmCpp.cpp UdmCppH.cpp \
			  UdmUtil.cpp \
			  ClassGen.cpp DiagramGen.cpp NamespaceGen.cpp

# UdmCS.cpp
libudm_la_LIBADD	= File2Code/libudm_file2code.la \
			  JavaAPIGen/libudm_javaapigen.la \
			  PythonAPIGen/libudm_pythonapigen.la
