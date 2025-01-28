#ifndef SEATAB_H
#define SEATAB_H

#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QUrl>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEnginePage>
#include <QTimer>
#include <QTabBar>
#include <QMouseEvent>
#include <QMovie>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>
#include "browserCache.h"
#include <QWebEngineUrlRequestInfo>
#include <QFrame>
#include <QFontDatabase>

class seaTab : public QWidget {

    Q_OBJECT

public:
    seaTab(QWidget *parent = nullptr);
    ~seaTab();
    QWebEngineView* getWebView() const;

private slots:
    void processURL();
    void openDevTools();
    void onBackClicked();
    void onRefreshClicked();
    void onForwardClicked();
    void handleLoadFinished(bool ok);
    bool doesUrlExist(const QUrl &url);
    void urlChange(const QUrl &url);
    void onNewTabRequested(const QUrl &url);
    void interceptRequest(QWebEngineUrlRequestInfo &info);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWebEngineView *webView;
    QNetworkAccessManager *networkManager;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *refreshButton;
    QLineEdit *urlSearch;
    QPushButton *options;
    BrowserCache *cache;
    void setupCache();
    void loadFromCache(const QUrl &url);
    void cacheResponse(const QUrl &url, const QByteArray &data,
                       const QString &contentType);

signals:
    void newTabRequested(const QUrl &url);
};


#endif // SEATAB_H
