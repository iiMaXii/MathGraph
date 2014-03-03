//
//  RenderArea.cpp
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#include "renderarea.h"
#include <iostream>
#include <cmath>

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    currentPosition(0, 0),
    leftDrag(false),
    plotter()
{
    Expression::addFunction("sin", std::sin);
    Expression::addFunction("cos", std::cos);
    
    setCursor(Qt::OpenHandCursor);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 300);
}

Plotter &RenderArea::getPlotter()
{
    return plotter;
}


void RenderArea::addExpression(const Expression &expr)
{
    plotter.addExpression(expr);
    
    rebuildFunctionCache();
    update();
}

void RenderArea::centerOrigo()
{
    plotter.centerOrigo();
    
    rebuildFunctionCache();
    update();
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    
    //painter.drawEllipse(currentPosition, 10, 10);
    
    // x-axis
    //painter.fillPath(xAxisArrow, QBrush(Qt::black));
    Point<int> origo = plotter.getOrigo();
    
    QPainterPath xAxisArrow = QPainterPath(QPoint(0, origo.getY()));
    xAxisArrow.lineTo(QPoint(width(), origo.getY()));
    xAxisArrow.lineTo(QPoint(width() - 8, origo.getY() - 4));
    xAxisArrow.lineTo(QPoint(width() - 8, origo.getY() + 4));
    xAxisArrow.lineTo(QPoint(width(), origo.getY()));
    
    painter.drawPath(xAxisArrow);
    painter.fillPath(xAxisArrow, QBrush(Qt::black));
    
    // y-axis
    QPainterPath yAxisArrow = QPainterPath(QPoint(origo.getX(), height()));
    yAxisArrow.lineTo(QPoint(origo.getX(), 0));
    yAxisArrow.lineTo(QPoint(origo.getX() - 4, 8));
    yAxisArrow.lineTo(QPoint(origo.getX() + 4, 8));
    yAxisArrow.lineTo(QPoint(origo.getX(), 0));
    
    painter.drawPath(yAxisArrow);
    painter.fillPath(yAxisArrow, QBrush(Qt::black));
    
    // X markers
    QFontMetrics currentFontMetrics = fontMetrics();
    
    std::vector<std::pair<int, std::string>> xMarkers = plotter.getXMarkers();
    for (auto it = xMarkers.cbegin(); it != xMarkers.cend(); ++it)
    {
        int currentPosition = it->first;
        
        QString value = it->second.c_str();
        
        painter.drawText(currentPosition - currentFontMetrics.width(value) / 2, origo.getY() + 20, value);
        painter.drawLine(currentPosition, origo.getY() - 3, currentPosition, origo.getY() + 3);
    }
    
    // Y markers
    std::vector<std::pair<int, std::string>> yMarkers = plotter.getYMarkers();
    for (auto it = yMarkers.cbegin(); it != yMarkers.cend(); ++it)
    {
        int currentPosition = it->first;
        
        painter.drawText(origo.getX() + 8, currentPosition + 5, tr(it->second.c_str()));
        painter.drawLine(origo.getX() - 3, currentPosition, origo.getX() + 3, currentPosition);
    }
    
    
    for (auto it = functionCache.cbegin(); it != functionCache.cend(); ++it)
        painter.drawPath(*it);
    
    //if (leftDrag)
    //{
    //    painter.drawEllipse(QPoint(10, 10), 5, 5);
    //}
    
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void RenderArea::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        currentPosition = event->pos();
        
        setCursor(Qt::ClosedHandCursor);
        leftDrag = true;
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && leftDrag)
    {
        QPoint newPosition = event->pos();
        move(newPosition);
        
        setCursor(Qt::OpenHandCursor);
        leftDrag = false;
        update();
    }
}

void RenderArea::mouseMoveEvent(QMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton && leftDrag)
    {
        QPoint newPosition = event->pos();
        move(newPosition);
    }
}

void RenderArea::wheelEvent(QWheelEvent * event)
{
    QPoint numPixels = event->pixelDelta();
    
    if(!numPixels.isNull())
    {
        plotter.zoom(numPixels.y(), event->x(), event->y());
        
        rebuildFunctionCache();
        update();
    }
}

void RenderArea::resizeEvent(QResizeEvent * event)
{
    QSize oldSize = event->oldSize();
    QSize newSize = event->size();
    
    plotter.resize(newSize.width(), newSize.height());
    rebuildFunctionCache();
    
    std::cout << "Resize: (" << oldSize.width() << ", " << oldSize.width() << ") -> ("
    << newSize.height() << ", " << newSize.height() << ")" << std::endl;
    
    update();
}

void RenderArea::move(const QPoint &newPosition)
{
    int xDiff = currentPosition.x() - newPosition.x();
    int yDiff = currentPosition.y() - newPosition.y();
    
    if (xDiff != 0 && yDiff != 0)
    {
        plotter.move(xDiff, yDiff);
        rebuildFunctionCache();
        
        currentPosition = newPosition;
        
        update();
    }
}

void RenderArea::rebuildFunctionCache()
{
    functionCache.clear();
    
    //std::cout << "Num functions: " << plotter.numExpressions() << std::endl;
    
    Point<int> origo = plotter.getOrigo();
    
    for (Plotter::size_type i = 0; i < plotter.numExpressions(); ++i)
    {
        std::vector<Point<int>> s = plotter.getPlotSamples(i);
        
        QPainterPath functionPath(QPoint(0, origo.getY()));
        
        for (auto it = s.cbegin(); it != s.cend(); ++it)
        {
            //std::cout << "Adding point: (" << it->getX() << ", " << it->getY() << ")" << std::endl;
            functionPath.lineTo(it->getX(), it->getY());
        }
        
        functionCache.push_back(functionPath);
    }
}
