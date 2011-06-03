#-------------------------------------------------
#
# Project created by QtCreator 2011-05-24T11:24:26
#
#-------------------------------------------------

QT       += core gui svg

TARGET = qtmindmap
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    graphwidget.cpp \
    node.cpp \
    edge.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    graphwidget.h \
    node.h \
    edge.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

RESOURCES = qtmindmap.qrc

TRANSLATIONS = qtmindmap_hu.ts

CODECFORTR = UTF-8
