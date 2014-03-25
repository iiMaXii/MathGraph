//
//  RenderArea.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#include "renderarea.h"
#include "real.h"

#include <QIcon>

RenderArea::RenderArea(QWidget *_parent)
    : QWidget(_parent),
      graphTool(MOVE),
      initialPosition(-1, -1),
      currentPosition(-1, -1),
      leftDrag(false),
      selectedCoordinateString(),
      plotter(),
      functionCache(),
      selectedFunction(npos),
      invalidSelectionErrorDialog(_parent)
{
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

Plotter::size_type RenderArea::addFunction(const Expression &expr)
{
    plotter.addExpression(expr);
    
    rebuildFunctionCache();
    update();
    
    return plotter.numExpressions() - 1;
}

Plotter::size_type RenderArea::removeSelectedFunction()
{
    Plotter::size_type removedIndex = selectedFunction;
    
    if (selectedFunction != npos)
    {
        plotter.removeExpression(selectedFunction);
        
        selectedFunction = npos;
        
        rebuildFunctionCache();
        update();
    }
    
    return removedIndex;
}

void RenderArea::centerOrigo()
{
    removeCurveSelection();
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
            removeCurveSelection();
            break;
        case SELECTION:
            setCursor(Qt::CrossCursor);
            removeCurveSelection();
            break;
        case ZOOM:
            setCursor(zoomPlusCursor);
            break;
    }
}

void RenderArea::clearSelection()
{
    plotter.clearSelection();
    selectedFunction = npos;
    
    update();
}

void RenderArea::select(Plotter::size_type expressionIndex)
{
    plotter.select(expressionIndex);
    selectedFunction = expressionIndex;
    
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
            functionCache[expressionIndex] = QPainterPath();
        }
        
        update();
    }
}

void RenderArea::keyPressEvent(QKeyEvent *event)
{
    if (graphTool == ZOOM && event->key() & Qt::Key_Alt && ignoreZoomBox(initialPosition, currentPosition))
    {
        setCursor(zoomMinusCursor);
    }
}

void RenderArea::keyReleaseEvent(QKeyEvent *event)
{
    if (graphTool == ZOOM && event->key() & Qt::Key_Alt && ignoreZoomBox(initialPosition, currentPosition))
    {
        setCursor(zoomPlusCursor);
    }
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFontMetrics currentFontMetrics = fontMetrics();
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
        
        painter.drawText(origo.getX() + 8, currentPosition + 5, it->second.c_str());
        painter.drawLine(origo.getX() - 3, currentPosition, origo.getX() + 3, currentPosition);
    }
    
    // Draw functions
    for (size_type i = 0; i != functionCache.size(); ++i)
    {
        if (!functionCache[i].isEmpty())
        {
            if (i == selectedFunction)
            {
                boldPen.setColor(FUNCTION_COLORS[i % FUNCTION_COLORS.size()]);
                painter.setPen(boldPen);
            }
            else
            {
                normalPen.setColor(FUNCTION_COLORS[i % FUNCTION_COLORS.size()]);
                painter.setPen(normalPen);
            }
            
            painter.drawPath(functionCache[i]);
            
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
        
        painter.drawLine(initialPosition.x()+2, initialPosition.y(), initialPosition.x()+5, initialPosition.y());
        painter.drawLine(initialPosition.x(), initialPosition.y()+2, initialPosition.x(), initialPosition.y()+5);
        painter.drawLine(initialPosition.x(), initialPosition.y()-2, initialPosition.x(), initialPosition.y()-5);
        painter.drawLine(initialPosition.x()-2, initialPosition.y(), initialPosition.x()-5, initialPosition.y());
        
        painter.setBrush(Qt::NoBrush);
        
        painter.drawText(width() - currentFontMetrics.width(selectedCoordinateString) - 3, height() - 5, selectedCoordinateString);
    }
    
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void RenderArea::mousePressEvent(QMouseEvent *event)
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

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
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
                doCurveSelection(event->pos());
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
                    int startX, endX;
                    int startY, endY;
                    
                    if (initialPosition.x() < currentPosition.x())
                    {
                        startX = initialPosition.x();
                        endX = currentPosition.x();
                    }
                    else
                    {
                        startX = currentPosition.x();
                        endX = initialPosition.x();
                    }
                    
                    if (initialPosition.y() < currentPosition.y())
                    {
                        startY = initialPosition.y();
                        endY = currentPosition.y();
                    }
                    else
                    {
                        startY = currentPosition.y();
                        endY = initialPosition.y();
                    }
                    
                    plotter.setBounds(startX, endX, startY, endY);
                    
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

void RenderArea::mouseMoveEvent(QMouseEvent *event)
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
            doCurveSelection(event->pos());
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

void RenderArea::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    
    if(!numPixels.isNull())
    {
        plotter.zoom(numPixels.y(), event->x(), event->y());
        removeCurveSelection();
        
        rebuildFunctionCache();
        update();
    }
}

void RenderArea::resizeEvent(QResizeEvent *event)
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

void RenderArea::doCurveSelection(const QPoint &pos)
{
    try
    {
        std::pair<Point<int>, Point<std::string>> selectedPoint = plotter.getPointFromSelected(pos.x());
        selectedCoordinateString = "(";
        selectedCoordinateString += selectedPoint.second.getX().c_str();
        selectedCoordinateString += ", ";
        selectedCoordinateString += selectedPoint.second.getY().c_str();
        selectedCoordinateString += ")";
        
        initialPosition = QPoint(selectedPoint.first.getX(), selectedPoint.first.getY());
        currentPosition = pos;
        
        update();
    }
    catch (const InvalidSelection &)
    {
        removeCurveSelection();
        
        invalidSelectionErrorDialog.exec();
    }
}

void RenderArea::removeCurveSelection()
{
    selectedCoordinateString.clear();
}

void RenderArea::rebuildFunctionCache()
{
    functionCache.clear();
    
    for (Plotter::size_type i = 0; i != plotter.numExpressions(); ++i)
    {
        if (plotter.isHidden(i))
        {
            functionCache.emplace_back();
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
            
            functionCache.emplace_back(functionPath);
        }
    }
}

bool RenderArea::ignoreZoomBox(const QPoint &begin, const QPoint &end)
{
    return (end - begin).manhattanLength() <= IGNORE_ZOOM_BOX;
}
