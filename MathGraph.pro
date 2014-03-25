#
#   MathGraph.pro
#   MathGraph
#
#   Copyright Max Ekström. Licensed under GPL v3 (see README).
#
#

CONFIG += c++11

QT += core gui
QT += widgets
QT += webkit
QT += webkitwidgets

TARGET = MathGraph
TEMPLATE = app

RESOURCES += images.qrc

SOURCES +=  Expression.cpp \
            MainWindow.cpp \
            Plotter.cpp \
            RenderArea.cpp \
            TokenReader.cpp \
            main.cpp \
            real.cpp \
            QListWidgetFunctionItem.cpp \
            HelpWindow.cpp

HEADERS  += Expression.h \
            MainWindow.h \
            Plotter.h \
            Point.h \
            RenderArea.h \
            TokenReader.h \
            real.h \
            QListWidgetFunctionItem.h \
            HelpWindow.h
