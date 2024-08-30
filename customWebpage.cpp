#include "customwebpage.h"

CustomWebPage::CustomWebPage(QObject *parent)
    : QWebEnginePage(parent)
{
}

QWebEnginePage *CustomWebPage::createWindow(WebWindowType type)
{
    if (type == QWebEnginePage::WebBrowserTab) {
        CustomWebPage *newPage = new CustomWebPage(this);
        connect(newPage, &QWebEnginePage::urlChanged, this, [this, newPage](const QUrl &url) {
            emit newTabRequested(url);
            disconnect(newPage, &QWebEnginePage::urlChanged, this, nullptr);
        });

        return newPage;
    }
    return nullptr;
}
