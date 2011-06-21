#-------------------------------------------------
#
# Project created by QtCreator 2011-05-24T11:24:26
#
#-------------------------------------------------

QT       += core gui svg xml

TARGET = qtmindmap
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    graphwidget.cpp \
    node.cpp \
    edge.cpp \
    systemtray.cpp \
    argumentparser.cpp

HEADERS  += mainwindow.h \
    graphwidget.h \
    node.h \
    edge.h \
    systemtray.h \
    argumentparser.h

FORMS    += mainwindow.ui

RESOURCES = qtmindmap.qrc

TRANSLATIONS = qtmindmap_hu.ts

CODECFORTR = UTF-8
