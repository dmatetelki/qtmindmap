SLOT="0"
KEYWORDS="~x86"

EAPI="3"
inherit qt4-r2
LANGS="hu"
IUSE="linguas_hu"

DESCRIPTION="MindMap software written in Qt"
HOMEPAGE="https://gitorious.org/qtmindmap"
SRC_URI="http://matetelki.com/qtmindmap/qtmindmap-qtmindmap-master.tar.gz"
LICENSE="GPL-2"

DEPEND=">=x11-libs/qt-core-4.6
        >=x11-libs/qt-gui-4.6
        >=x11-libs/qt-svg-4.6
        >=x11-libs/qt-test-4.6
        >=x11-libs/qt-xmlpatterns-4.6"

RDEPEND="${DEPEND}"

src_configure() {
  eqmake4 "${S}"/qtmindmap-qtmindmap/qtmindmap.pro
}

src_install() {
    echo "hello"
    ls
    ls -R
    ls ../ -R
    # do normal installation
    # qt4-r2_src_install
    # Install translations
    # TODO how?
}