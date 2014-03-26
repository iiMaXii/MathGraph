//
//  MainWindow.h
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#ifndef __MathGraph__MainWindow__
#define __MathGraph__MainWindow__

#include "RenderArea.h"
#include "HelpWindow.h"

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>

class MainWindow
    : public QWidget
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
    
    void expressionChanged(QListWidgetItem *item);
    void expressionSelectionChanged();
    
    void displayAboutWindow();
    void displayHelpWindow();
    
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    
    void closeEvent(QCloseEvent *event);
    
private:
    QListWidget *functionList;
    RenderArea *renderArea;
    QLineEdit *expressionLineEdit;
    
    HelpWindow *helpWindow;
};

#endif /* defined(__MathGraph__MainWindow__) */
