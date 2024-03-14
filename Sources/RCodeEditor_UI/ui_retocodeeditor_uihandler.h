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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RetoCodeEditor_UIHandler
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RetoCodeEditor_UIHandler)
    {
        if (RetoCodeEditor_UIHandler->objectName().isEmpty())
            RetoCodeEditor_UIHandler->setObjectName("RetoCodeEditor_UIHandler");
        RetoCodeEditor_UIHandler->resize(1920, 1080);
        centralwidget = new QWidget(RetoCodeEditor_UIHandler);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 0, 1919, 1029));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(verticalLayoutWidget);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        RetoCodeEditor_UIHandler->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RetoCodeEditor_UIHandler);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1920, 21));
        RetoCodeEditor_UIHandler->setMenuBar(menubar);
        statusbar = new QStatusBar(RetoCodeEditor_UIHandler);
        statusbar->setObjectName("statusbar");
        RetoCodeEditor_UIHandler->setStatusBar(statusbar);

        retranslateUi(RetoCodeEditor_UIHandler);

        QMetaObject::connectSlotsByName(RetoCodeEditor_UIHandler);
    } // setupUi

    void retranslateUi(QMainWindow *RetoCodeEditor_UIHandler)
    {
        RetoCodeEditor_UIHandler->setWindowTitle(QCoreApplication::translate("RetoCodeEditor_UIHandler", "RetoCodeEditor_UIHandler", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("RetoCodeEditor_UIHandler", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("RetoCodeEditor_UIHandler", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RetoCodeEditor_UIHandler: public Ui_RetoCodeEditor_UIHandler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETOCODEEDITOR_UIHANDLER_H
