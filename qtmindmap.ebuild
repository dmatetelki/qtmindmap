DESCRIPTION="MindMap software written in Qt"
HOMEPAGE="https://gitorious.org/qtmindmap"
SRC_URI="https://gitorious.org/qtmindmap/qtmindmap/archive-tarball/master"
LICENSE=""

SLOT="0"
KEYWORDS="~x86"
IUSE=""

DEPEND=">=x11-libs/qt-core-4.6"
RDEPEND="${DEPEND}"

src_compile() {
  qmake qtmindmap.pro
  emake || die "emake failed"
}

src_install() {
  emake DESTDIR="${D}" install || die "emake install failed"
}