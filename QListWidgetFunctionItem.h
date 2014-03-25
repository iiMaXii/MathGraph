//
//  QListWidgetFunctionItem.h
//  MathGraph
//
//  Created by Max Ekström on 2014-03-18.
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
    QListWidgetFunctionItem(Plotter::size_type plotterItemIndex, const QColor &color, const QString & text, QListWidget *parent = nullptr);
    
    Plotter::size_type getPlotterIndex() const;
    QColor getColor() const;
};

#endif /* defined(__MathGraph__QListWidgetFunctionItem__) */
