// customwebpage.h
#ifndef CUSTOMWEBPAGE_H
#define CUSTOMWEBPAGE_H

#include <QWebEnginePage>
#include <QUrl>

class CustomWebPage : public QWebEnginePage
{
    Q_OBJECT

public:
    explicit CustomWebPage(QObject *parent = nullptr);

protected:
    QWebEnginePage *createWindow(WebWindowType type) override;

signals:
    void newTabRequested(const QUrl &url);
};

#endif // CUSTOMWEBPAGE_H

