#include "rmainwindow.h"
#include "ui_rmainwindow.h"

namespace RetoNS {
RMainWindow::RMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::RMainWindow) {
    ui->setupUi(this);
}

RMainWindow::~RMainWindow() {
    delete ui;
}
} // RetoNS
