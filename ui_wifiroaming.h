/********************************************************************************
** Form generated from reading UI file 'wifiroaming.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIFIROAMING_H
#define UI_WIFIROAMING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wifiRoaming
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *wifiRoaming)
    {
        if (wifiRoaming->objectName().isEmpty())
            wifiRoaming->setObjectName(QStringLiteral("wifiRoaming"));
        wifiRoaming->resize(400, 300);
        menuBar = new QMenuBar(wifiRoaming);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        wifiRoaming->setMenuBar(menuBar);
        mainToolBar = new QToolBar(wifiRoaming);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        wifiRoaming->addToolBar(mainToolBar);
        centralWidget = new QWidget(wifiRoaming);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        wifiRoaming->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(wifiRoaming);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        wifiRoaming->setStatusBar(statusBar);

        retranslateUi(wifiRoaming);

        QMetaObject::connectSlotsByName(wifiRoaming);
    } // setupUi

    void retranslateUi(QMainWindow *wifiRoaming)
    {
        wifiRoaming->setWindowTitle(QApplication::translate("wifiRoaming", "wifiRoaming", 0));
    } // retranslateUi

};

namespace Ui {
    class wifiRoaming: public Ui_wifiRoaming {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIFIROAMING_H
