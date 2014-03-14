//
//  RenderArea.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#include "renderarea.h"
#include <iostream>
#include <cmath>
#include "real.h"

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
	graphTool(MOVE),
    initialPosition(0, 0),
    currentPosition(0, 0),
    leftDrag(false),
    plotter()
{
	Expression::addFunction("sin", real_functions::sin);
    Expression::addFunction("cos", real_functions::cos);
    Expression::addFunction("tan", real_functions::tan);
    
    
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

void RenderArea::setTool(GraphTool _graphTool)
{
	graphTool = _graphTool;
    
    switch(graphTool)
    {
        case MOVE:
            setCursor(Qt::OpenHandCursor);
            break;
        case SELECTION:
            setCursor(Qt::CrossCursor);
            break;
        case ZOOM:
            setCursor(Qt::ForbiddenCursor);
            break;
    }
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
	//painter.setPen(QPen(QBrush(Qt::black), 4));

    // x-axis
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
    
    // Draw functions
	for (auto it = functionCache.cbegin(); it != functionCache.cend(); ++it)
	{
		//painter.setPen(QPen(QBrush(Qt::black), 4));
		painter.drawPath(*it);
	}
    
    // Draw tools
    if (leftDrag)
    {
        switch (graphTool)
        {
            case ZOOM:
                painter.setPen(Qt::DashLine);
                painter.setBrush(Qt::NoBrush);
                painter.drawRect(QRect(initialPosition, currentPosition));
                break;
        }
    }
    
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void RenderArea::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        initialPosition = event->pos();
		leftDrag = true;

		switch(graphTool)
		{
            case MOVE:
                setCursor(Qt::ClosedHandCursor);
                break;
            case SELECTION:
                //
                break;
            case ZOOM:
                initialPosition = event->pos();
                break;
		}
        
        
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && leftDrag)
    {
        QPoint newPosition = event->pos();
		leftDrag = false;

		switch (graphTool)
		{
            case MOVE:
                move(newPosition);
                setCursor(Qt::OpenHandCursor);
                break;
            case SELECTION:
                //plotter.getPoint()
                break;
            case ZOOM:
                std::cout << "(" << initialPosition.x() << ", " << initialPosition.y() << ")" << std::endl;
                std::cout << "(" << currentPosition.x() << ", " << currentPosition.y() << ")" << std::endl;
                plotter.setBounds(initialPosition.x(), currentPosition.x(), initialPosition.y(), currentPosition.y());
                rebuildFunctionCache();
                break;
		}

        update();
    }
}

void RenderArea::mouseMoveEvent(QMouseEvent * event)
{
    switch(graphTool)
    {
        case MOVE:
            if (event->buttons() & Qt::LeftButton && leftDrag)
            {
                QPoint newPosition = event->pos();
                move(newPosition);
            }
            break;
        case SELECTION:
            // maybe
            break;
        case ZOOM:
            if (event->buttons() & Qt::LeftButton && leftDrag)
            {
                currentPosition = event->pos();
                update();
            }
            break;
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
    QSize newSize = event->size();
    
    plotter.resize(newSize.width(), newSize.height());
    rebuildFunctionCache();
    
    update();
}

void RenderArea::move(const QPoint &newPosition)
{
    int xDiff = initialPosition.x() - newPosition.x();
    int yDiff = initialPosition.y() - newPosition.y();
    
    if (xDiff != 0 && yDiff != 0)
    {
        plotter.move(xDiff, yDiff);
        rebuildFunctionCache();
        
        initialPosition = newPosition;
        
        update();
    }
}

void RenderArea::rebuildFunctionCache()
{
    functionCache.clear();
    
    Point<int> origo = plotter.getOrigo();
    
    for (Plotter::size_type i = 0; i < plotter.numExpressions(); ++i)
    {
        std::vector<Point<int>> s = plotter.getPlotSamples(i);
        
        QPainterPath functionPath(QPoint(0, origo.getY()));
        
        for (auto it = s.cbegin(); it != s.cend(); ++it)
        {
            functionPath.lineTo(it->getX(), it->getY());
        }
        
        functionCache.push_back(functionPath);
    }
}
