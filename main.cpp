#include <QApplication>
#include "Sources/MainWindow/rmainwindow.h"

using namespace RetoNS;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    RetoNS::RMainWindow *rm = new RetoNS::RMainWindow;
    rm->show();

    return QApplication::exec();
}
