#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T22:37:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qgameoflife
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    model/golmodel.cpp \
    view/gameoflifeview.cpp \
    view/gameoflifescene.cpp \
    model/gameoflifepatternmodel.cpp \
    generic/model/genericgameoflifepatternmodel.cpp \
    model/cellpainter.cpp \
    view/patternsview.cpp \
    view/cellgraphicsitem.cpp

HEADERS  += mainwindow.h \
    model/golmodel.h \
    view/gameoflifeview.h \
    view/gameoflifescene.h \
    model/gameoflifepatternmodel.h \
    generic/model/genericgameoflifepatternmodel.h \
    model/cellpainter.h \
    view/patternsview.h \
    view/cellgraphicsitem.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    data/patterns/coral.rle

patterns.path = data/patterns/
patterns.files = *.rle
INSTALLS += patterns


# boost
INCLUDEPATH += /opt/local/include
QMAKE_LIBDIR += /opt/local/lib
LIBS += -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt -lboost_regex-mt
