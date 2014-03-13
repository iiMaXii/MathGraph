//
//  MainWindow.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
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
: QWidget()
{
    setMinimumSize(400, 300);
    
    QVBoxLayout * mainLayout = new QVBoxLayout;
    
    // Toolbar
	QHBoxLayout * toolbar = new QHBoxLayout;

	// Toolbar: Move tool button
    QPushButton * moveToolButton = new QPushButton(QIcon(), "");
	moveToolButton->setToolTip("Move tool");
	//moveToolButton->setIconSize(QSize(40, 40));
	moveToolButton->setIcon(QIcon("/Users/Max/Desktop/cursor-openhand.png"));
    
    moveToolButton->setMinimumWidth(50);
    moveToolButton->setMaximumWidth(50);
    moveToolButton->setMinimumHeight(50);
    moveToolButton->setMaximumHeight(50);
    
	toolbar->addWidget(moveToolButton);

	// Toolbar: Zoom tool button
    QPushButton * zoomToolButton = new QPushButton();
	zoomToolButton->setToolTip("Zoom tool");
	//zoomToolButton->setIconSize(QSize(32, 32));
	zoomToolButton->setIcon(QIcon(":/images/cursor-zoom-plus.png"));

    zoomToolButton->setMinimumWidth(50);
    zoomToolButton->setMaximumWidth(50);
    zoomToolButton->setMinimumHeight(50);
    zoomToolButton->setMaximumHeight(50);
    
	toolbar->addWidget(zoomToolButton);

	// Toolbar: Selection tool button
    QPushButton * selectionToolButton = new QPushButton();
	selectionToolButton->setToolTip("Selection tool");
	selectionToolButton->setIcon(QIcon("/Users/Max/Desktop/cursor-cross.png"));

    selectionToolButton->setMinimumWidth(50);
    selectionToolButton->setMaximumWidth(50);
    selectionToolButton->setMinimumHeight(50);
    selectionToolButton->setMaximumHeight(50);
    
	toolbar->addWidget(selectionToolButton);

	toolbar->addSpacing(20);

	// Toolbar: Center origo button
    QPushButton * centerOrigioButton = new QPushButton(tr("Center origo"));
    
    toolbar->addWidget(centerOrigioButton);
    toolbar->addStretch();
    
    mainLayout->addLayout(toolbar);
    
    // Output area
    QSplitter * outputArea = new QSplitter();
    
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
    QPushButton * addExpressionButton = new QPushButton("Add");
    
    QHBoxLayout * expressionInput = new QHBoxLayout;
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
    
    connect(centerOrigioButton, SIGNAL(clicked()), this, SLOT(centerOrigo()));
	connect(moveToolButton, SIGNAL(clicked()), this, SLOT(setMoveTool()));
	connect(zoomToolButton, SIGNAL(clicked()), this, SLOT(setZoomTool()));
	connect(selectionToolButton, SIGNAL(clicked()), this, SLOT(setSelectionTool()));
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
