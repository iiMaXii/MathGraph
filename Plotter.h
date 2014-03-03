//
//  Plotter.h
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#ifndef __MathGraph__Plotter__
#define __MathGraph__Plotter__

#include "real.h"
#include "Expression.h"
#include "Point.h"
#include <vector>
#include <utility>

class Plotter
{
public:
    typedef std::vector<Expression>::size_type size_type;
	typedef std::vector<Expression>::const_iterator const_iterator;
private:
    int pixelWidth;
    int pixelHeight;
    
    real xMin, xMax;
    real yMin, yMax;
    
    // Step in pixels between plot points
    double samplingRate;
    
    // Desired gap between markers
    int pixelMarkerGap;
    
    std::vector<Expression> expressions;
    
public:
    Plotter(int _pixelWidth, int _pixelHeight, real _xMin, real _xMax, real _yMin, real _yMax, double _samplingRate = 2, int _pixelMarkerGap = 100);
    Plotter(int _pixelWidth, int _pixelHeight);
	Plotter();
    
	void addExpression(std::string expression);
	void addExpression(const Expression &expression);
    
    void centerOrigo();
    void setBounds(real _xMin, real _xMax, real _yMin, real _yMax);
    void setSamplingRate(real _samplingRate);
    
    void resize(int _pixelWidth, int _pixelHeight);
    void move(int diffX, int diffY);
    void zoom(int steps, int x = -1, int y = -1);
    
    Point<int> getOrigo() const;
    std::vector<std::pair<int, std::string>> getXMarkers() const;
    std::vector<std::pair<int, std::string>> getYMarkers() const;
    
    size_type numExpressions() const;
    std::vector<Point<int>> getPlotSamples(size_type expressionIndex) const;
    Point<real> getPoint(size_type expressionIndex, real x) const;
    
	const_iterator cbegin() const;
	const_iterator cend() const;
};

#endif /* defined(__MathGraph__Plotter__) */
