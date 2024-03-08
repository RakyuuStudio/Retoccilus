#include <QApplication>
#include "Sources/MainWindow/rmainwindow.h"
#include "Sources/RCodeEditor_UI/rce_ui.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

//    RMainWindow *rm = new RMainWindow;
//    rm->show();

    auto *rce = new RCE_UI;
    rce->show();

    return QApplication::exec();
}
