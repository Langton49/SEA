#ifndef SEATABWIDGET_H
#define SEATABWIDGET_H

#include <QTabWidget>
#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMovie>
#include <QtUiTools/QUiLoader>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui {
class TabWidget;
}
QT_END_NAMESPACE

class seaTabWidget : public QTabWidget {
    Q_OBJECT

public:
    explicit seaTabWidget(QWidget *parent = nullptr);
    ~seaTabWidget();
    QPushButton* getMinimizeButton() const;
    QPushButton* getMaximizeButton() const;
    QPushButton* getCloseButton() const;
    int getCount();
    using QTabWidget::addTab;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();
    void checkTabNumber();
    void addNewTab();
    QMovie *loading;
    QIcon defaultFavicon;
    QIcon noEndpointFavicon;
    int count = 0;
    QFrame *frame;
    QPushButton *closeWindow;
    QPushButton *minimize;
    QPushButton *maximize;
    QPushButton *newTab;
    void setupNewTabButton();
    Ui::TabWidget *ui;
    void updateTabTitle(int index, const QString &title);

};



#endif // SEATABWIDGET_H
