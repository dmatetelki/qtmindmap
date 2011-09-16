QT       += core gui svg xml

CONFIG += qtestlib

TARGET = qtmindmap_test

SOURCES += src/mainwindow.cpp \
           src/graphwidget.cpp \
           src/graphlogic.cpp \
           src/node.cpp \
           src/edge.cpp \
           src/systemtray.cpp \
           src/argumentparser.cpp \
           test/algorithmtests.cpp

HEADERS  += include/mainwindow.h \
            include/graphwidget.h \
            include/graphlogic.hpp \
            include/node.h \
            include/edge.h \
            include/systemtray.h \
            include/argumentparser.h \
            test/algorithmtests.h

FORMS    += ui/mainwindow.ui

RESOURCES = images/qtmindmap.qrc
