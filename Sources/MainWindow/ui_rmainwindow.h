/********************************************************************************
** Form generated from reading UI file 'rmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RMAINWINDOW_H
#define UI_RMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RMainWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RMainWindow)
    {
        if (RMainWindow->objectName().isEmpty())
            RMainWindow->setObjectName("RMainWindow");
        RMainWindow->resize(1280, 768);
        centralwidget = new QWidget(RMainWindow);
        centralwidget->setObjectName("centralwidget");
        RMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RMainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 400, 17));
        RMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RMainWindow);
        statusbar->setObjectName("statusbar");
        RMainWindow->setStatusBar(statusbar);

        retranslateUi(RMainWindow);

        QMetaObject::connectSlotsByName(RMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RMainWindow)
    {
        RMainWindow->setWindowTitle(QCoreApplication::translate("RMainWindow", "RMainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RMainWindow: public Ui_RMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RMAINWINDOW_H
