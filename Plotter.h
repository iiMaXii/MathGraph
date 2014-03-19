//
//  Plotter.h
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#ifndef __MathGraph__Plotter__
#define __MathGraph__Plotter__

#include "real.h"
#include "Expression.h"
#include "Point.h"
#include <vector>
#include <utility>

class InvalidSelection : public std::logic_error
{
public:
    InvalidSelection(const std::string& _what_arg);
};

class Plotter
{
public:
    typedef std::vector<Expression>::size_type size_type;
	typedef std::vector<Expression>::const_iterator const_iterator;
    const size_type npos = -1;
    
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
    std::vector<bool> expressionIsHidden;
    size_type selectedExpression = npos;
    
	int xPtToPx(real x) const;
	int yPtToPx(real y) const;
	
	real xPxToPt(int x) const;
	real yPxToPt(int y) const;

public:
	Plotter(int _pixelWidth, int _pixelHeight, real _xMin, real _xMax, real _yMin, real _yMax, double _samplingRate = 2, int _pixelMarkerGap = 100);
    Plotter(int _pixelWidth, int _pixelHeight);
	Plotter();
    
	void addExpression(const std::string &expression);
	void addExpression(const Expression &expression);
    
    void centerOrigo();
    void setBounds(real _xMin, real _xMax, real _yMin, real _yMax);
    void setBounds(int xPixelMin, int xPixelMax, int yPixelMin, int yPixelMax);
    void setSamplingRate(real _samplingRate);
    
    void resize(int _pixelWidth, int _pixelHeight);
    void move(int diffX, int diffY);
    void zoom(int steps, int x = -1, int y = -1);
    
    Point<int> getOrigo() const;
    std::vector<std::pair<int, std::string>> getXMarkers() const;
    std::vector<std::pair<int, std::string>> getYMarkers() const;
    
    size_type numExpressions() const;
    void setHidden(size_type expressionIndex, bool hidden);
    bool isHidden(size_type expressionIndex) const;
    void select(size_type expressionIndex);
    void clearSelection();
    bool isSelected(size_type expressionIndex) const;
    std::vector<Point<int>> getPlotSamples(size_type expressionIndex) const;
    std::pair<Point<int>, Point<std::string>> getPointFromSelected(int x) const;

	const_iterator cbegin() const;
	const_iterator cend() const;
};

#endif /* defined(__MathGraph__Plotter__) */
