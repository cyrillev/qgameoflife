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
    view/gameoflifeview.cpp \
    view/gameoflifescene.cpp \
    model/gameoflifepatternmodel.cpp \
    generic/model/golmodel.cpp \
    generic/model/genericgameoflifepatternmodel.cpp \
    model/cellpainter.cpp \
    view/patternsview.cpp \
    view/cellgraphicsitem.cpp \
    generic/model/golquadtree.cpp \
    generic/model/golsetmodel.cpp \
    generic/model/golquadtreenodebase.cpp \
    generic/model/golquadtreenodeleaf.cpp \
    generic/model/golquadtreenode.cpp

HEADERS  += mainwindow.h \
    view/gameoflifeview.h \
    view/gameoflifescene.h \
    model/gameoflifepatternmodel.h \
    generic/model/golmodel.h \
    generic/model/genericgameoflifepatternmodel.h \
    model/cellpainter.h \
    view/patternsview.h \
    view/cellgraphicsitem.h \
    generic/model/golquadtree.h \
    generic/model/goltypes.h \
    generic/model/golsetmodel.h \
    generic/model/golquadtreenodebase.h \
    generic/model/golquadtreenodeleaf.h \
    generic/model/golquadtreenode.h \
    generic/model/golmodelinterface.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    data/patterns/coral.rle \
    NOTES.txt

patterns.path = data/patterns/
patterns.files = *.rle
INSTALLS += patterns


# boost
INCLUDEPATH += /opt/local/include
QMAKE_LIBDIR += /opt/local/lib
LIBS += -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt -lboost_regex-mt -lboost_signals-mt
