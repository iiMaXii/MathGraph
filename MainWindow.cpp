//
//  MainWindow.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#include <iostream>

#include "MainWindow.h"
#include "Expression.h"
#include "QListWidgetFunctionItem.h"

#include <QCursor>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>


MainWindow::MainWindow()
    : QWidget()
{
    setMinimumSize(400, 300);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    // Toolbar
    QHBoxLayout *toolbar = new QHBoxLayout;

    // Toolbar: Move tool button
    QPushButton *moveToolButton = new QPushButton(QIcon(), "");
    moveToolButton->setToolTip("Move tool");
    moveToolButton->setIcon(QIcon(":images/cursor_open_hand.gif"));
    moveToolButton->setIconSize(QSize(32, 32));
    
    moveToolButton->setMinimumWidth(50);
    moveToolButton->setMaximumWidth(50);
    moveToolButton->setMinimumHeight(50);
    moveToolButton->setMaximumHeight(50);
    
    toolbar->addWidget(moveToolButton);

    // Toolbar: Zoom tool button
    QPushButton *zoomToolButton = new QPushButton();
    zoomToolButton->setToolTip("Zoom tool");
    zoomToolButton->setIcon(QIcon(":images/cursor_zoom_plus.gif"));

    zoomToolButton->setMinimumWidth(50);
    zoomToolButton->setMaximumWidth(50);
    zoomToolButton->setMinimumHeight(50);
    zoomToolButton->setMaximumHeight(50);
    
    toolbar->addWidget(zoomToolButton);

    // Toolbar: Selection tool button
    QPushButton *selectionToolButton = new QPushButton();
    selectionToolButton->setToolTip("Selection tool");
    selectionToolButton->setIcon(QIcon(":images/cursor_selector.gif"));

    selectionToolButton->setMinimumWidth(50);
    selectionToolButton->setMaximumWidth(50);
    selectionToolButton->setMinimumHeight(50);
    selectionToolButton->setMaximumHeight(50);
    
    toolbar->addWidget(selectionToolButton);

    toolbar->addSpacing(20);

    // Toolbar: Center origo button
    QPushButton *centerOrigioButton = new QPushButton("Center origo");
    
    toolbar->addWidget(centerOrigioButton);
    toolbar->addStretch();
    
    mainLayout->addLayout(toolbar);
    
    // Output area
    QSplitter *outputArea = new QSplitter();
    
    // Output area: Expression list
    expressionList = new QListWidget;
    outputArea->addWidget(expressionList);
    
    // Output area: Render area
    renderArea = new RenderArea;
    outputArea->addWidget(renderArea);
    
    outputArea->setStretchFactor(0, 1);
    outputArea->setStretchFactor(1, 3);
    
    mainLayout->addWidget(outputArea);
    
    // Input area
    expressionLineEdit = new QLineEdit;
    QPushButton *addExpressionButton = new QPushButton("Add");
    
    QHBoxLayout *expressionInput = new QHBoxLayout;
    expressionInput->addWidget(expressionLineEdit);
    expressionInput->addWidget(addExpressionButton);
    
    mainLayout->addLayout(expressionInput);
    
    // Main layout
    setLayout(mainLayout);
    
    // Set focus
    expressionLineEdit->setFocus();
    
    // Events
    connect(addExpressionButton, SIGNAL(clicked()), this, SLOT(addExpression()));
    connect(expressionLineEdit, SIGNAL(returnPressed()), addExpressionButton, SIGNAL(clicked()));
    
    // Toolbar
    connect(centerOrigioButton, SIGNAL(clicked()), this, SLOT(centerOrigo()));
    connect(moveToolButton, SIGNAL(clicked()), this, SLOT(setMoveTool()));
    connect(zoomToolButton, SIGNAL(clicked()), this, SLOT(setZoomTool()));
    connect(selectionToolButton, SIGNAL(clicked()), this, SLOT(setSelectionTool()));
    
    // Expression list
    connect(expressionList, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(expressionChanged(QListWidgetItem *))); // used to check for checked items
    connect(expressionList, SIGNAL(itemSelectionChanged()), this, SLOT(expressionSelectionChanged()));
}

QSize MainWindow::sizeHint() const
{
    return QSize(800, 600);
}

void MainWindow::addExpression()
{
    std::string expressionString = expressionLineEdit->text().toStdString();
    
    try
    {
        Expression expr(expressionString);
        
        expressionLineEdit->setStyleSheet("");
        
        Plotter::size_type plotterItemIndex = renderArea->addExpression(expr);
        
        QListWidgetItem *item = new QListWidgetFunctionItem(plotterItemIndex, renderArea->FUNCTION_COLORS[plotterItemIndex % renderArea->FUNCTION_COLORS.size()], expressionString.c_str());
        
        expressionList->addItem(item);
        
        expressionLineEdit->setText("");
    }
    catch(InvalidExpression &e)
    {
        expressionLineEdit->setStyleSheet("background: #FF3333;");
    }
}

void MainWindow::centerOrigo()
{
    renderArea->centerOrigo();
}

void MainWindow::setMoveTool()
{
    renderArea->setTool(MOVE);
}

void MainWindow::setSelectionTool()
{
    renderArea->setTool(SELECTION);
}

void MainWindow::setZoomTool()
{
    renderArea->setTool(ZOOM);
}

void MainWindow::expressionChanged(QListWidgetItem *item)
{
    QListWidgetFunctionItem *functionItem = static_cast<QListWidgetFunctionItem *>(item);
    
    renderArea->setEnabled(functionItem->getPlotterIndex(), functionItem->checkState() == Qt::Checked);
}

void MainWindow::expressionSelectionChanged()
{
    renderArea->clearSelection();
    
    for (int i = 0; i < expressionList->count(); ++i)
    {
        if (expressionList->item(i)->isSelected()) renderArea->select(i);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    renderArea->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    renderArea->keyReleaseEvent(event);
}
