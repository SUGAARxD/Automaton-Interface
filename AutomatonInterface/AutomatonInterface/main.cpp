#include "AutomatonInterface.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AutomatonInterface w;
    w.show();
    return a.exec();
}
