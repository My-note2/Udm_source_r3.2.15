#!/bin/bash

VERSION=3.13

# http://stackoverflow.com/questions/880227/what-is-the-minimum-i-have-to-do-to-create-an-rpm-file

set -e
set -u

if [ -d udm-$VERSION ]; then rm -rf udm-$VERSION; fi
mkdir udm-$VERSION
pushd ..
pushd debian/zlib-1.2.3.3.dfsg/contrib/minizip && make libminizip && popd
autoreconf -i
./configure LDFLAGS="-L$(pwd)/debian/zlib-1.2.3.3.dfsg/contrib/minizip/" CPPFLAGS="-I$(pwd)/debian/zlib-1.2.3.3.dfsg/contrib/"  CXXFLAGS="-I$(pwd)/debian/zlib-1.2.3.3.dfsg/contrib/" --without-xalan --without-antlr --prefix=/usr
make DESTDIR="$(pwd)"/rpmbuild_CentOS/udm-$VERSION install
popd

tar -zcvf udm-$VERSION.tar.gz udm-$VERSION
cp -f udm-$VERSION.tar.gz SOURCES/

rpmbuild --define "_topdir $(pwd)" -ba SPECS/udm.spec
