//
//  QListWidgetFunctionItem.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#include "QListWidgetFunctionItem.h"

QListWidgetFunctionItem::QListWidgetFunctionItem(Plotter::size_type _plotterItemIndex, const QColor &_color, const QString &_text, QListWidget *_parent)
    : QListWidgetItem(_text, _parent),
      plotterItemIndex(_plotterItemIndex),
      color(_color)
{
    setTextColor(_color);
    setFlags(flags() | Qt::ItemIsUserCheckable);
    setCheckState(Qt::Checked);
}

Plotter::size_type QListWidgetFunctionItem::getPlotterIndex() const
{
    return plotterItemIndex;
}

QColor QListWidgetFunctionItem::getColor() const
{
    return color;
}

void QListWidgetFunctionItem::setPlotterIndex(Plotter::size_type _plotterItemIndex)
{
    plotterItemIndex = _plotterItemIndex;
}

void QListWidgetFunctionItem::setColor(const QColor &_color)
{
    color = _color;
    setTextColor(color);
}