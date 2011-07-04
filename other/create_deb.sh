#!/bin/bash

EXPECTED_ARGS=1
E_BADARGS=65

if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` <path to qtmindmap source>"
  exit $E_BADARGS
fi

srcdir=$1
packagedir="/tmp/qtmindmap-0.2"
tempdir="/tmp/qtmindmap-build"
currdir=`pwd`


# compile
rm -rf $tempdir
mkdir $tempdir
cd $tempdir
qmake $srcdir/qtmindmap.pro
make

#copy
rm -rf $packagedir
mkdir -p $packagedir/usr/bin
mkdir -p $packagedir/usr/share/applications
mkdir -p $packagedir/usr/share/pixmaps
mkdir -p $packagedir/usr/share/qtmindmap/i18n

cp $tempdir/qtmindmap $packagedir/usr/bin/
cp $srcdir/other/qtmindmap.desktop $packagedir/usr/share/applications/
cp $srcdir/images/qtmindmap.svg $packagedir/usr/share/pixmaps/
cp $tempdir/.qm/i18n/*.qm $packagedir/usr/share/qtmindmap/i18n/

#package
mkdir $packagedir/DEBIAN/

echo "Package: qtmindmap
Version: 0.2
Section: base
Priority: optional
Architecture: all
Depends:  libqtcore4 (>= 4.6), libqtgui4 (>= 4.6), libqt4-svg (>= 4.6), libqt4-xml (>= 4.6)
Maintainer: Dénes Mátételki <denes.matetelki@gmial.com>
Homepage: https://gitorious.org/qtmindmap
Description: Mindmap software in Qt." > $packagedir/DEBIAN/control

cd $currdir
dpkg --build $packagedir
