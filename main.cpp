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

/**
 * To do:
 *  - Function with no parenthesises around argument gives unexpected results, throw error (!)
 *  - Implicit multiplication not implemented (eg. 5(1+2) or 5a)
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
