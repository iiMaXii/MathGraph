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
#include <QIcon>

RenderArea::RenderArea(QWidget * parent) :
    QWidget(parent),
	graphTool(MOVE),
    initialPosition(-1, -1),
    currentPosition(-1, -1),
    leftDrag(false),
    plotter(),
    invalidSelectionErrorDialog(parent)
{
	Expression::addFunction("sin", real_functions::sin);
    Expression::addFunction("cos", real_functions::cos);
    Expression::addFunction("tan", real_functions::tan);
    
    setCursor(Qt::OpenHandCursor);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    
    invalidSelectionErrorDialog.setIcon(QMessageBox::Warning);
    invalidSelectionErrorDialog.setText("u haz no functionz se1ect3d1!!!11! n00b");
    
    // Cursors
    zoomPlusCursor = QCursor(QIcon(":images/cursor_zoom_plus.gif").pixmap(16, 16));
    zoomMinusCursor = QCursor(QIcon(":images/cursor_zoom_minus.gif").pixmap(16, 16));
    cropCursor = QCursor(QIcon(":images/cursor_crop.gif").pixmap(16, 16));
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 300);
}

Plotter::size_type RenderArea::addExpression(const Expression &expr)
{
    plotter.addExpression(expr);
    
    rebuildFunctionCache();
    update();
    
    return plotter.numExpressions() - 1;
}

void RenderArea::centerOrigo()
{
    plotter.centerOrigo();
    
    selectedCoordinateString.clear();
    
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
            selectedCoordinateString.clear();
            break;
        case SELECTION:
            setCursor(Qt::CrossCursor);
            selectedCoordinateString.clear();
            break;
        case ZOOM:
            setCursor(zoomPlusCursor);
            break;
    }
}

void RenderArea::clearSelection()
{
    plotter.clearSelection();
    
    for (auto it = functionCache.begin(); it != functionCache.cend(); ++it)
    {
        it->first = false;
    }
    
    update();
}

void RenderArea::select(Plotter::size_type expressionIndex)
{
    plotter.select(expressionIndex);
    
    functionCache[expressionIndex].first = true;
    
    update();
}

