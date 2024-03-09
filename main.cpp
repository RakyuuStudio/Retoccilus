#include <QtQml>
#include <QApplication>
#include "Sources/MainWindow/rmainwindow.h"
#include "Sources/UIs/CodeEditor/rcodeeditorhandler.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

//    RMainWindow *rm = new RMainWindow;
//    rm->show();

    RCodeEditorHandler *rceh = new RCodeEditorHandler;
    rceh->show();

    return QApplication::exec();
}
