/********************************************************************************
** Form generated from reading UI file 'rce_ui.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCE_UI_H
#define UI_RCE_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RCE_UI
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RCE_UI)
    {
        if (RCE_UI->objectName().isEmpty())
            RCE_UI->setObjectName("RCE_UI");
        RCE_UI->resize(1280, 768);
        centralwidget = new QWidget(RCE_UI);
        centralwidget->setObjectName("centralwidget");
        RCE_UI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RCE_UI);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1920, 21));
        RCE_UI->setMenuBar(menubar);
        statusbar = new QStatusBar(RCE_UI);
        statusbar->setObjectName("statusbar");
        RCE_UI->setStatusBar(statusbar);

        retranslateUi(RCE_UI);

        QMetaObject::connectSlotsByName(RCE_UI);
    } // setupUi

    void retranslateUi(QMainWindow *RCE_UI)
    {
        RCE_UI->setWindowTitle(QCoreApplication::translate("RCE_UI", "RCE_UI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RCE_UI: public Ui_RCE_UI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCE_UI_H
