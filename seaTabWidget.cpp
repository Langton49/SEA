#include "seaTabWidget.h"
#include "ui_seaTabWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTabBar>
#include <QIcon>
#include <QSize>
#include <QEvent>
#include "seaTab.h"

seaTabWidget::seaTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , loading(new QMovie(":/icons/images/loading.gif"))
    , defaultFavicon(":/icons/images/file.png")
    , noEndpointFavicon(":/icons/images/document-circle-wrong.png")
    , ui(new Ui::TabWidget)
{
    ui->setupUi(this);
    frame = new QFrame(this);
    frame->setStyleSheet("QFrame{background: transparent; border: none;}");
    QHBoxLayout *frameLayout = new QHBoxLayout();
    frameLayout->setContentsMargins(0, 0, 0, 0);
    frameLayout->setSpacing(0);
    newTab = new QPushButton(this);
    minimize = new QPushButton(this);
    maximize = new QPushButton(this);
    closeWindow = new QPushButton(this);
    connect(maximize, &QPushButton::clicked, this, &seaTabWidget::onMaximizeClicked);
    connect(minimize, &QPushButton::clicked, this, &seaTabWidget::onMinimizeClicked);
    connect(closeWindow, &QPushButton::clicked, this, &seaTabWidget::onCloseClicked);
    closeWindow->installEventFilter(this);
    newTab->setStyleSheet("QPushButton{background: transparent; border: none; border-radius: 9px; height: 100%; padding-left: 15px; padding-right: 15px; margin: 5px;}"
                          "QPushButton:hover{background: #cacaca;}");
    minimize->setStyleSheet("QPushButton{background: transparent; border: none; height: 100%; padding-left: 25px; padding-right: 25px;}"
                            "QPushButton:hover{background: #dfdddd;}");
    maximize->setStyleSheet("QPushButton{background: transparent; border: none; height: 100%; padding-left: 25px; padding-right: 25px;}"
                            "QPushButton:hover{background: #dfdddd;}");
    closeWindow->setStyleSheet("QPushButton{background: transparent; border: none; height: 100%; padding-left: 25px; padding-right: 25px;}"
                         "QPushButton:hover{background: #d30000;}");
    QIcon minus(":/icons/images/minus.png");
    QIcon square(":/icons/images/square.png");
    QIcon cross(":/icons/images/black_cross.png");
    QIcon plus(":/icons/images/plus.png");
    QSize iconSize(10, 10);
    minimize->setIcon(minus);
    maximize->setIcon(square);
    closeWindow->setIcon(cross);
    newTab->setIcon(plus);
    minimize->setIconSize(iconSize);
    maximize->setIconSize(iconSize);
    closeWindow->setIconSize(iconSize);
    newTab->setIconSize(QSize(12, 12));
    frameLayout->addWidget(minimize);
    frameLayout->addWidget(maximize);
    frameLayout->addWidget(closeWindow);
    frame->setLayout(frameLayout);
    QWidget *tabBarContainer = new QWidget(this);
    QHBoxLayout *tabBarLayout = new QHBoxLayout(tabBarContainer);
    tabBarLayout->setContentsMargins(0, 0, 0, 0);
    tabBarLayout->addWidget(frame);
    tabBarLayout->setAlignment(frame, Qt::AlignRight);
    setCornerWidget(tabBarContainer, Qt::TopRightCorner);
    setWindowFlags(Qt::FramelessWindowHint);
    setupNewTabButton();
    addNewTab();
}

void seaTabWidget::setupNewTabButton()
{
    QWidget* cornerWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(cornerWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(newTab);
    setCornerWidget(cornerWidget, Qt::TopLeftCorner);
    connect(newTab, &QPushButton::clicked, this, &seaTabWidget::addNewTab);
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, [this](int index){
        removeTab(index);
        count--;
        checkTabNumber();
    });

}

void seaTabWidget::onMinimizeClicked()
{
    showMinimized();
}

void seaTabWidget::onMaximizeClicked()
{
    if (isMaximized()){
        showNormal();
        maximize->setIcon(QIcon(":/icons/images/square.png"));
    }
    else{
        showMaximized();
        maximize->setIcon(QIcon(":/icons/images/minimize.png"));
    }

}

void seaTabWidget::onCloseClicked()
{
    close();
}

void seaTabWidget::checkTabNumber(){
    if(!count){
        close();
    }
}


bool seaTabWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == closeWindow) {
        if (event->type() == QEvent::Enter) {
            closeWindow->setIcon(QIcon(":/icons/images/white_cross.png"));
            closeWindow->setIconSize(QSize(10, 10));
        }
        else if (event->type() == QEvent::Leave) {
            closeWindow->setIcon(QIcon(":/icons/images/black_cross.png"));
            closeWindow->setIconSize(QSize(10, 10));
        }
    }
    return QTabWidget::eventFilter(obj, event);
}

void seaTabWidget::addNewTab() {
    seaTab *newTab = new seaTab(this);
    QWebEngineView *webView = newTab->getWebView();
    int newIndex = addTab(newTab, QString("New Tab"));
    setTabIcon(newIndex, defaultFavicon);
    count++;
    setCurrentIndex(newIndex);

    if (webView) {
        connect(webView, &QWebEngineView::titleChanged, this, [this, newIndex](const QString &title) {
            updateTabTitle(newIndex, title);
        });

        connect(webView, &QWebEngineView::loadStarted, this, [this]() {
            setTabIcon(currentIndex(), QIcon(loading->currentPixmap()));
            loading->start();
            connect(loading, &QMovie::frameChanged, this, [this]() {
                setTabIcon(currentIndex(), QIcon(loading->currentPixmap()));
            });
        });

        connect(webView, &QWebEngineView::loadFinished, this, [this, newIndex, webView]() {
            QWebEnginePage *page = webView->page();

            auto iconChangedPtr = std::make_shared<bool>(false);

            connect(page, &QWebEnginePage::iconChanged, this, [this, iconChangedPtr](const QIcon &icon) {
                loading->stop();
                *iconChangedPtr = true;
                setTabIcon(currentIndex(), icon);
                tabBar()->update();
            });

            QTimer::singleShot(2000, [this, newIndex, iconChangedPtr]() {
                if (!*iconChangedPtr) {
                    loading->stop();
                    setTabIcon(currentIndex(), defaultFavicon);
                    tabBar()->update();
                }
            });
        });
    }
}

void seaTabWidget::updateTabTitle(int index, const QString &title)
{
    setTabText(index, title);
}


seaTabWidget::~seaTabWidget()
{
    delete ui;
}
