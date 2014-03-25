//
//  HelpWindow.cpp
//  MathGraph
//
//  Created by Max Ekström on 2014-03-25.
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
