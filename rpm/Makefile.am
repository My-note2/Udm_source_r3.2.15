## Makefile.am -- Process this file with automake to produce Makefile.in

ARCH = $(shell uname -i)

rpms: RPMS/$(ARCH)/udm-@PACKAGE_VERSION@-0localbuild.$(ARCH).rpm SRPMS/udm-@PACKAGE_VERSION@-0localbuild.srpm

RPMS/$(ARCH)/udm-@PACKAGE_VERSION@-0localbuild.$(ARCH).rpm: prepare_build_env
	rpmbuild --define "_topdir `pwd`" -bb SPECS/udm.spec

SRPMS/udm-@PACKAGE_VERSION@-0localbuild.srpm: prepare_build_env
	rpmbuild --define "_topdir `pwd`" -bs SPECS/udm.spec

prepare_build_env: udm.spec
	mkdir -p RPMS/$(ARCH) BUILD SOURCES SPECS SRPMS
	cp udm.spec SPECS
	(cd .. && make dist && mv udm-@PACKAGE_VERSION@.tar.gz rpm/SOURCES)

clean:
	rm -rf BUILD RPMS SOURCES SPECS SRPMS
