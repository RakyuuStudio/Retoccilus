#include "rmainwindow.h"
#include "ui_rmainwindow.h"

#include "RCodeEditor.h"
#include "RCFamilyHighlighter.h"
#include "RSyntaxStyle.h"

//using namespace RetoccilusCodeEditor;

RMainWindow::RMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::RMainWindow) {
    ui->setupUi(this);
    auto *rce = new RCodeEditor(this);
    rce->setStyleSheet("QPlainTextEdit { background-color: #282a36; color: #ffffff; font-family: \"Consolas\" }");
    auto *highlighter = new RCFamilyHighlighter(rce->document());
    setCentralWidget(rce);
}

RMainWindow::~RMainWindow() {
    delete ui;
}