void RenderArea::setEnabled(Plotter::size_type expressionIndex, bool enabled)
{
    if (plotter.isHidden(expressionIndex) == enabled)
    { // Change!
        
        plotter.setHidden(expressionIndex, !enabled);
        
        if (enabled)
        {
            rebuildFunctionCache();
        }
        else
        {
            functionCache[expressionIndex].second = QPainterPath();
        }
        
        update();
    }
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen normalPen = QPen(QBrush(Qt::black), 1);
    QPen boldPen = QPen(QBrush(Qt::black), 3);
	painter.setPen(normalPen);

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
    for (size_type i = 0; i != functionCache.size(); ++i)
	{
        if (!functionCache[i].second.isEmpty())
        {
            if (functionCache[i].first)
            {
                boldPen.setColor(FUNCTION_COLOURS[i % FUNCTION_COLOURS.size()]);
                painter.setPen(boldPen);
            }
            else
            {
                normalPen.setColor(FUNCTION_COLOURS[i % FUNCTION_COLOURS.size()]);
                painter.setPen(normalPen);
            }
            
            painter.drawPath(functionCache[i].second);
            
            painter.setPen(normalPen);
        }
	}
    
    normalPen.setColor(Qt::black);
    painter.setPen(normalPen);
    
    // Draw tools
    if (leftDrag && graphTool == ZOOM && !ignoreZoomBox(initialPosition, currentPosition))
    {
        painter.setPen(Qt::DashLine);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(QRect(initialPosition, currentPosition));
    }
    
    if (graphTool == SELECTION && !selectedCoordinateString.isEmpty())
    {
        painter.setBrush(Qt::black);
        //painter.drawEllipse(initialPosition, 5, 5);
        painter.drawLine(initialPosition.x()+2, initialPosition.y(), initialPosition.x()+5, initialPosition.y());
        painter.drawLine(initialPosition.x(), initialPosition.y()+2, initialPosition.x(), initialPosition.y()+5);
        painter.drawLine(initialPosition.x(), initialPosition.y()-2, initialPosition.x(), initialPosition.y()-5);
        painter.drawLine(initialPosition.x()-2, initialPosition.y(), initialPosition.x()-5, initialPosition.y());
        //painter.drawEllipse(QRect(initialPosition.x()-3, initialPosition.y()-3, 4, 4));
        painter.setBrush(Qt::NoBrush);
        //painter.drawLine(initialPosition, currentPosition);
        painter.drawText(currentPosition, selectedCoordinateString);
    }
    
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void RenderArea::keyPressEvent(QKeyEvent * event)
{
    if (graphTool == ZOOM && event->key() & Qt::Key_Alt && ignoreZoomBox(initialPosition, currentPosition))
    {
        setCursor(zoomMinusCursor);
    }
}

void RenderArea::keyReleaseEvent(QKeyEvent * event)
{
    if (graphTool == ZOOM && event->key() & Qt::Key_Alt && ignoreZoomBox(initialPosition, currentPosition))
    {
        setCursor(zoomPlusCursor);
    }
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
				initialPosition = event->pos();
                break;
            case ZOOM:
                currentPosition = initialPosition = event->pos();
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
            {
                try
                {
                    std::pair<Point<int>, Point<std::string>> selectedPoint = plotter.getPointFromSelected(event->x());
                    selectedCoordinateString = "(";
                    selectedCoordinateString += selectedPoint.second.getX().c_str();
                    selectedCoordinateString += ", ";
                    selectedCoordinateString += selectedPoint.second.getY().c_str();
                    selectedCoordinateString += ")";
                    
                    initialPosition = QPoint(selectedPoint.first.getX(), selectedPoint.first.getY());
                    currentPosition = event->pos();
                }
                catch (InvalidSelection &)
                {
                    selectedCoordinateString.clear();
                    
                    invalidSelectionErrorDialog.exec();
                }
            }
                break;
            case ZOOM:
                if (ignoreZoomBox(initialPosition, currentPosition))
                {
                    if (event->modifiers() & Qt::AltModifier)
                    { // Zoom out
                        plotter.zoom(-1, event->x(), event->y());
                    }
                    else
                    { // Zoom in
                        plotter.zoom(1, event->x(), event->y());
                    }
                }
                else
                {
                    plotter.setBounds(initialPosition.x(), currentPosition.x(), initialPosition.y(), currentPosition.y());
                    
                    // Reset cursor
                    if (event->modifiers() & Qt::AltModifier)
                        setCursor(zoomMinusCursor);
                    else
                        setCursor(zoomPlusCursor);
                    
                    // Reset position
                    initialPosition = currentPosition = event->pos();
                }
                
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
                
                if (ignoreZoomBox(initialPosition, currentPosition))
                { // Normal zoom
                    if (event->modifiers() & Qt::AltModifier)
                        setCursor(zoomMinusCursor);
                    else
                        setCursor(zoomPlusCursor);
                }
                else
                { // Box zoom
                    setCursor(cropCursor);
                }
                
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
        
        selectedCoordinateString.clear();
        
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
    
    if (xDiff != 0 || yDiff != 0)
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
    
    for (Plotter::size_type i = 0; i < plotter.numExpressions(); ++i)
    {
        if (plotter.isHidden(i))
        {
            functionCache.emplace_back(false, QPainterPath());
        }
        else
        {
            std::vector<Point<int>> s = plotter.getPlotSamples(i);

            QPainterPath functionPath;
            
            if (!s.empty())
            {
                Point<int> firstPoint = s.front();
                functionPath.moveTo(firstPoint.getX(), firstPoint.getY());
                
                for (const Point<int> &expr : s)
                {
                    functionPath.lineTo(expr.getX(), expr.getY());
                }
            }
            
            functionCache.emplace_back(plotter.isSelected(i), functionPath);
        }
    }
}

bool RenderArea::ignoreZoomBox(const QPoint &begin, const QPoint &end)
{
    return std::abs(end.x() - begin.x()) + std::abs(end.y() - begin.y()) <= IGNORE_ZOOM_BOX; // Manhattan length as in QPoint::manhattanLength()
}
