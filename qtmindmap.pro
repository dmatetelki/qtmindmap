
#version check qt: some functions introduced in 4.6
contains(QT_VERSION, ^4\\.[0-5]\\..*) {
message("Cannot build Qt Creator with Qt version $${QT_VERSION}.")
error("Use at least Qt 4.6.")
}

QT       += core gui svg xml

CONFIG   += warn_on

TARGET    = qtmindmap

TEMPLATE  = app


SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/graphwidget.cpp \
           src/node.cpp \
           src/edge.cpp \
           src/systemtray.cpp \
           src/argumentparser.cpp

HEADERS  += include/mainwindow.h \
            include/graphwidget.h \
            include/node.h \
            include/edge.h \
            include/systemtray.h \
            include/argumentparser.h

FORMS        += ui/mainwindow.ui

RESOURCES += images/qtmindmap.qrc


# the translation hack
include(lang/locale.pri)

INSTALLS += target translations desktop icon

target.path = /usr/bin

translations.files += .qm/i18n
translations.path += /usr/share/qtmindmap

desktop.files += other/qtmindmap.desktop
desktop.path += /usr/share/applications

icon.files += images/qtmindmap.svg
icon.path += /usr/share/pixmaps
