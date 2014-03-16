//
//  MainWindow.h
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
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
	void setMoveTool();
	void setSelectionTool();
	void setZoomTool();
    
    void expressionSelectionChanged();
private:
    QListWidget * expressionList;
    RenderArea * renderArea;
    QLineEdit * expressionLineEdit;
};

#endif /* defined(__MathGraph__MainWindow__) */
