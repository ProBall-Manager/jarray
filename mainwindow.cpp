#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlDatabase>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentId(-1)
{
    ui->setupUi(this);

    // Create a connection object
    connection = new Connection();

    // Initialize the database connection
    if (!connection->createconnect()) {
        QMessageBox::critical(this, "Database Error", "Cannot establish database connection!");
        return;
    }

    // Set up the model for the table view
    model = new QSqlTableModel(this, connection->getConnection());
    model->setTable("MATCHES"); // Use your MATCHES table

    // Set edit strategy
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // Set column headers - adjusted to match MATCHES table structure
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Date");
    model->setHeaderData(2, Qt::Horizontal, "Lieu");
    model->setHeaderData(3, Qt::Horizontal, "Status");

    // Connect the table view to the model
    ui->tableView->setModel(model);

    // Set selection behavior to select rows
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    // We don't load data automatically - user will click Read button instead
}

MainWindow::~MainWindow()
{
    delete model;
    delete connection;
    delete ui;
}

void MainWindow::refreshTable()
{
    model->select();

    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Database Error",
                             "Failed to load data: " + model->lastError().text());
    }
}

void MainWindow::on_pushButton_Read_clicked()
{
    // Refresh the table to show all records
    refreshTable();
    clearInputFields();
}

void MainWindow::on_pushButton_Create_clicked()
{
    // Get data from input fields
    QString dateMatch = ui->lineEdit_Column1->text();
    QString lieu = ui->lineEdit_Column2->text();
    QString status = ui->lineEdit_Column3->text();

    // Validate inputs
    if (dateMatch.isEmpty() || lieu.isEmpty() || status.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "All fields must be filled");
        return;
    }

    // Using direct SQL to insert the record
    QSqlQuery query(connection->getConnection());
    query.prepare("INSERT INTO MATCHES (DATEMATCH, LIEU, STATUS) VALUES (?, ?, ?)");

    // Convert the date string to QDateTime and then to the database format
    QDateTime dateTime = QDateTime::fromString(dateMatch, "yyyy-MM-dd HH:mm");
    query.bindValue(0, dateTime);
    query.bindValue(1, lieu);
    query.bindValue(2, status);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Match created successfully");
        refreshTable(); // Refresh to show the new data
        clearInputFields();
    } else {
        QMessageBox::critical(this, "Database Error",
                              "Failed to create match: " + query.lastError().text());
    }
}

void MainWindow::on_pushButton_Update_clicked()
{
    // Get the currently selected row
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Selection Error", "Please select a match to update");
        return;
    }

    // Get the ID from the selected row (we'll keep this ID)
    QModelIndex idIndex = model->index(currentIndex.row(), 0);
    int id = model->data(idIndex).toInt();

    // Get data from input fields
    QString dateMatch = ui->lineEdit_Column1->text();
    QString lieu = ui->lineEdit_Column2->text();
    QString status = ui->lineEdit_Column3->text();

    // Validate inputs
    if (dateMatch.isEmpty() || lieu.isEmpty() || status.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "All fields must be filled");
        return;
    }

    // Update the record using the original ID
    QSqlQuery query(connection->getConnection());
    query.prepare("UPDATE MATCHES SET DATEMATCH=?, LIEU=?, STATUS=? WHERE IDMATCH=?");

    // Convert the date string to QDateTime and then to the database format
    QDateTime dateTime = QDateTime::fromString(dateMatch, "yyyy-MM-dd HH:mm");
    if (!dateTime.isValid()) {
        // If the full datetime format fails, try just the date
        dateTime = QDateTime::fromString(dateMatch, "yyyy-MM-dd");
        if (dateTime.isValid()) {
            // If it's just a date, set the time to midnight
            dateTime.setTime(QTime(0, 0));
        }
    }

    query.bindValue(0, dateTime);
    query.bindValue(1, lieu);
    query.bindValue(2, status);
    query.bindValue(3, id);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Match updated successfully");
        refreshTable(); // Refresh to show the updated data
        clearInputFields();
    } else {
        QMessageBox::critical(this, "Database Error",
                              "Failed to update match: " + query.lastError().text());
    }
}

void MainWindow::on_pushButton_Delete_clicked()
{
    // Get the currently selected row
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Selection Error", "Please select a match to delete");
        return;
    }

    // Get the ID from the selected row
    QModelIndex idIndex = model->index(currentIndex.row(), 0);
    int id = model->data(idIndex).toInt();

    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Deletion",
                                                              "Are you sure you want to delete this match?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Using direct SQL to delete the record
        QSqlQuery query(connection->getConnection());
        query.prepare("DELETE FROM MATCHES WHERE IDMATCH=?");
        query.bindValue(0, id);

        if (query.exec()) {
            QMessageBox::information(this, "Success", "Match deleted successfully");
            refreshTable(); // Refresh to show the data after deletion
            clearInputFields();
        } else {
            QMessageBox::critical(this, "Database Error",
                                  "Failed to delete match: " + query.lastError().text());
        }
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    // Get the row of the clicked item
    int row = index.row();

    // Get data from the selected row
    QSqlRecord record = model->record(row);

    // Update input fields with the selected record's data
    ui->lineEdit_ID_2->setText(record.value("IDMATCH").toString());

    // Format the date for display with time
    QDateTime dateTime = record.value("DATEMATCH").toDateTime();
    QString formattedDate = dateTime.toString("yyyy-MM-dd HH:mm");
    ui->lineEdit_Column1->setText(formattedDate);

    ui->lineEdit_Column2->setText(record.value("LIEU").toString());
    ui->lineEdit_Column3->setText(record.value("STATUS").toString());
}

void MainWindow::clearInputFields()
{
    ui->lineEdit_ID_2->clear();
    ui->lineEdit_Column1->clear();
    ui->lineEdit_Column2->clear();
    ui->lineEdit_Column3->clear();
}
