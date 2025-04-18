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
    QPushButton *pushButton_SortDateAsc;
    QPushButton *pushButton_SortDateDesc;
    QPushButton *pushButton_ExportPDF;
    QLineEdit *lineEdit_Score;
    QLineEdit *lineEdit_TypeMatch;
    QLineEdit *lineEdit_Spectateurs;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *pushButton_MatchStats;
    QPushButton *pushButton_Calendar;
    QLineEdit *lineEdit_SearchTypeMatch;
    QLabel *label_8;
    QLabel *label_9;
    QPushButton *pushButton_Simulate;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1126, 508);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(290, 70, 731, 311));
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
        pushButton_SortDateAsc = new QPushButton(centralwidget);
        pushButton_SortDateAsc->setObjectName("pushButton_SortDateAsc");
        pushButton_SortDateAsc->setGeometry(QRect(60, 50, 80, 24));
        pushButton_SortDateDesc = new QPushButton(centralwidget);
        pushButton_SortDateDesc->setObjectName("pushButton_SortDateDesc");
        pushButton_SortDateDesc->setGeometry(QRect(160, 50, 80, 24));
        pushButton_ExportPDF = new QPushButton(centralwidget);
        pushButton_ExportPDF->setObjectName("pushButton_ExportPDF");
        pushButton_ExportPDF->setGeometry(QRect(520, 410, 80, 24));
        lineEdit_Score = new QLineEdit(centralwidget);
        lineEdit_Score->setObjectName("lineEdit_Score");
        lineEdit_Score->setGeometry(QRect(120, 270, 121, 24));
        lineEdit_TypeMatch = new QLineEdit(centralwidget);
        lineEdit_TypeMatch->setObjectName("lineEdit_TypeMatch");
        lineEdit_TypeMatch->setGeometry(QRect(120, 310, 121, 24));
        lineEdit_Spectateurs = new QLineEdit(centralwidget);
        lineEdit_Spectateurs->setObjectName("lineEdit_Spectateurs");
        lineEdit_Spectateurs->setGeometry(QRect(120, 350, 121, 24));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(60, 270, 49, 16));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(38, 310, 71, 20));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(38, 350, 71, 20));
        pushButton_MatchStats = new QPushButton(centralwidget);
        pushButton_MatchStats->setObjectName("pushButton_MatchStats");
        pushButton_MatchStats->setGeometry(QRect(900, 410, 101, 31));
        pushButton_MatchStats->setStyleSheet(QString::fromUtf8("color: rgb(95, 161, 9);"));
        pushButton_Calendar = new QPushButton(centralwidget);
        pushButton_Calendar->setObjectName("pushButton_Calendar");
        pushButton_Calendar->setGeometry(QRect(770, 410, 101, 31));
        pushButton_Calendar->setStyleSheet(QString::fromUtf8("color: rgb(95, 161, 9);"));
        lineEdit_SearchTypeMatch = new QLineEdit(centralwidget);
        lineEdit_SearchTypeMatch->setObjectName("lineEdit_SearchTypeMatch");
        lineEdit_SearchTypeMatch->setGeometry(QRect(890, 20, 113, 24));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(730, 20, 141, 20));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(-260, -260, 1381, 761));
        label_9->setStyleSheet(QString::fromUtf8("background-image: url(:/Desktop/aa.png);"));
        pushButton_Simulate = new QPushButton(centralwidget);
        pushButton_Simulate->setObjectName("pushButton_Simulate");
        pushButton_Simulate->setGeometry(QRect(630, 410, 80, 24));
        MainWindow->setCentralWidget(centralwidget);
        label_9->raise();
        tableView->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        lineEdit_ID_2->raise();
        lineEdit_Column1->raise();
        lineEdit_Column2->raise();
        lineEdit_Column3->raise();
        pushButton_Create->raise();
        pushButton_Update->raise();
        pushButton_Delete->raise();
        pushButton_Read->raise();
        pushButton_SortDateAsc->raise();
        pushButton_SortDateDesc->raise();
        pushButton_ExportPDF->raise();
        lineEdit_Score->raise();
        lineEdit_TypeMatch->raise();
        lineEdit_Spectateurs->raise();
        label_5->raise();
        label_6->raise();
        label_7->raise();
        pushButton_MatchStats->raise();
        pushButton_Calendar->raise();
        lineEdit_SearchTypeMatch->raise();
        label_8->raise();
        pushButton_Simulate->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1126, 21));
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
        pushButton_SortDateAsc->setText(QCoreApplication::translate("MainWindow", "Sort Date \342\206\221", nullptr));
        pushButton_SortDateDesc->setText(QCoreApplication::translate("MainWindow", "Sort Date \342\206\223", nullptr));
        pushButton_ExportPDF->setText(QCoreApplication::translate("MainWindow", "Export to pdf", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Score", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Type match", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Spectatures", nullptr));
        pushButton_MatchStats->setText(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        pushButton_Calendar->setText(QCoreApplication::translate("MainWindow", "Calendar", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Recherche (type de match)", nullptr));
        label_9->setText(QString());
        pushButton_Simulate->setText(QCoreApplication::translate("MainWindow", "Simulation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
