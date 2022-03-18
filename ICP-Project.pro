#-------------------------------------------------
#
# Project created by QtCreator 2022-03-13T21:48:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ICP-Project
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
    ERDview.cpp \
    SEQview.cpp \
    Highlighter.cpp \
    TextDebug.cpp \
    TextCode.cpp \
    MainWindow.cpp

HEADERS  += \
    ERDview.h \
    SEQview.h \
    Highlighter.h \
    TextDebug.h \
    TextCode.h \
    MainWindow.h

FORMS    +=
