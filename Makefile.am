## Makefile.am -- Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign
SUBDIRS = . doc etc include src lib bin samples tests judm rpm

EXTRA_DIST	= CHANGELOG.txt license.txt Linux_build.txt \
	model-files.zip readme.txt releaseinfo.txt

all-local: model-files.zip
	unzip -x -o $(srcdir)/model-files.zip

rpms: all
	$(MAKE) rpms -C rpm
