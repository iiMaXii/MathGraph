#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T17:38:04
#
#-------------------------------------------------

CONFIG += c++11

QT += core gui
QT += widgets

TARGET = MathGraph
TEMPLATE = app


SOURCES +=  Expression.cpp \
            MainWindow.cpp \
            Plotter.cpp \
            RenderArea.cpp \
            TokenReader.cpp \
            main.cpp \
            real.cpp

HEADERS  += DEBUG.h \
            Expression.h \
            MainWindow.h \
            Plotter.h \
            Point.h \
            RenderArea.h \
            TokenReader.h \
            real.h
