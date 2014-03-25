//
//  QListWidgetFunctionItem.h
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#ifndef __MathGraph__QListWidgetFunctionItem__
#define __MathGraph__QListWidgetFunctionItem__

#include "Plotter.h"

#include <QListWidgetItem>

class QListWidgetFunctionItem : public QListWidgetItem
{
    Plotter::size_type plotterItemIndex;
    QColor color;
    
public:
    QListWidgetFunctionItem(Plotter::size_type _plotterItemIndex, const QColor &_color, const QString &_text, QListWidget *_parent = nullptr);
    
    Plotter::size_type getPlotterIndex() const;
    QColor getColor() const;
    
    void setPlotterIndex(Plotter::size_type _plotterItemIndex);
    void setColor(const QColor &_color);
};

#endif /* defined(__MathGraph__QListWidgetFunctionItem__) */
