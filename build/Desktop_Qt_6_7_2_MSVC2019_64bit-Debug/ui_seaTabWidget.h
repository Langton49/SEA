/********************************************************************************
** Form generated from reading UI file 'seaTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEATABWIDGET_H
#define UI_SEATABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTabWidget>

QT_BEGIN_NAMESPACE

class Ui_TabWidget
{
public:

    void setupUi(QTabWidget *TabWidget)
    {
        if (TabWidget->objectName().isEmpty())
            TabWidget->setObjectName("TabWidget");
        TabWidget->resize(1142, 761);
        TabWidget->setStyleSheet(QString::fromUtf8("QTabBar::tab{\n"
"width: 200px;\n"
"height: 14px;\n"
"padding: 10px;\n"
"background: white;\n"
"margin-top: 8px;\n"
"border-radius: 10px;\n"
"margin-bottom: 5px;\n"
"margin-left: 8px;\n"
"border: 2px solid transparent;\n"
"}\n"
"\n"
"QTabBar::tab:selected{\n"
"background: white;\n"
"border: 2px solid #0FF3E8;\n"
"}\n"
"\n"
"QTabBar::tab:hover{\n"
"background: #c9c9c9;\n"
"color: white;\n"
"}\n"
"\n"
"QTabBar::tab:selected:hover{\n"
"background: white;\n"
"color: black;\n"
"}\n"
"\n"
"QTabBar::close-button{\n"
"image: url(:/icons/images/black_cross.png);\n"
"margin-right: 3px;\n"
"margin-top: 3px;\n"
"background: transparent;\n"
"padding: 2px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QTabBar::close-button:hover{\n"
"image: url(:/icons/images/white_cross.png);\n"
"}\n"
"\n"
"CustomTabWidget{\n"
"border: 2px solid;\n"
"}\n"
""));
        TabWidget->setElideMode(Qt::TextElideMode::ElideRight);
        TabWidget->setTabsClosable(true);
        TabWidget->setTabBarAutoHide(true);

        retranslateUi(TabWidget);

        QMetaObject::connectSlotsByName(TabWidget);
    } // setupUi

    void retranslateUi(QTabWidget *TabWidget)
    {
        TabWidget->setWindowTitle(QCoreApplication::translate("TabWidget", "TabWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TabWidget: public Ui_TabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEATABWIDGET_H
