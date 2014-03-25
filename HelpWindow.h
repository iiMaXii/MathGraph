//
//  HelpWindow.h
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#ifndef __MathGraph__HelpWindow__
#define __MathGraph__HelpWindow__

#include <QWidget>
#include <QWebView>

class HelpWindow
    : public QWidget
{
    Q_OBJECT
    
public:
    HelpWindow();
    QSize sizeHint() const;
    
private:
    QWebView *webView;
};

#endif /* defined(__MathGraph__HelpWindow__) */
