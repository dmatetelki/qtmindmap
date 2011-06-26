SLOT="0"
KEYWORDS="~x86"

EAPI="3"
inherit qt4-r2
LANGS="hu nb"
IUSE="linguas_hu linguas_nb"

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

  lrelease "${S}"/qtmindmap-qtmindmap/lang/qtmindmap_hu.ts
  lrelease "${S}"/qtmindmap-qtmindmap/lang/qtmindmap_nb_NO.ts
}

src_install() {

    insopts -m0755
    insinto /usr/bin
    doins qtmindmap

    insinto /usr/share/qtmindmap/i18n
    doins "${S}"/qtmindmap-qtmindmap/lang/qtmindmap_hu.qm
    doins "${S}"/qtmindmap-qtmindmap/lang/qtmindmap_nb_NO.qm
}