#include <QtGui/QApplication>
#include "qtclausula.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtClausula w;
    w.show();
    return a.exec();
}
