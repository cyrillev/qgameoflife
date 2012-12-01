#-------------------------------------------------
#
# Project created by QtCreator 2012-11-28T22:01:38
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_quadtreeunittesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_quadtreeunittesttest.cpp \
    generic/model/golquadtree.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    generic/model/golquadtree.h


# boost
INCLUDEPATH += /opt/local/include
QMAKE_LIBDIR += /opt/local/lib
LIBS += -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt -lboost_regex-mt -lboost_signals-mt
