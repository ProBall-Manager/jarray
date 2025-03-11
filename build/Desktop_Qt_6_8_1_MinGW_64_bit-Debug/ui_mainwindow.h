/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableView *tableView;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_ID_2;
    QLineEdit *lineEdit_Column1;
    QLineEdit *lineEdit_Column2;
    QLineEdit *lineEdit_Column3;
    QPushButton *pushButton_Create;
    QPushButton *pushButton_Update;
    QPushButton *pushButton_Delete;
    QPushButton *pushButton_Read;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(310, 30, 451, 311));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 110, 49, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(60, 150, 49, 16));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(60, 190, 49, 16));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(60, 230, 49, 16));
        lineEdit_ID_2 = new QLineEdit(centralwidget);
        lineEdit_ID_2->setObjectName("lineEdit_ID_2");
        lineEdit_ID_2->setGeometry(QRect(120, 110, 121, 21));
        lineEdit_Column1 = new QLineEdit(centralwidget);
        lineEdit_Column1->setObjectName("lineEdit_Column1");
        lineEdit_Column1->setGeometry(QRect(120, 150, 121, 24));
        lineEdit_Column2 = new QLineEdit(centralwidget);
        lineEdit_Column2->setObjectName("lineEdit_Column2");
        lineEdit_Column2->setGeometry(QRect(120, 190, 121, 24));
        lineEdit_Column3 = new QLineEdit(centralwidget);
        lineEdit_Column3->setObjectName("lineEdit_Column3");
        lineEdit_Column3->setGeometry(QRect(120, 230, 121, 24));
        pushButton_Create = new QPushButton(centralwidget);
        pushButton_Create->setObjectName("pushButton_Create");
        pushButton_Create->setGeometry(QRect(30, 420, 80, 24));
        pushButton_Update = new QPushButton(centralwidget);
        pushButton_Update->setObjectName("pushButton_Update");
        pushButton_Update->setGeometry(QRect(230, 420, 80, 24));
        pushButton_Delete = new QPushButton(centralwidget);
        pushButton_Delete->setObjectName("pushButton_Delete");
        pushButton_Delete->setGeometry(QRect(340, 420, 80, 24));
        pushButton_Read = new QPushButton(centralwidget);
        pushButton_Read->setObjectName("pushButton_Read");
        pushButton_Read->setGeometry(QRect(130, 420, 80, 24));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        pushButton_Create->setText(QCoreApplication::translate("MainWindow", "create", nullptr));
        pushButton_Update->setText(QCoreApplication::translate("MainWindow", "update", nullptr));
        pushButton_Delete->setText(QCoreApplication::translate("MainWindow", "delete", nullptr));
        pushButton_Read->setText(QCoreApplication::translate("MainWindow", "read", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
