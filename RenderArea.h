//
//  RenderArea.h
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
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
	Move,
	Selection,
	Zoom
};

class RenderArea : public QWidget
{
    Q_OBJECT
    
public:
    RenderArea(QWidget * parent = 0);
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
    Plotter &getPlotter();
    
    void addExpression(const Expression &expr);
    void centerOrigo();
	void setTool(GraphTool _graphTool);
protected:
    void paintEvent(QPaintEvent * event);
    
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void wheelEvent (QWheelEvent * event);
    
    void resizeEvent(QResizeEvent * event);
private:
    void move(const QPoint &newPosition);
    void rebuildFunctionCache();
    
	GraphTool graphTool;
    QPoint currentPosition;
    bool leftDrag;
    
    Plotter plotter;
    std::vector<QPainterPath> functionCache;
};

#endif /* defined(__MathGraph__RenderArea__) */
