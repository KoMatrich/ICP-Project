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
        mainwindow.cpp \
    highlighter.cpp \
    ERDview.cpp \
    SEQview.cpp \
    MainTextEdit.cpp \
    DebugTextEdit.cpp

HEADERS  += mainwindow.h \
    highlighter.h \
    ERDview.h \
    SEQview.h \
    MainTextEdit.h \
    DebugTextEdit.h

FORMS    +=
