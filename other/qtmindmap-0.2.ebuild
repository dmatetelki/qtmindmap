SLOT="0"
KEYWORDS="~x86 ~amd64"

EAPI="3"
inherit qt4-r2
# LANGS="hu nb"
# IUSE="linguas_hu linguas_nb"

DESCRIPTION="MindMap software written in Qt"
HOMEPAGE="https://gitorious.org/qtmindmap"
SRC_URI="http://matetelki.com/qtmindmap/qtmindmap-qtmindmap-master.tar.gz"
LICENSE="GPL-2"

# some functions introduced in qt 4.6
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

  qt4-r2_src_install
}