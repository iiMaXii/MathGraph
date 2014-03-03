//
//  MainWindow.h
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#ifndef __MathGraph__MainWindow__
#define __MathGraph__MainWindow__

#include "RenderArea.h"
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    MainWindow();
    
    QSize sizeHint() const;
public slots:
    void addExpression();
    void centerOrigo();
private:
    QListWidget * expressionList;
    RenderArea * renderArea;
    QLineEdit * expressionLineEdit;
};

#endif /* defined(__MathGraph__MainWindow__) */
