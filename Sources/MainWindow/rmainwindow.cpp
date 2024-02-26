#include "rmainwindow.h"
#include "ui_rmainwindow.h"
#include "RCodeEditor.h"

RMainWindow::RMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::RMainWindow) {
    ui->setupUi(this);
    RCodeEditor *rce = new RCodeEditor(this);
    rce->setStyleSheet("QPlainTextEdit { background-color: #282a36; font-family: \"Consolas\" }");
    RSyntaxHighlighter *rhl;
    rhl = new RSyntaxHighlighter(rce->document());
    setCentralWidget(rce);
}

RMainWindow::~RMainWindow() {
    delete ui;
}
