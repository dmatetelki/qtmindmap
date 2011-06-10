QT       += core gui svg

CONFIG += qtestlib

TARGET = qtmindmap_test

SOURCES += mainwindow.cpp \
    aboutdialog.cpp \
    graphwidget.cpp \
    node.cpp \
    edge.cpp \
    systemtray.cpp \
    argumentparser.cpp \
    algorithmtests.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    graphwidget.h \
    node.h \
    edge.h \
    systemtray.h \
    argumentparser.h \
    algorithmtests.h

FORMS    += mainwindow.ui \
    aboutdialog.ui
