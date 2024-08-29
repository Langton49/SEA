#include "seaTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

seaTab::seaTab(QWidget *parent)
    : QWidget(parent),
    webView(new QWebEngineView(this)),
    networkManager(new QNetworkAccessManager(this))
{
    QFrame *urlBarFrame = new QFrame(this);
    QHBoxLayout *urlBar = new QHBoxLayout();
    urlBar->setContentsMargins(11, 7, 11, 0);
    urlBar->setSpacing(15);
    QIcon backIcon(":/icons/images/Black-angle-left.png");
    QIcon forwardIcon(":/icons/images/Black-angle-right.png");
    QIcon refreshIcon(":/icons/images/Black-refresh.png");
    QIcon optionsIcon(":/icons/images/menu-dots-vertical.png");
    backButton = new QPushButton(this);
    backButton->setIcon(backIcon);
    backButton->setStyleSheet("QPushButton{ margin-right: 5px; border: none; }"
                              "QPushButton:hover{ background: #ececec; border-radius: 10px;}");
    backButton->setMinimumSize(40, 30);
    forwardButton = new QPushButton(this);
    forwardButton->setIcon(forwardIcon);
    forwardButton->setStyleSheet("QPushButton{ margin-right: 5px; border: none; }"
                               "QPushButton:hover{ background: #ececec; border-radius: 10px;}");
    forwardButton->setMinimumSize(40, 30);
    refreshButton = new QPushButton(this);
    refreshButton->setIcon(refreshIcon);
    refreshButton->setStyleSheet("QPushButton{ margin-right: 5px; border: none; }"
                                 "QPushButton:hover{ background: #ececec; border-radius: 10px;}");
    refreshButton->setMinimumSize(40, 30);
    urlSearch = new QLineEdit(this);
    urlSearch->setStyleSheet("QLineEdit{border: none; padding-left: 20px; background: #eeeeee; border-radius: 17px;}"
                             "QLineEdit:focus{background: #fff; border: 1px solid #0FF3E8;}");
    urlSearch->setMinimumSize(0, 35);
    urlSearch->setPlaceholderText("Search Google or URL");
    urlSearch->installEventFilter(this);
    options = new QPushButton(this);
    options->setIcon(optionsIcon);
    options->setStyleSheet("QPushButton{ margin-right: 5px; border: none; }"
                                 "QPushButton:hover{ background: #ececec; border-radius: 10px;}");
    options->setMinimumSize(40, 30);
    urlBar->addWidget(backButton);
    urlBar->addWidget(forwardButton);
    urlBar->addWidget(refreshButton);
    urlBar->addWidget(urlSearch);
    urlBar->addWidget(options);
    urlBarFrame->setLayout(urlBar);
    urlBarFrame->setMaximumHeight(55);
    QFrame *webPageFrame = new QFrame(this);
    QHBoxLayout *webPageView = new QHBoxLayout();
    webPageView->addWidget(webView);
    webPageView->setContentsMargins(0,0,0,0);
    webPageFrame->setLayout(webPageView);
    webPageFrame->setStyleSheet("border-top: 1px solid #d8d8d8;");
    QVBoxLayout *mainPage = new QVBoxLayout();
    mainPage->setContentsMargins(0, 0, 0, 0);
    mainPage->setSpacing(3);
    mainPage->addWidget(urlBarFrame);
    mainPage->addWidget(webPageFrame);
    setLayout(mainPage);
    connect(backButton, &QPushButton::clicked, this, &seaTab::onBackClicked);
    connect(forwardButton, &QPushButton::clicked, this, &seaTab::onForwardClicked);
    connect(refreshButton, &QPushButton::clicked, this, &seaTab::onRefreshClicked);
    connect(urlSearch, &QLineEdit::returnPressed, this, &seaTab::processURL);

}

void seaTab::onBackClicked(){
    webView->back();
}

void seaTab::onForwardClicked(){
    webView->forward();
}

void seaTab::onRefreshClicked(){
    webView->reload();
}

void seaTab::processURL()
{
    QString urlText = urlSearch->text();
    QUrl url1;
    QStringList parts = urlText.split('.', Qt::SkipEmptyParts);
    if (!urlText.contains(' ') && parts.size() >= 2 && QUrl(urlText).scheme().isEmpty()){
        url1 = QUrl::fromUserInput(urlText);
        url1.setScheme("https");
    } else {
        if(!QUrl(urlText).scheme().isEmpty()){
            url1 = QUrl(urlText);
        } else {
            QByteArray encodedString = QUrl::toPercentEncoding(urlText);
            QString newString = "https://www.google.com/search?q=" + encodedString;
            url1 = QUrl(newString);
        }
    }
    webView->load(url1);
    connect(webView, &QWebEngineView::loadFinished, this, [this]() {
        QWebEnginePage *page = webView->page();
        urlSearch->setText(page->url().toString());

    });
    urlSearch->clearFocus();
}

void seaTab::urlChange(const QUrl &url){
    urlSearch->setText(url.toString());
}

bool seaTab::doesUrlExist(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->head(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(2000, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {

        reply->deleteLater();
        return true;
    }

    reply->deleteLater();
    return false;
}

void seaTab::openDevTools(){
    QWebEnginePage *page = webView->page();
    page->settings()->setAttribute(QWebEngineSettings::WebAttribute::JavascriptEnabled, true);
    page->setInspectedPage(page);
    page->triggerAction(QWebEnginePage::WebAction::InspectElement);
}

QWebEngineView* seaTab::getWebView() const
{
    return webView;
}

bool seaTab::eventFilter(QObject *obj, QEvent *event){

    if (event->type() == QEvent::MouseButtonPress) {
        if(obj == urlSearch){
            QTimer::singleShot(0, urlSearch, &QLineEdit::selectAll);
        }
    }
    return QWidget::eventFilter(obj, event);
}

seaTab::~seaTab() {
    delete webView;
    delete networkManager;
}



