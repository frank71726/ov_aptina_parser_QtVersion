#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(picture);

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
