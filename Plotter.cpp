//
//  Plotter.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#include <iostream>
#include "Plotter.h"
#include <cmath>
#include <limits>

int Plotter::xPtToPx(real x) const
{
	return static_cast<int>(pixelWidth * (x - xMin) / (xMax - xMin));
}

int Plotter::yPtToPx(real y) const
{
	return static_cast<int>(pixelHeight * (1 - (y - yMin) / (yMax - yMin)));
}

real Plotter::xPxToPt(int x) const
{
	return (xMax - xMin) * x / pixelWidth + xMin;
}

real Plotter::yPxToPt(int y) const
{
	return 1 - (yMax - yMin) * y / pixelHeight + yMin;
}

Plotter::Plotter(int _pixelWidth, int _pixelHeight, real _xMin, real _xMax, real _yMin, real _yMax, double _samplingRate, int _pixelMarkerGap)
: pixelWidth(_pixelWidth), pixelHeight(_pixelHeight), xMin(_xMin), xMax(_xMax), yMin(_yMin), yMax(_yMax), samplingRate(_samplingRate), pixelMarkerGap(_pixelMarkerGap)
{
    Expression::addVariable("x", 0);
}

Plotter::Plotter(int _pixelWidth, int _pixelHeight)
: Plotter(_pixelWidth, _pixelHeight, -10, 10, -10, 10)
{}

Plotter::Plotter()
: Plotter(0, 0)
{}

void Plotter::addExpression(std::string expression)
{
    expressions.emplace_back(expression);
}

void Plotter::addExpression(const Expression &expression)
{
	expressions.push_back(expression);
}

void Plotter::centerOrigo()
{
    real x = (xMax - xMin) / 2;
    real y = (yMax - yMin) / 2;
    
    setBounds(-x, x, -y, y);
}

void Plotter::setBounds(real _xMin, real _xMax, real _yMin, real _yMax)
{
    xMin = _xMin;
    xMax = _xMax;
    
    yMin = _yMin;
    yMax = _yMax;
}

void Plotter::setSamplingRate(real _samplingRate)
{
    samplingRate = _samplingRate;
}

void Plotter::resize(int _pixelWidth, int _pixelHeight)
{
    pixelWidth = _pixelWidth;
    pixelHeight = _pixelHeight;
}

void Plotter::move(int diffX, int diffY)
{
	real xMoveFactor = pixelWidth / (xMax - xMin);
	real yMoveFactor = pixelHeight / (yMax - yMin);
    
	xMin += diffX / xMoveFactor;
	xMax += diffX / xMoveFactor;
    
	yMin -= diffY / yMoveFactor;
	yMax -= diffY / yMoveFactor;
}

void Plotter::zoom(int steps, int x, int y)
{
    if (steps != 0)
    {
        if (x < 0) x = pixelWidth / 2;
        if (y < 0) y = pixelHeight / 2;
        
        real xModifier = (xMax - xMin) / 2;
        real yModifier = (yMax - yMin) / 2;
        
        real xMinPercentage = static_cast<real>(x) / pixelWidth;
        real xMaxPercentage = 1 - xMinPercentage;
        
        real yMaxPercentage = static_cast<real>(y) / pixelHeight;
        real yMinPercentage = 1 - yMaxPercentage;
        
        //std::cout << "xModifier: " << xMinPercentage << ", " << xMaxPercentage << std::endl;
        //std::cout << "yModifier: " << yMinPercentage << ", " << yMaxPercentage << std::endl;
        
        if (steps < 0)
        {
            xMin -= xModifier * xMaxPercentage;
            xMax += xModifier * xMinPercentage;
            
            yMin -= yModifier * yMaxPercentage;
            yMax += yModifier * yMinPercentage;
        }
        else if (steps > 0)
        {
            xMin += xModifier * xMinPercentage;
            xMax -= xModifier * xMaxPercentage;
            
            yMin += yModifier * yMinPercentage;
            yMax -= yModifier * yMaxPercentage;
        }
    }
}

Point<int> Plotter::getOrigo() const
{
    real diffX = xMax - xMin;
    real diffY = yMax - yMin;
    
	return Point<int>(static_cast<int>(-xMin * pixelWidth / diffX), static_cast<int>(yMax * pixelHeight / diffY));
}

std::vector<std::pair<int, std::string>> Plotter::getXMarkers() const
{
    std::vector<std::pair<int, std::string>> markers;
    
    real step = pixelMarkerGap * (xMax - xMin) / pixelWidth;
    step = real_functions::round(step, static_cast<int>(real_functions::floor(real_functions::log10(step))));
    
    real start = std::ceil(xMin / step) * step;
    
    real pixelRatio = pixelWidth / (xMax - xMin);
    for (real x = start; x <= xMax; x += step)
    {
        if (x != 0) markers.emplace_back(static_cast<int>((x - xMin) * pixelRatio), real_functions::toString(x));
    }
    
    return markers;
}

std::vector<std::pair<int, std::string>> Plotter::getYMarkers() const
{
    std::vector<std::pair<int, std::string>> markers;
    
    real step = pixelMarkerGap * (yMax - yMin) / pixelHeight;
    step = real_functions::round(step, static_cast<int>(real_functions::floor(real_functions::log10(step))));
    
    real start = std::ceil(yMin / step) * step;
    
    real yDiff = yMax - yMin;
    for (real y = start; y <= yMax; y += step)
    {
        if (y != 0) markers.emplace_back(static_cast<int>(pixelHeight * (1 - (y - yMin) / yDiff)), real_functions::toString(y));
    }
    
    return markers;
}

Plotter::size_type Plotter::numExpressions() const
{
    return expressions.size();
}

std::vector<Point<int>> Plotter::getPlotSamples(Plotter::size_type expressionIndex) const
{
    std::vector<Point<int>> result;
    
    const Expression &currentExpression = expressions[expressionIndex];
    
    real diffX = xMax - xMin;
    real diffY = yMax - yMin;
    real step = diffX * samplingRate / pixelWidth;
    real y;
    for (real x = xMin; x < xMax + step; x += step)
    {
        Expression::setVariable("x", x);
        y = currentExpression.evaluate();
        
		result.emplace_back(static_cast<int>(pixelWidth * (x - xMin) / diffX), static_cast<int>(pixelHeight * (1 - (y - yMin) / diffY)));
    }
    
    return result;
}

std::pair<Point<int>, Point<std::string>> Plotter::getNearestPoint(int x, int y) const
{ // do nearest point instead pythagoras
	Expression::setVariable("x", yPxToPt(x));
	
	int nearestValue = std::numeric_limits<int>::min();
	size_t nearestIndex = 0;
	for (size_type i = 0; i != expressions.size(); ++i)
	{
		int currentValue = yPtToPx(expressions[i].evaluate());

		if (std::abs(y - currentValue) < std::abs(y - nearestValue))
		{
			nearestValue = currentValue;
			nearestIndex = i;
		}
	}

	return std::pair<Point<int>, Point<std::string>>(
		Point<int>(x, nearestValue),
		Point<std::string>(
			real_functions::toString(yPxToPt(x)), // has already been calculated
			real_functions::toString(0))
		);
}

Plotter::const_iterator Plotter::cbegin() const
{
	return expressions.cbegin();
}

Plotter::const_iterator Plotter::cend() const
{
	return expressions.cend();
}
