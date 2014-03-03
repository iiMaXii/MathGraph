//
//  MainWindow.cpp
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#include "MainWindow.h"
#include "Expression.h"

#include <QCursor>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

MainWindow::MainWindow()
: QWidget::QWidget()
{
    setMinimumSize(400, 300);
    
    QVBoxLayout * mainLayout = new QVBoxLayout;
    
    // Fancy buttons
    QPushButton * moveToolButton = new QPushButton(QIcon("/Users/Max/Desktop/cursor-openhand.png"), "");
    //moveToolButton->setIconSize(QSize(40, 40));
    
    moveToolButton->setMinimumWidth(50);
    moveToolButton->setMaximumWidth(50);
    moveToolButton->setMinimumHeight(50);
    moveToolButton->setMaximumHeight(50);
    
    QPushButton * zoomToolButton = new QPushButton(QIcon("/Users/Max/Desktop/cursor-zoom-plus.png"), "");
    
    zoomToolButton->setMinimumWidth(50);
    zoomToolButton->setMaximumWidth(50);
    zoomToolButton->setMinimumHeight(50);
    zoomToolButton->setMaximumHeight(50);
    
    QPushButton * selectionToolButton = new QPushButton(QIcon("/Users/Max/Desktop/cursor-cross.png"), "");

    selectionToolButton->setMinimumWidth(50);
    selectionToolButton->setMaximumWidth(50);
    selectionToolButton->setMinimumHeight(50);
    selectionToolButton->setMaximumHeight(50);
    
    QPushButton * centerOrigioButton = new QPushButton(tr("Center origo"));
    
    QHBoxLayout * toolbar = new QHBoxLayout;
    
    //toolbar->SetMinimumSize(32, 32);
    
    toolbar->addWidget(moveToolButton);
    toolbar->addWidget(zoomToolButton);
    toolbar->addWidget(selectionToolButton);
    toolbar->addSpacing(20);
    toolbar->addWidget(centerOrigioButton);
    toolbar->addStretch();
    
    mainLayout->addLayout(toolbar);
    
    //Output area
    QSplitter * splitter = new QSplitter();
    
    // Expression list
    expressionList = new QListWidget;
    splitter->addWidget(expressionList);
    
    // Render area
    renderArea = new RenderArea;
    splitter->addWidget(renderArea);
    
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    
    mainLayout->addWidget(splitter);
    
    // Input area
    expressionLineEdit = new QLineEdit;
    expressionLineEdit->setFocus();
    QPushButton * addExpressionButton = new QPushButton(tr("Add"));
    
    // Main layout
    QHBoxLayout * expressionInput = new QHBoxLayout;
    expressionInput->addWidget(expressionLineEdit);
    expressionInput->addWidget(addExpressionButton);
    
    mainLayout->addLayout(expressionInput);
    
    setLayout(mainLayout);
    
    // Events
    connect(addExpressionButton, SIGNAL(clicked()), this, SLOT(addExpression()));
    connect(expressionLineEdit, SIGNAL(returnPressed()), addExpressionButton, SIGNAL(clicked()));
    
    
    connect(centerOrigioButton, SIGNAL(clicked()), this, SLOT(centerOrigo()));
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
        
        renderArea->addExpression(expr);
        
        QListWidgetItem * item = new QListWidgetItem(QString(expressionString.c_str()), expressionList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
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
