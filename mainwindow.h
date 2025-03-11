#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
#include "connection.h" // Make sure this header exists and contains your Connection class

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Read_clicked();     // Show all records
    void on_pushButton_Create_clicked();   // Insert new record
    void on_pushButton_Update_clicked();   // Update existing record
    void on_pushButton_Delete_clicked();   // Delete record by ID
    void on_tableView_clicked(const QModelIndex &index); // Select record from table

private:
    Ui::MainWindow *ui;
    Connection *connection;
    QSqlTableModel *model;
    int currentId;
    void refreshTable();
    void clearInputFields();
};
#endif // MAINWINDOW_H
