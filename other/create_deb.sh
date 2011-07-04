#!/bin/bash

# version="0.1"
version="0.2"
# version="master"



if [ "$version" = "master" ]; then
  packageversion="9999"
else
  packageversion=$version
fi


packagedir="/tmp/qtmindmap-${packageversion}"
tempdir="/tmp/qtmindmap-qtmindmap"
currdir=`pwd`


# get source
wget -c http://gitorious.org/qtmindmap/qtmindmap/archive-tarball/${version} -O /tmp/qtmindmap-${packageversion}.tar.gz
cd /tmp
tar -xvf qtmindmap-${packageversion}.tar.gz

# compile
cd $tempdir
qmake qtmindmap.pro
make

#copy
rm -rf $packagedir

mkdir -p $packagedir/usr/bin
cp $tempdir/qtmindmap $packagedir/usr/bin/

if [ "$version" != "0.1" ]; then
  mkdir -p $packagedir/usr/share/applications
  mkdir -p $packagedir/usr/share/pixmaps
  mkdir -p $packagedir/usr/share/qtmindmap/i18n
  cp $tempdir/other/qtmindmap.desktop $packagedir/usr/share/applications/
  cp $tempdir/images/qtmindmap.svg $packagedir/usr/share/pixmaps/
  cp $tempdir/.qm/i18n/*.qm $packagedir/usr/share/qtmindmap/i18n/
fi

#package
mkdir $packagedir/DEBIAN/

echo "Package: qtmindmap
Version: ${packageversion}
Section: base
Priority: optional
Architecture: all
Depends:  libqtcore4 (>= 4.6), libqtgui4 (>= 4.6), libqt4-svg (>= 4.6), libqt4-xml (>= 4.6)
Maintainer: Denes Matetelki <denes.matetelki@gmial.com>
Homepage: https://gitorious.org/qtmindmap
Description: Mindmap software in Qt." > $packagedir/DEBIAN/control

cd $currdir
dpkg --build $packagedir

#cleanup
rm /tmp/qtmindmap-${packageversion}.tar.gz
rm -rf $packagedir
rm -rf $tempdir
