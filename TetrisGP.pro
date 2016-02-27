#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T18:47:12
#
#-------------------------------------------------

QT       += core gui xml concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = TetrisGP
TEMPLATE = app


SOURCES += main.cpp\
    control.cpp \
    tetrispiece.cpp \
    tetrisboard.cpp \
    tetriswindow.cpp \
    tetrisfeatures.cpp \
    tetriski.cpp \
    tetrisrunnable.cpp \
    individuum.cpp \
    tetrisrenderer.cpp \
    tetrisevaluation.cpp

HEADERS  += \
    control.h \
    tetrispiece.h \
    tetrisboard.h \
    tetriswindow.h \
    random.h \
    MersenneTwister.h \
    QPCTimer.h \
    tetrisfeatures.h \
    tetriski.h \
    tetrisrunnable.h \
    individuum.h \
    tetrisrenderer.h \
    tetrisevaluation.h

FORMS    += \
    TetrisWindow.ui

#CXXFLAGS += -O3

Debug:QMAKE_CXXFLAGS += -g3 -gstabs -O2
QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
QMAKE_LFLAGS += -stdlib=libc++

#INCLUDEPATH += "E:/Dateien/Programmierung/libs/boost_1_53_0"
