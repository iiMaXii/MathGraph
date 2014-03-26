//
//  HelpWindow.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#include "HelpWindow.h"

#include <QVBoxLayout>
#include <QPushButton>

HelpWindow::HelpWindow()
    : QWidget()
{
    setMinimumSize(600, 400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    webView = new QWebView();
    
    webView->setUrl(QUrl("qrc:/doc/index.html"));
    
    mainLayout->addWidget(webView);
    
    setLayout(mainLayout);
}

QSize HelpWindow::sizeHint() const
{
    return QSize(800, 600);
}
