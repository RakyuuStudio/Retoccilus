/********************************************************************************
** Form generated from reading UI file 'rcodeeditorhandler.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCODEEDITORHANDLER_H
#define UI_RCODEEDITORHANDLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RCodeEditorHandler
{
public:
    QWidget *centralwidget;

    void setupUi(QMainWindow *RCodeEditorHandler)
    {
        if (RCodeEditorHandler->objectName().isEmpty())
            RCodeEditorHandler->setObjectName("RCodeEditorHandler");
        RCodeEditorHandler->resize(1920, 1080);
        centralwidget = new QWidget(RCodeEditorHandler);
        centralwidget->setObjectName("centralwidget");
        RCodeEditorHandler->setCentralWidget(centralwidget);

        retranslateUi(RCodeEditorHandler);

        QMetaObject::connectSlotsByName(RCodeEditorHandler);
    } // setupUi

    void retranslateUi(QMainWindow *RCodeEditorHandler)
    {
        RCodeEditorHandler->setWindowTitle(QCoreApplication::translate("RCodeEditorHandler", "RCodeEditorHandler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RCodeEditorHandler: public Ui_RCodeEditorHandler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCODEEDITORHANDLER_H
