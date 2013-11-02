#-------------------------------------------------
#
# Project created by QtCreator 2013-10-31T13:09:09
#
#-------------------------------------------------

QT       += core gui network multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LearningFromSub
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    subtitlesview.cpp \
    subservice.cpp \
    translater.cpp \
    sqlprovider.cpp

HEADERS  += mainwindow.h \
    subtitlesview.h \
    subservice.h \
    translater.h \
    sqlprovider.h

FORMS    += mainwindow.ui \
    subtitlesview.ui
