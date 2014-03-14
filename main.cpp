//
//  main.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#include "MainWindow.h"
#include <QApplication>

#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <cmath>
#include <map>

#include "Plotter.h"

//using namespace std;

/**
 * To do:
 *  - Function with no parenthesises around argument gives unexpected results, throw error (!)
 *  - Implicit multiplication not implemented (eg. 5(1+2) or 5a)
 */
/*int main(int argc, const char * argv[])
{
    Plotter plotter(800, 600, -10, 10, -1.2, 1.2, 2, 100);
    
	Expression::addFunction("sin", std::sin);
    
    string strExpression("x");
    
    try {
        plotter.addExpression(strExpression);
        
        Point<int> origo = plotter.getOrigo();
        cout << "origo = (" << origo.getX() << ", " << origo.getY() << ")" << endl;
        
        vector<Point<int>> samples = plotter.getPlotSamples(0);
        for(auto it = samples.cbegin(); it != samples.cend(); ++it)
        {
            cout << "(" << it->getX() << ", " << it->getY() << ")" << endl;
        }
        
        auto markers = plotter.getXMarkers();
        
        for (auto it = markers.cbegin(); it != markers.cend(); ++it)
        {
            cout << it->first << ", " << it->second << endl;
        }
        
        
    } catch (InvalidExpression &e) {
        cout << "Caugth exception: " << e.what() << endl;
        cout << "Error occured in (" << e.getPosition() << ", " << e.getLength() << "): " << strExpression.substr(e.getPosition(), e.getLength());
    }
    
#ifdef _WIN32
	system("pause");
#endif
    
    return 0;
}*/

int main(int argc, char * argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
