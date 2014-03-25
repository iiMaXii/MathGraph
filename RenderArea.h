//
//  RenderArea.h
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#ifndef __MathGraph__RenderArea__
#define __MathGraph__RenderArea__

#include "Plotter.h"

#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMessageBox>

#include <vector>
#include <array>

enum GraphTool
{
    MOVE,
    SELECTION,
    ZOOM
};

class RenderArea : public QWidget
{
    Q_OBJECT
    
public:
    const std::array<QColor, 10> FUNCTION_COLORS = {{Qt::blue, Qt::red, Qt::green, Qt::cyan, Qt::magenta, Qt::black, Qt::darkMagenta, Qt::darkYellow, Qt::gray, Qt::lightGray}};
    
    RenderArea(QWidget *parent = nullptr);
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
    Plotter::size_type addExpression(const Expression &expr);
    void centerOrigo();
    void setTool(GraphTool _graphTool);
    
    void clearSelection();
    void select(Plotter::size_type expressionIndex);
    void setEnabled(Plotter::size_type expressionIndex, bool enabled);
    
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    
protected:
    void paintEvent(QPaintEvent *event);
    
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent (QWheelEvent *event);
    
    void resizeEvent(QResizeEvent *event);
    
private:
    void move(const QPoint &newPosition);
    void rebuildFunctionCache();
    
    const int IGNORE_ZOOM_BOX = 8; // No box zoom if area is less or equal
    bool ignoreZoomBox(const QPoint &begin, const QPoint &end);
    
    QCursor zoomPlusCursor;
    QCursor zoomMinusCursor;
    QCursor cropCursor;
    
    GraphTool graphTool;
    QPoint initialPosition; // Move, zoom start point,
    QPoint currentPosition; // Zoom end point,
    bool leftDrag; // In case dragging started outside widget
    QString selectedCoordinateString;
    
    Plotter plotter;
    std::vector<std::pair<bool, QPainterPath>> functionCache;
    typedef std::vector<std::pair<bool, QPainterPath>>::size_type size_type;
    
    QMessageBox invalidSelectionErrorDialog;
};

#endif /* defined(__MathGraph__RenderArea__) */
