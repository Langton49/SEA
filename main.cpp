#include "seaTabWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    seaTabWidget w;
    w.show();
    return a.exec();
}
