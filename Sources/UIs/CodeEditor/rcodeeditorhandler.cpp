#include "rcodeeditorhandler.h"
#include "ui_RCodeEditorHandler.h"
#include <QMouseEvent>

RCodeEditorHandler::RCodeEditorHandler(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RCodeEditorHandler)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    performConnection();
}

RCodeEditorHandler::~RCodeEditorHandler() {
    delete ui;
}

void RCodeEditorHandler::performConnection() {

}

void RCodeEditorHandler::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        last = event->globalPosition();
    }
}

void RCodeEditorHandler::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        auto dx = event->globalPosition().x() - last.x();
        auto dy = event->globalPosition().y() - last.y();
        last = event->globalPosition();
        move(x() + dx, y() + dy);
    }
}

void RCodeEditorHandler::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int dx = event->globalPosition().x() - last.x();
        int dy = event->globalPosition().y() - last.y();
        move(x()+dx, y()+dy);
    }
}
