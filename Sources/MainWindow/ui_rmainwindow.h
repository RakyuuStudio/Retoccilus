/********************************************************************************
** Form generated from reading UI file 'rmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
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

namespace RetoNS {

class Ui_RMainWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RetoNS__RMainWindow)
    {
        if (RetoNS__RMainWindow->objectName().isEmpty())
            RetoNS__RMainWindow->setObjectName("RetoNS__RMainWindow");
        RetoNS__RMainWindow->resize(400, 300);
        centralwidget = new QWidget(RetoNS__RMainWindow);
        centralwidget->setObjectName("centralwidget");
        RetoNS__RMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RetoNS__RMainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 400, 17));
        RetoNS__RMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RetoNS__RMainWindow);
        statusbar->setObjectName("statusbar");
        RetoNS__RMainWindow->setStatusBar(statusbar);

        retranslateUi(RetoNS__RMainWindow);

        QMetaObject::connectSlotsByName(RetoNS__RMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RetoNS__RMainWindow)
    {
        RetoNS__RMainWindow->setWindowTitle(QCoreApplication::translate("RetoNS::RMainWindow", "RMainWindow", nullptr));
    } // retranslateUi

};

} // namespace RetoNS

namespace RetoNS {
namespace Ui {
    class RMainWindow: public Ui_RMainWindow {};
} // namespace Ui
} // namespace RetoNS

#endif // UI_RMAINWINDOW_H
