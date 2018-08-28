#include "mapwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("MapRouting v1.0");
    w.show();

    return a.exec();
}
