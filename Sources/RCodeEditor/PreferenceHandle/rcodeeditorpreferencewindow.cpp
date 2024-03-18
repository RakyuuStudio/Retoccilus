//
// Created by ryan_ on 2024/3/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RCodeEditorPreferenceWindow.h" resolved

#include "rcodeeditorpreferencewindow.h"
#include "ui_RCodeEditorPreferenceWindow.h"


RCodeEditorPreferenceWindow::RCodeEditorPreferenceWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::RCodeEditorPreferenceWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &RCodeEditorPreferenceWindow::changeIndex);
}

RCodeEditorPreferenceWindow::~RCodeEditorPreferenceWindow() {
    delete ui;
}

void RCodeEditorPreferenceWindow::changeIndex(QTreeWidgetItem *item, int col) {
    if (item->text(col) == "Apperance") {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (item->text(col) == "File Color") {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if (item->text(col) == "Notification") {
        ui->stackedWidget->setCurrentIndex(3);
    }
}