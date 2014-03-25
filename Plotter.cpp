//
//  Plotter.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#include "Plotter.h"
#include <cmath>

InvalidSelection::InvalidSelection(const std::string &_what_arg)
    : std::logic_error(_what_arg)
{}


Plotter::Plotter(int _pixelWidth, int _pixelHeight, real _xMin, real _xMax, real _yMin, real _yMax, double _samplingRate, int _pixelMarkerGap)
    : pixelWidth(_pixelWidth),
      pixelHeight(_pixelHeight),
      xMin(_xMin),
      xMax(_xMax),
      yMin(_yMin),
      yMax(_yMax),
      samplingRate(_samplingRate),
      pixelMarkerGap(_pixelMarkerGap)
{
    Expression::addVariable("x", 0);
}

Plotter::Plotter(int _pixelWidth, int _pixelHeight) : Plotter(_pixelWidth, _pixelHeight, -10, 10, -10, 10) {}

Plotter::Plotter() : Plotter(0, 0) {}

void Plotter::addExpression(const std::string &expression)
{
    expressions.emplace_back(expression);
    expressionIsHidden.push_back(false);
}

void Plotter::addExpression(const Expression &expression)
{
    expressions.emplace_back(expression);
    expressionIsHidden.push_back(false);
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

void Plotter::setBounds(int xPixelMin, int xPixelMax, int yPixelMin, int yPixelMax)
{
    setBounds(xPxToPt(xPixelMin), xPxToPt(xPixelMax), yPxToPt(yPixelMax), yPxToPt(yPixelMin));
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

void Plotter::setHidden(size_type expressionIndex, bool hidden)
{
    expressionIsHidden[expressionIndex] = hidden;
}

bool Plotter::isHidden(size_type expressionIndex) const
{
    return expressionIsHidden[expressionIndex];
}

void Plotter::select(size_type expressionIndex)
{
    selectedExpression = expressionIndex;
}

void Plotter::clearSelection()
{
    selectedExpression = npos;
}

bool Plotter::isSelected(size_type expressionIndex) const
{
    return expressionIndex == selectedExpression;
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

std::pair<Point<int>, Point<std::string>> Plotter::getPointFromSelected(int x) const
{
    if (selectedExpression == npos)
    {
        throw InvalidSelection("No expression selected");
    }
    else
    {
        real real_x = xPxToPt(x);
        Expression::setVariable("x", real_x);
        
        real real_y = expressions[selectedExpression].evaluate();
        
        int y = yPtToPx(real_y);
        
        std::pair<Point<int>, Point<std::string>> point(
            Point<int>(x, y),
            Point<std::string>(real_functions::toString(real_x),
                               real_functions::toString(real_y))
        );
        
        return point;
    }
}

Plotter::const_iterator Plotter::cbegin() const
{
    return expressions.cbegin();
}

Plotter::const_iterator Plotter::cend() const
{
    return expressions.cend();
}

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
    return x * (xMax - xMin) / pixelWidth + xMin;
}

real Plotter::yPxToPt(int y) const
{
    return (pixelHeight - y) * (yMax - yMin) / pixelHeight + yMin;
}
