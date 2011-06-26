
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

RESOURCES     = images/qtmindmap.qrc

TRANSLATIONS += lang/qtmindmap_hu.ts \
                lang/qtmindmap_nb_NO.ts

CODECFORTR    = UTF-8

#DESTDIR = usr/bin
