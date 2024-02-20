#include "rmainwindow.h"
#include "ui_rmainwindow.h"
#include "RCodeEditor.h"

namespace RetoNS {
    RMainWindow::RMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::RMainWindow) {
        ui->setupUi(this);
        RCodeEditor *rce = new RCodeEditor(this);
        rce->setStyleSheet("QPlainTextEdit { background-color: #383a59; font-family: \"Consolas\" }");
        setCentralWidget(rce);
    }

    RMainWindow::~RMainWindow() {
        delete ui;
    }
} // RetoNS
