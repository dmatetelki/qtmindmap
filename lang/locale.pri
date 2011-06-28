CODECFORTR    = UTF-8

TRANSLATIONS = lang/qtmindmap_hu.ts \
               lang/qtmindmap_nb_NO.ts

qmfiles = .qm/i18n/qtmindmap_hu.qm \
          .qm/i18n/qtmindmap_nb_NO.qm

system(mkdir -p .qm/i18n && touch $${qmfiles})


isEmpty(QMAKE_LRELEASE)
{
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.input = TRANSLATIONS

updateqm.output = .qm/i18n/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE -silent ${QMAKE_FILE_IN} -qm .qm/i18n/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm
PRE_TARGETDEPS += compiler_updateqm_make_all
