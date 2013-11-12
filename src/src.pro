QT += core gui network multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LearningFromSub
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
    mainwindow.cpp\
    subtitlesview.cpp\
    subservice.cpp\
    translater.cpp\
    sqlprovider.cpp 

HEADERS += headers/mainwindow.h\
    headers/subtitlesview.h\
    headers/subservice.h\
    headers/translater.h\
    headers/sqlprovider.h 

FORMS += ui/mainwindow.ui \
    ui/subtitlesview.ui

RESOURCES += \
    resources/res.qrc
