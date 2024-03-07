#include "rmainwindow.h"
#include "ui_rmainwindow.h"

#include "RCodeEditor.h"
#include "RCFamilyHighlighter.h"
#include "RSyntaxStyle.h"

RMainWindow::RMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::RMainWindow) {
    ui->setupUi(this);
    auto *rce = new RCodeEditor(this);
    rce->setStyleSheet("QTextEdit { background-color: #282a36; color: #ffffff; font-family: \"Consolas\" }");
    setCentralWidget(rce);
}

RMainWindow::~RMainWindow() {
    delete ui;
}