//
//  main.cpp
//  MathGraph
//
//  Copyright Max Ekstršm. Licensed under GPL v3 (see README).
//
//

#include "MainWindow.h"
#include "HelpWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
