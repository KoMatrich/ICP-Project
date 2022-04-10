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

FORMS    +=

HEADERS += \
    Code/Highlighter.h \
    Code/TextCode.h \
    Code/TextDebug.h \
    Graph/ERDview.h \
    Graph/SEQview.h \
    MainWindow.h \
    Graph/DragItem.h \
    Code/Syntax.h \
    Code/Analyzer.h

SOURCES += \
    Code/Highlighter.cpp \
    Code/TextDebug.cpp \
    Graph/ERDview.cpp \
    Graph/SEQview.cpp \
    main.cpp \
    MainWindow.cpp \
    Graph/DragItem.cpp \
    Code/Syntax.cpp \
    Code/Analyzer.cpp
