//
//  RenderArea.h
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#ifndef __MathGraph__RenderArea__
#define __MathGraph__RenderArea__

#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <vector>
#include "Plotter.h"

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
    RenderArea(QWidget * parent = nullptr);
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
    void addExpression(const Expression &expr);
    void centerOrigo();
	void setTool(GraphTool _graphTool);
    
    void clearSelection();
    void select(Plotter::size_type expressionIndex);
protected:
    void paintEvent(QPaintEvent * event);
    
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void wheelEvent (QWheelEvent * event);
    
    void resizeEvent(QResizeEvent * event);
private:
    void move(const QPoint &newPosition);
    void rebuildFunctionCache();
    
    const int IGNORE_ZOOM_AREA = 4; // No box zoom if area is less or equal
    
    QCursor zoomPlusCursor;
    QCursor zoomMinusCursor;
    QCursor cropCursor;
    
	GraphTool graphTool;
    QPoint initialPosition; // Move and zoom start point
    QPoint currentPosition; // Zoom end point
    bool leftDrag;
    
    Plotter plotter;
    std::vector<std::pair<bool, QPainterPath>> functionCache;
};

#endif /* defined(__MathGraph__RenderArea__) */
