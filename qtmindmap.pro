#-------------------------------------------------
#
# Project created by QtCreator 2011-05-24T11:24:26
#
#-------------------------------------------------

QT       += core gui svg xml

CONFIG   += warn_on

TARGET = qtmindmap

TEMPLATE = app


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

FORMS    += ui/mainwindow.ui

RESOURCES = images/qtmindmap.qrc

TRANSLATIONS += lang/qtmindmap_hu.ts \
                lang/qtmindmap_nb_NO.ts

CODECFORTR = UTF-8
