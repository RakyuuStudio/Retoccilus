#include <QApplication>
#include "Sources/MainWindow/rmainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    RMainWindow *rm = new RMainWindow;
    rm->show();

    return QApplication::exec();
}
