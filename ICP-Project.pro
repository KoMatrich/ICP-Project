#-------------------------------------------------
#
# Project created by QtCreator 2022-03-13T21:48:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ICP-Project
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17

SOURCES += main.cpp\
        mainwindow.cpp \
    graphview.cpp \
    highlighter.cpp

HEADERS  += mainwindow.h \
    graphview.h \
    highlighter.h \
    pch/precompile_header.h

FORMS    +=

PRECOMPILED_HEADER = 'pch/precompile_header.h'
CONFIG += precompile_header