#include "rce_ui.h"
#include "ui_RCE_UI.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

RCE_UI::RCE_UI(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RCE_UI)
{
    ui->setupUi(this);
    initUI();
}

RCE_UI::~RCE_UI() {
    delete ui;
}

void RCE_UI::initUI() {
    this->setWindowFlags(Qt::FramelessWindowHint);

    closeButton = new FluentPushButton(this, "Close");

    auto *layout = new QGridLayout();
    layout->setSpacing(0);

    layout->addWidget(closeButton, 0, 0, 1, 1);

    connect(closeButton, &FluentPushButton::clicked, this, &RCE_UI::onCloseTriggered);

    setLayout(layout);
}

void RCE_UI::onCloseTriggered() {
    this->close();
}

void RCE_UI::onMinimizeTriggered() {
    this->showMinimized();
}

void RCE_UI::onMaximizeTriggered() {
    if (this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}