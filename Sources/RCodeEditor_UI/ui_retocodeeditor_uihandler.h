/********************************************************************************
** Form generated from reading UI file 'retocodeeditor_uihandler.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETOCODEEDITOR_UIHANDLER_H
#define UI_RETOCODEEDITOR_UIHANDLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RetoCodeEditor_UIHandler
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;

    void setupUi(QMainWindow *RetoCodeEditor_UIHandler)
    {
        if (RetoCodeEditor_UIHandler->objectName().isEmpty())
            RetoCodeEditor_UIHandler->setObjectName("RetoCodeEditor_UIHandler");
        RetoCodeEditor_UIHandler->resize(1920, 1080);
        centralwidget = new QWidget(RetoCodeEditor_UIHandler);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(0, -1, 1920, 1052));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        RetoCodeEditor_UIHandler->setCentralWidget(centralwidget);

        retranslateUi(RetoCodeEditor_UIHandler);

        QMetaObject::connectSlotsByName(RetoCodeEditor_UIHandler);
    } // setupUi

    void retranslateUi(QMainWindow *RetoCodeEditor_UIHandler)
    {
        RetoCodeEditor_UIHandler->setWindowTitle(QCoreApplication::translate("RetoCodeEditor_UIHandler", "RetoCodeEditor_UIHandler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RetoCodeEditor_UIHandler: public Ui_RetoCodeEditor_UIHandler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETOCODEEDITOR_UIHANDLER_H
