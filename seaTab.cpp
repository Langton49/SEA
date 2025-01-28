#include "seaTab.h"
#include "customWebpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInfo>
#include <QWebEngineUrlRequestInterceptor>
#include <QStandardPaths>

seaTab::seaTab(QWidget *parent)
    : QWidget(parent),
    webView(new QWebEngineView(this)),
    networkManager(new QNetworkAccessManager(this))
{
    int fontId = QFontDatabase::addApplicationFont(":/../Downloads/Signika_Negative/SignikaNegative-VariableFont_wght.ttf");
    if (fontId != -1) {
        QString customFont = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont customAppFont(customFont, 10);
        this->setFont(customAppFont);
        urlSearch = new QLineEdit(this);
        urlSearch->setStyleSheet("QLineEdit{border: none; font-family: '%1'; font-size: 12px; padding-left: 20px; background: #eeeeee; border-radius: 17px;}"
                                 "QLineEdit:focus{background: #fff; border: 1px solid #0FF3E8;}");
    }
    CustomWebPage *page = new CustomWebPage(this);
    webView->setPage(page);

    QWebEngineSettings *settings = page->settings();
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly, true);
    settings->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, true);
    settings->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    settings->setAttribute(QWebEngineSettings::ShowScrollBars, true);
    settings->setAttribute(QWebEngineSettings::WebGLEnabled, true);  // Allow mixed content
    QString userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36";
    page->profile()->setHttpUserAgent(userAgent);
    // Add this line if you want to enable DRM content (like some video players)
    page->profile()->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);

    setupCache();
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
    connect(page, &CustomWebPage::newTabRequested, this, &seaTab::onNewTabRequested);

    class RequestInterceptor : public QWebEngineUrlRequestInterceptor {
    public:
        RequestInterceptor(seaTab* tab) : tab_(tab) {}
        void interceptRequest(QWebEngineUrlRequestInfo &info) override {
            tab_->interceptRequest(info);
        }
    private:
        seaTab* tab_;
    };

    webView->page()->profile()->setUrlRequestInterceptor(
        new RequestInterceptor(this));

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
    QUrl url;

    // Existing URL processing code...
    QStringList parts = urlText.split('.', Qt::SkipEmptyParts);
    if (!urlText.contains(' ') && parts.size() >= 2 &&
        QUrl(urlText).scheme().isEmpty()) {
        url = QUrl::fromUserInput(urlText);
        url.setScheme("https");
    } else {
        if (!QUrl(urlText).scheme().isEmpty()) {
            url = QUrl(urlText);
        } else {
            QByteArray encodedString = QUrl::toPercentEncoding(urlText);
            QString newString = "https://www.google.com/search?q=" + encodedString;
            url = QUrl(newString);
        }
    }

    // Try loading from cache first
    loadFromCache(url);

    // If not in cache or expired, load from network
    webView->load(url);
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

void seaTab::onNewTabRequested(const QUrl &url)
{
    emit newTabRequested(url);
}

seaTab::~seaTab() {
    delete webView;
    delete networkManager;
}

void seaTab::setupCache()
{
    QString cachePath = QStandardPaths::writableLocation(
                            QStandardPaths::CacheLocation) + "/browser-cache";
    cache = new BrowserCache(cachePath, 50, 500); // 50MB RAM, 500MB disk
}

void seaTab::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    QUrl url = info.requestUrl();

    // Skip non-GET requests and non-cacheable content
    if (info.requestMethod() != "GET" ||
        url.scheme() == "data" ||
        url.scheme() == "blob" ||
        url.scheme() == "file") {
        return;
    }

    // Check cache for valid entry
    if (CacheEntry* entry = cache->retrieve(url)) {
        // Add validation headers if available
        if (!entry->etag.isEmpty()) {
            info.setHttpHeader("If-None-Match", entry->etag.toUtf8());
        }
        if (!entry->lastModified.isEmpty()) {
            info.setHttpHeader("If-Modified-Since",
                               entry->lastModified.toUtf8());
        }
    }
}

void seaTab::handleLoadFinished(bool ok)
{
    if (!ok) return;

    QWebEnginePage *page = webView->page();
    QUrl url = page->url();

    // Cache the current page
    page->toHtml([this, url](const QString &html) {
        QByteArray data = html.toUtf8();
        cacheResponse(url, data, "text/html");
    });

    // Update URL bar
    urlSearch->setText(url.toString());
}

void seaTab::loadFromCache(const QUrl &url)
{
    if (CacheEntry* entry = cache->retrieve(url)) {
        if (entry->contentType == "text/html") {
            webView->setHtml(QString::fromUtf8(entry->data), url);
        }
    }
}

void seaTab::cacheResponse(const QUrl &url, const QByteArray &data,
                           const QString &contentType)
{
    // Get response headers from the page
    QWebEnginePage *page = webView->page();

    // Cache the response with a default 1-hour expiry
    QDateTime expiry = QDateTime::currentDateTime().addSecs(3600);

    cache->store(url, data, contentType, expiry);
}






