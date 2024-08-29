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
#include <QFrame>

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
    bool doesUrlExist(const QUrl &url);
    void urlChange(const QUrl &url);

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
};


#endif // SEATAB_H
