#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QTableWidget>
#include <QCalendarWidget>
#include <QSet>
#include <QDate>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QGraphicsEffect>
#include <QPainter>
#include <QLinearGradient>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QPrinter>
#include <QPainter>

#include <QSerialPort>
#include <QSerialPortInfo>

#include "hologrambar.h" // Include the separate hologrambar header

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

    // Sorting slots
    void sortDateAscending();
    void sortDateDescending();
    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

    // PDF export
    void exportToPdf();

    // Statistics slots
    void calculateAttendanceStats();
    void showAttendanceStatsDialog();
    void exportStatsTableToPdf(QTableWidget *table);
    void on_pushButton_MatchStats_clicked();
    void filterByTypeMatch();

    // Calendar slots
    void on_pushButton_Calendar_clicked();
    void onCalendarClicked(const QDate &date);

    // Simulation slots
    void on_pushButton_Simulate_clicked();
    void updateSimulation();




private:
    Ui::MainWindow *ui;
    Connection *connection;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    int currentId;

    // Calendar members
    QCalendarWidget *calendar;
    QSet<QDate> matchDates;

    // Simulation members
    QGraphicsScene *simulationScene;
    QGraphicsView *simulationView;
    QTimer *simulationTimer;
    QList<QGraphicsEllipseItem*> teamAPlayers;
    QList<QGraphicsEllipseItem*> teamBPlayers;
    QGraphicsEllipseItem *ball;
    int simulationStep;
    int teamAScore;
    int teamBScore;
    QGraphicsTextItem *scoreDisplay;
    QElapsedTimer formationReleaseTimer;
    QGraphicsTextItem *timerDisplay;
    int elapsedSeconds;
    bool isHalfTimePause;
    int pauseCounter;
    int targetTeamAScore;
    int targetTeamBScore;
    QList<int> teamAScoreTimes;
    QList<int> teamBScoreTimes;
    QGraphicsItem* currentBallHolder = nullptr;

    // Private methods
    void refreshTable();
    void clearInputFields();
    void showCalendarDialog();
    void loadMatchDates();
    void highlightMatchDates();
    void refreshCalendar();
    void showMatchSimulation(int expectedTeamAScore, int expectedTeamBScore);
    void placePlayersInFormation();

    // Simulation methods
    void movePlayersRealistically();
    void moveBallRealistically();
    void checkForGoals();
    void initializePlayersInFormation();
    void setupGame();
    void exportHologramStatsToPdf(QGraphicsScene *scene);

    QSerialPort *arduino;
    void setupArduinoConnection();
    void sendStadiumCapacityToArduino(int spectatorCount);
    void resetStadiumBarrier();
};

#endif // MAINWINDOW_H
