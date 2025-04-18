#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSortFilterProxyModel>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextTable>
#include <QTextCursor>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QCalendarWidget>
#include <QElapsedTimer>
#include <QGraphicsSceneHoverEvent>
#include "hologrambar.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentId(-1),
    calendar(nullptr)
{
    ui->setupUi(this);

    // Create a connection object
    connection = new Connection();
    connect(ui->lineEdit_SearchTypeMatch, &QLineEdit::textChanged, this, &MainWindow::filterByTypeMatch);


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

    // Set column headers - adjusted to match MATCHES table structure including new columns
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Date");
    model->setHeaderData(2, Qt::Horizontal, "Lieu");
    model->setHeaderData(3, Qt::Horizontal, "Status");
    model->setHeaderData(4, Qt::Horizontal, "Score");
    model->setHeaderData(5, Qt::Horizontal, "Type");
    model->setHeaderData(6, Qt::Horizontal, "Spectateurs");

    // Create a proxy model for sorting
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    // Connect the table view to the proxy model instead of directly to the model
    ui->tableView->setModel(proxyModel);

    // Enable sorting on the table view
    ui->tableView->setSortingEnabled(true);

    // Set selection behavior to select rows
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Connect sort indicator changed signal
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged,
            this, &MainWindow::onSortIndicatorChanged);

    // Connect sorting buttons
    connect(ui->pushButton_SortDateAsc, &QPushButton::clicked, this, &MainWindow::sortDateAscending);
    connect(ui->pushButton_SortDateDesc, &QPushButton::clicked, this, &MainWindow::sortDateDescending);

    // Connect PDF export button
    connect(ui->pushButton_ExportPDF, &QPushButton::clicked, this, &MainWindow::exportToPdf);

    // Connect Calendar button
    connect(ui->pushButton_Calendar, &QPushButton::clicked, this, &MainWindow::on_pushButton_Calendar_clicked);

    // We don't load data automatically - user will click Read button instead
}

MainWindow::~MainWindow()
{
    if (calendar) {
        delete calendar;
    }
    delete proxyModel;
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
void MainWindow::filterByTypeMatch()
{
    QString filterText = ui->lineEdit_SearchTypeMatch->text();
    if (filterText.isEmpty()) {
        proxyModel->setFilterFixedString(""); // Clears the filter

    } else {
        // Assuming the "TYPEMATCH" column is column index 5
        proxyModel->setFilterKeyColumn(5);
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setFilterFixedString(filterText);
    }
}
void MainWindow::on_pushButton_Simulate_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Selection Error", "Please select a match to simulate");
        return;
    }

    // Get the score from column 4 (the Score column)
    int scoreColumn = 4;
    int sourceRow = proxyModel->mapToSource(currentIndex).row();
    QString scoreString = model->data(model->index(sourceRow, scoreColumn)).toString();

    // Split the score string "2-2" into home and away scores
    QStringList scores = scoreString.split("-");
    int homeScore = 0;
    int awayScore = 0;

    if (scores.size() == 2) {
        homeScore = scores[0].toInt();
        awayScore = scores[1].toInt();
    }

    qDebug() << "Home Score:" << homeScore << "Away Score:" << awayScore;

    // Show the match simulation with the parsed scores
    showMatchSimulation(homeScore, awayScore);
}
// Replace your existing showMatchSimulation() method with this one
void MainWindow::showMatchSimulation(int expectedTeamAScore, int expectedTeamBScore)
{
    // Use the expected scores from the match clicked in the table
    targetTeamAScore = expectedTeamAScore;
    targetTeamBScore = expectedTeamBScore;

    // Create a dialog for the simulation
    QDialog simulationDialog(this);
    simulationDialog.setWindowTitle("Match Simulation");
    simulationDialog.setMinimumSize(800, 600);

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(&simulationDialog);

    // Create graphics scene and view
    simulationScene = new QGraphicsScene();
    simulationView = new QGraphicsView(simulationScene);
    simulationView->setRenderHint(QPainter::Antialiasing);
    simulationView->setMinimumSize(780, 500);

    // Add the view to the layout
    layout->addWidget(simulationView);

    // Create field
    QGraphicsRectItem *field = new QGraphicsRectItem(0, 0, 700, 450);
    field->setBrush(QBrush(QColor(34, 139, 34))); // Forest green
    field->setPen(QPen(Qt::white, 2));
    simulationScene->addItem(field);

    // Add center circle
    QGraphicsEllipseItem *centerCircle = new QGraphicsEllipseItem(300, 175, 100, 100);
    centerCircle->setPen(QPen(Qt::white, 2));
    centerCircle->setBrush(Qt::NoBrush);
    simulationScene->addItem(centerCircle);

    // Add center line
    QGraphicsLineItem *centerLine = new QGraphicsLineItem(350, 0, 350, 450);
    centerLine->setPen(QPen(Qt::white, 2));
    simulationScene->addItem(centerLine);

    // Add goal areas
    QGraphicsRectItem *goalAreaA = new QGraphicsRectItem(0, 175, 50, 100);
    goalAreaA->setPen(QPen(Qt::white, 2));
    goalAreaA->setBrush(Qt::NoBrush);
    simulationScene->addItem(goalAreaA);

    QGraphicsRectItem *goalAreaB = new QGraphicsRectItem(650, 175, 50, 100);
    goalAreaB->setPen(QPen(Qt::white, 2));
    goalAreaB->setBrush(Qt::NoBrush);
    simulationScene->addItem(goalAreaB);

    // Add penalty areas
    QGraphicsRectItem *penaltyAreaA = new QGraphicsRectItem(0, 125, 100, 200);
    penaltyAreaA->setPen(QPen(Qt::white, 2));
    penaltyAreaA->setBrush(Qt::NoBrush);
    simulationScene->addItem(penaltyAreaA);

    QGraphicsRectItem *penaltyAreaB = new QGraphicsRectItem(600, 125, 100, 200);
    penaltyAreaB->setPen(QPen(Qt::white, 2));
    penaltyAreaB->setBrush(Qt::NoBrush);
    simulationScene->addItem(penaltyAreaB);

    // Create score display
    teamAScore = 0;
    teamBScore = 0;
    scoreDisplay = new QGraphicsTextItem(QString("Score: %1 - %2").arg(teamAScore).arg(teamBScore));
    scoreDisplay->setPos(300, 460);
    scoreDisplay->setFont(QFont("Arial", 16, QFont::Bold));
    simulationScene->addItem(scoreDisplay);

    // Display expected final score
    QGraphicsTextItem *expectedScoreDisplay = new QGraphicsTextItem(QString("Expected Result: %1 - %2").arg(targetTeamAScore).arg(targetTeamBScore));
    expectedScoreDisplay->setPos(50, 460);
    expectedScoreDisplay->setFont(QFont("Arial", 12));
    simulationScene->addItem(expectedScoreDisplay);

    // Create match timer display
    elapsedSeconds = 0;
    isHalfTimePause = false;
    pauseCounter = 0;
    timerDisplay = new QGraphicsTextItem(QString("Time: %1'").arg(elapsedSeconds));
    timerDisplay->setPos(300, 490);
    timerDisplay->setFont(QFont("Arial", 14, QFont::Bold));
    simulationScene->addItem(timerDisplay);

    // Plan goal times for team A
    teamAScoreTimes.clear();
    for (int i = 0; i < targetTeamAScore; i++) {
        // Distribute goals throughout the match, some in first half, some in second
        int goalTime;
        if (i < targetTeamAScore / 2) {
            // First half goals (1-45 seconds)
            goalTime = QRandomGenerator::global()->bounded(5, 44);
        } else {
            // Second half goals (46-90 seconds)
            goalTime = QRandomGenerator::global()->bounded(46, 89);
        }
        teamAScoreTimes.append(goalTime);
    }
    // Sort goal times in ascending order
    std::sort(teamAScoreTimes.begin(), teamAScoreTimes.end());

    // Plan goal times for team B
    teamBScoreTimes.clear();
    for (int i = 0; i < targetTeamBScore; i++) {
        // Distribute goals throughout the match, some in first half, some in second
        int goalTime;
        if (i < targetTeamBScore / 2) {
            // First half goals (1-45 seconds)
            goalTime = QRandomGenerator::global()->bounded(5, 44);
        } else {
            // Second half goals (46-90 seconds)
            goalTime = QRandomGenerator::global()->bounded(46, 89);
        }
        teamBScoreTimes.append(goalTime);
    }
    // Sort goal times in ascending order
    std::sort(teamBScoreTimes.begin(), teamBScoreTimes.end());

    // Create players for team A (red)
    for (int i = 0; i < 11; i++) {
        QGraphicsEllipseItem *player = new QGraphicsEllipseItem(0, 0, 20, 20);
        player->setBrush(QBrush(Qt::red));
        player->setPen(QPen(Qt::black, 1));

        // Distribute players on the left side of the field
        qreal x = QRandomGenerator::global()->bounded(20, 320);
        qreal y = QRandomGenerator::global()->bounded(20, 430);
        player->setPos(x, y);

        teamAPlayers.append(player);
        simulationScene->addItem(player);
    }

    // Create players for team B (blue)
    for (int i = 0; i < 11; i++) {
        QGraphicsEllipseItem *player = new QGraphicsEllipseItem(0, 0, 20, 20);
        player->setBrush(QBrush(Qt::blue));
        player->setPen(QPen(Qt::black, 1));

        // Distribute players on the right side of the field
        qreal x = QRandomGenerator::global()->bounded(380, 680);
        qreal y = QRandomGenerator::global()->bounded(20, 430);
        player->setPos(x, y);

        teamBPlayers.append(player);
        simulationScene->addItem(player);
    }

    // Create ball
    ball = new QGraphicsEllipseItem(0, 0, 15, 15);
    ball->setBrush(QBrush(Qt::white));
    ball->setPen(QPen(Qt::black, 1));
    ball->setPos(350, 225); // Center of the field
    simulationScene->addItem(ball);

    // Create control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *startButton = new QPushButton("Start Simulation");
    QPushButton *stopButton = new QPushButton("Stop Simulation");
    QPushButton *resetButton = new QPushButton("Reset");
    QPushButton *closeButton = new QPushButton("Close");

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    // Create simulation timer
    simulationTimer = new QTimer(&simulationDialog);
    simulationStep = 0;

    // Connect timer to update function
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::updateSimulation);

    // Connect buttons
    connect(startButton, &QPushButton::clicked, [this]() {
        simulationTimer->start(100); // Update every 100ms
    });

    connect(stopButton, &QPushButton::clicked, [this]() {
        simulationTimer->stop();
    });

    connect(resetButton, &QPushButton::clicked, [this, expectedScoreDisplay]() {
        simulationTimer->stop();

        // Reset scores but keep the expected scores the same
        teamAScore = 0;
        teamBScore = 0;
        scoreDisplay->setPlainText(QString("Score: %1 - %2").arg(teamAScore).arg(teamBScore));

        // Reset timer
        elapsedSeconds = 0;
        isHalfTimePause = false;
        pauseCounter = 0;
        timerDisplay->setPlainText(QString("Time: %1'").arg(elapsedSeconds));

        // Replan goal times (keeping the same number of goals)
        teamAScoreTimes.clear();
        for (int i = 0; i < targetTeamAScore; i++) {
            int goalTime;
            if (i < targetTeamAScore / 2) {
                goalTime = QRandomGenerator::global()->bounded(5, 44);
            } else {
                goalTime = QRandomGenerator::global()->bounded(46, 89);
            }
            teamAScoreTimes.append(goalTime);
        }
        std::sort(teamAScoreTimes.begin(), teamAScoreTimes.end());

        teamBScoreTimes.clear();
        for (int i = 0; i < targetTeamBScore; i++) {
            int goalTime;
            if (i < targetTeamBScore / 2) {
                goalTime = QRandomGenerator::global()->bounded(5, 44);
            } else {
                goalTime = QRandomGenerator::global()->bounded(46, 89);
            }
            teamBScoreTimes.append(goalTime);
        }
        std::sort(teamBScoreTimes.begin(), teamBScoreTimes.end());

        // Reset player positions
        for (int i = 0; i < teamAPlayers.size(); i++) {
            qreal x = QRandomGenerator::global()->bounded(20, 320);
            qreal y = QRandomGenerator::global()->bounded(20, 430);
            teamAPlayers[i]->setPos(x, y);
        }

        for (int i = 0; i < teamBPlayers.size(); i++) {
            qreal x = QRandomGenerator::global()->bounded(380, 680);
            qreal y = QRandomGenerator::global()->bounded(20, 430);
            teamBPlayers[i]->setPos(x, y);
        }

        // Reset ball position
        ball->setPos(350, 225);

        simulationStep = 0;
    });

    connect(closeButton, &QPushButton::clicked, &simulationDialog, &QDialog::accept);

    // Show the dialog
    simulationDialog.exec();

    // Clean up
    simulationTimer->stop();
    delete simulationTimer;
    simulationTimer = nullptr;

    teamAPlayers.clear();
    teamBPlayers.clear();
    simulationScene = nullptr;
    simulationView = nullptr;
}

// Now you need to call this function from where you click on the match
// For example, in your table click handler:

// Replace your existing updateSimulation() method with this one
void MainWindow::updateSimulation()
{
    simulationStep++;

    // Handle match timer - one second of real time equals one second of match time
    if (simulationStep % 10 == 0 && !isHalfTimePause) {
        elapsedSeconds++;
        timerDisplay->setPlainText(QString("Time: %1'").arg(elapsedSeconds));
    }

    // Check for full time (90 seconds)
    if (elapsedSeconds >= 90) {
        // Match finished
        simulationTimer->stop();
        QMessageBox::information(nullptr, "Match Complete",
                                 QString("Full Time! Final Score: %1 - %2").arg(teamAScore).arg(teamBScore));
        return;
    }

    // Check for half-time (45 seconds)
    if (elapsedSeconds == 45 && !isHalfTimePause) {
        isHalfTimePause = true;
        timerDisplay->setPlainText(QString("Half Time"));
    }

    if (isHalfTimePause) {
        // Half-time pause (3 seconds real time = 30 timer ticks at 100ms)
        pauseCounter++;

        if (pauseCounter >= 30) {
            isHalfTimePause = false;
            pauseCounter = 0;
            elapsedSeconds = 46; // Start second half
            timerDisplay->setPlainText(QString("Time: %1'").arg(elapsedSeconds));

            // Reset player positions for second half
            for (int i = 0; i < teamAPlayers.size(); i++) {
                qreal x = QRandomGenerator::global()->bounded(20, 320);
                qreal y = QRandomGenerator::global()->bounded(20, 430);
                teamAPlayers[i]->setPos(x, y);
            }

            for (int i = 0; i < teamBPlayers.size(); i++) {
                qreal x = QRandomGenerator::global()->bounded(380, 680);
                qreal y = QRandomGenerator::global()->bounded(20, 430);
                teamBPlayers[i]->setPos(x, y);
            }

            // Reset ball position
            ball->setPos(350, 225);
        }

        // Don't update the rest of the simulation during half-time
        return;
    }

    // Check if it's time for a team A goal
    if (!teamAScoreTimes.isEmpty() && elapsedSeconds == teamAScoreTimes.first()) {
        // Score a goal for team A
        teamAScore++;
        scoreDisplay->setPlainText(QString("Score: %1 - %2").arg(teamAScore).arg(teamBScore));

        // Create a goal celebration animation
        QGraphicsTextItem *goalText = new QGraphicsTextItem("GOAL!");
        goalText->setFont(QFont("Arial", 24, QFont::Bold));
        goalText->setDefaultTextColor(Qt::red);
        goalText->setPos(300, 200);
        simulationScene->addItem(goalText);

        // Move ball to team B's goal
        ball->setPos(690, 225);

        // Create a timer to remove the goal text after 1 second
        QTimer::singleShot(1000, [this, goalText]() {
            simulationScene->removeItem(goalText);
            delete goalText;

            // Reset ball position
            ball->setPos(350, 225);
        });

        // Remove this goal time from the list
        teamAScoreTimes.removeFirst();
    }

    // Check if it's time for a team B goal
    if (!teamBScoreTimes.isEmpty() && elapsedSeconds == teamBScoreTimes.first()) {
        // Score a goal for team B
        teamBScore++;
        scoreDisplay->setPlainText(QString("Score: %1 - %2").arg(teamAScore).arg(teamBScore));

        // Create a goal celebration animation
        QGraphicsTextItem *goalText = new QGraphicsTextItem("GOAL!");
        goalText->setFont(QFont("Arial", 24, QFont::Bold));
        goalText->setDefaultTextColor(Qt::blue);
        goalText->setPos(300, 200);
        simulationScene->addItem(goalText);

        // Move ball to team A's goal
        ball->setPos(10, 225);

        // Create a timer to remove the goal text after 1 second
        QTimer::singleShot(1000, [this, goalText]() {
            simulationScene->removeItem(goalText);
            delete goalText;

            // Reset ball position
            ball->setPos(350, 225);
        });

        // Remove this goal time from the list
        teamBScoreTimes.removeFirst();
    }

    // Move players with more realistic football behavior
    movePlayersRealistically();

    // Move ball with possession-based realism
    moveBallRealistically();
}

// Add these new methods to your implementation file
void MainWindow::movePlayersRealistically()
{
    QPointF ballPos = ball->pos();
    bool teamAHasPossession = ballPos.x() < 350;
    const int SEPARATION_DISTANCE = 30;
    const qreal MAX_SPEED = 8.0;

    // Define player roles - 0: goalkeeper, 1-4: defenders, 5-7: midfielders, 8-10: forwards
    QList<int> roles;
    for (int i = 0; i < 11; i++) {
        roles.append(i <= 0 ? 0 : (i <= 4 ? 1 : (i <= 7 ? 2 : 3))); // 0=GK, 1=DEF, 2=MID, 3=FWD
    }

    // Define formations
    const QList<QPointF> teamAFormation = {
        QPointF(50, 225),  // Goalkeeper
        QPointF(120, 100),  // Defender 1
        QPointF(120, 175),  // Defender 2
        QPointF(120, 275),  // Defender 3
        QPointF(120, 350),  // Defender 4
        QPointF(200, 150),  // Midfielder 1
        QPointF(200, 225),  // Midfielder 2
        QPointF(200, 300),  // Midfielder 3
        QPointF(280, 125),  // Forward 1
        QPointF(280, 225),  // Forward 2
        QPointF(280, 325)   // Forward 3
    };

    const QList<QPointF> teamBFormation = {
        QPointF(630, 225),  // Goalkeeper
        QPointF(560, 100),  // Defender 1
        QPointF(560, 175),  // Defender 2
        QPointF(560, 275),  // Defender 3
        QPointF(560, 350),  // Defender 4
        QPointF(480, 150),  // Midfielder 1
        QPointF(480, 225),  // Midfielder 2
        QPointF(480, 300),  // Midfielder 3
        QPointF(400, 125),  // Forward 1
        QPointF(400, 225),  // Forward 2
        QPointF(400, 325)   // Forward 3
    };

    // Move Team A players
    for (int i = 0; i < teamAPlayers.size(); ++i) {
        auto player = teamAPlayers[i];
        QPointF playerPos = player->pos();
        QPointF baseFormationPos = teamAFormation[i];
        QPointF targetPos;

        int playerRole = roles[i];
        bool isGoalkeeper = playerRole == 0;
        bool isDefender = playerRole == 1;
        bool isMidfielder = playerRole == 2;
        bool isForward = playerRole == 3;

        // Calculate how far player should move forward/backward based on ball position
        qreal forwardShift = 0;
        if (teamAHasPossession) {
            // When Team A has possession, move players forward
            if (isGoalkeeper) forwardShift = 10;
            else if (isDefender) forwardShift = 30;
            else if (isMidfielder) forwardShift = 50;
            else if (isForward) forwardShift = 70;
        } else {
            // When Team B has possession, move players back
            if (isGoalkeeper) forwardShift = -5;
            else if (isDefender) forwardShift = -15;
            else if (isMidfielder) forwardShift = -30;
            else if (isForward) forwardShift = -40;
        }

        // Calculate sideways shift to follow the ball
        qreal ballYPos = ballPos.y();
        qreal sideShift = (ballYPos - 225) * 0.2; // Slight shift toward ball's vertical position

        // Apply formation position with adjustments
        targetPos = baseFormationPos + QPointF(forwardShift, sideShift);

        // Only forwards and midfielders chase the ball closely, with lower probability for defenders
        bool shouldChaseBall = false;
        qreal distanceToBall = QLineF(playerPos, ballPos).length();

        if (distanceToBall < 100) {
            int chaseChance = 0;
            if (isForward) chaseChance = 80;
            else if (isMidfielder) chaseChance = 60;
            else if (isDefender) chaseChance = 30;
            else chaseChance = 0; // Goalkeeper doesn't chase

            shouldChaseBall = QRandomGenerator::global()->bounded(100) < chaseChance;
        }

        // If player should chase ball and ball is in their zone
        if (shouldChaseBall && ((teamAHasPossession && playerPos.x() < ballPos.x()) ||
                                (!teamAHasPossession && playerPos.x() < 350))) {
            targetPos = ballPos + QPointF(QRandomGenerator::global()->bounded(-20, 20),
                                          QRandomGenerator::global()->bounded(-20, 20));
        }

        // Calculate separation from other players
        QPointF separation(0, 0);
        for (auto otherPlayer : teamAPlayers + teamBPlayers) {
            if (otherPlayer == player) continue;
            QPointF diff = playerPos - otherPlayer->pos();
            qreal distance = QLineF(playerPos, otherPlayer->pos()).length();
            if (distance < SEPARATION_DISTANCE && distance > 0) {
                separation += diff / distance * 2; // Apply separation force
            }
        }

        // Move towards target position
        QLineF moveLine(playerPos, targetPos);
        qreal dx = 0, dy = 0;
        if (moveLine.length() > 1) {
            qreal moveSpeed = isGoalkeeper ? 2.0 : (isDefender ? 3.0 : (isMidfielder ? 4.0 : 5.0));
            moveLine.setLength(moveSpeed);
            dx = moveLine.dx();
            dy = moveLine.dy();
        }

        // Apply separation force
        dx += separation.x();
        dy += separation.y();

        // Limit the speed
        qreal speed = qSqrt(dx*dx + dy*dy);
        if (speed > MAX_SPEED) {
            dx = (dx / speed) * MAX_SPEED;
            dy = (dy / speed) * MAX_SPEED;
        }

        QPointF newPos = playerPos + QPointF(dx, dy);

        // Keep players within bounds
        newPos.setX(qBound(20.0, newPos.x(), 350.0));
        newPos.setY(qBound(20.0, newPos.y(), 430.0));

        player->setPos(newPos);
    }

    // Move Team B players (similar logic)
    for (int i = 0; i < teamBPlayers.size(); ++i) {
        auto player = teamBPlayers[i];
        QPointF playerPos = player->pos();
        QPointF baseFormationPos = teamBFormation[i];
        QPointF targetPos;

        int playerRole = roles[i];
        bool isGoalkeeper = playerRole == 0;
        bool isDefender = playerRole == 1;
        bool isMidfielder = playerRole == 2;
        bool isForward = playerRole == 3;

        // Calculate how far player should move forward/backward based on ball position
        qreal forwardShift = 0;
        if (!teamAHasPossession) {
            // When Team B has possession, move players forward
            if (isGoalkeeper) forwardShift = -10;
            else if (isDefender) forwardShift = -30;
            else if (isMidfielder) forwardShift = -50;
            else if (isForward) forwardShift = -70;
        } else {
            // When Team A has possession, move players back
            if (isGoalkeeper) forwardShift = 5;
            else if (isDefender) forwardShift = 15;
            else if (isMidfielder) forwardShift = 30;
            else if (isForward) forwardShift = 40;
        }

        // Calculate sideways shift to follow the ball
        qreal ballYPos = ballPos.y();
        qreal sideShift = (ballYPos - 225) * 0.2; // Slight shift toward ball's vertical position

        // Apply formation position with adjustments
        targetPos = baseFormationPos + QPointF(forwardShift, sideShift);

        // Only forwards and midfielders chase the ball closely, with lower probability for defenders
        bool shouldChaseBall = false;
        qreal distanceToBall = QLineF(playerPos, ballPos).length();

        if (distanceToBall < 100) {
            int chaseChance = 0;
            if (isForward) chaseChance = 80;
            else if (isMidfielder) chaseChance = 60;
            else if (isDefender) chaseChance = 30;
            else chaseChance = 0; // Goalkeeper doesn't chase

            shouldChaseBall = QRandomGenerator::global()->bounded(100) < chaseChance;
        }

        // If player should chase ball and ball is in their zone
        if (shouldChaseBall && ((!teamAHasPossession && playerPos.x() > ballPos.x()) ||
                                (teamAHasPossession && playerPos.x() > 350))) {
            targetPos = ballPos + QPointF(QRandomGenerator::global()->bounded(-20, 20),
                                          QRandomGenerator::global()->bounded(-20, 20));
        }

        // Calculate separation from other players
        QPointF separation(0, 0);
        for (auto otherPlayer : teamAPlayers + teamBPlayers) {
            if (otherPlayer == player) continue;
            QPointF diff = playerPos - otherPlayer->pos();
            qreal distance = QLineF(playerPos, otherPlayer->pos()).length();
            if (distance < SEPARATION_DISTANCE && distance > 0) {
                separation += diff / distance * 2; // Apply separation force
            }
        }

        // Move towards target position
        QLineF moveLine(playerPos, targetPos);
        qreal dx = 0, dy = 0;
        if (moveLine.length() > 1) {
            qreal moveSpeed = isGoalkeeper ? 2.0 : (isDefender ? 3.0 : (isMidfielder ? 4.0 : 5.0));
            moveLine.setLength(moveSpeed);
            dx = moveLine.dx();
            dy = moveLine.dy();
        }

        // Apply separation force
        dx += separation.x();
        dy += separation.y();

        // Limit the speed
        qreal speed = qSqrt(dx*dx + dy*dy);
        if (speed > MAX_SPEED) {
            dx = (dx / speed) * MAX_SPEED;
            dy = (dy / speed) * MAX_SPEED;
        }

        QPointF newPos = playerPos + QPointF(dx, dy);

        // Keep players within bounds
        newPos.setX(qBound(350.0, newPos.x(), 660.0));
        newPos.setY(qBound(20.0, newPos.y(), 430.0));

        player->setPos(newPos);
    }
}

void MainWindow::initializePlayersInFormation()
{
    // Define formations - same as in movePlayersRealistically
    const QList<QPointF> teamAFormation = {
        QPointF(50, 225),  // Goalkeeper
        QPointF(120, 100),  // Defender 1
        QPointF(120, 175),  // Defender 2
        QPointF(120, 275),  // Defender 3
        QPointF(120, 350),  // Defender 4
        QPointF(200, 150),  // Midfielder 1
        QPointF(200, 225),  // Midfielder 2
        QPointF(200, 300),  // Midfielder 3
        QPointF(280, 125),  // Forward 1
        QPointF(280, 225),  // Forward 2
        QPointF(280, 325)   // Forward 3
    };

    const QList<QPointF> teamBFormation = {
        QPointF(630, 225),  // Goalkeeper
        QPointF(560, 100),  // Defender 1
        QPointF(560, 175),  // Defender 2
        QPointF(560, 275),  // Defender 3
        QPointF(560, 350),  // Defender 4
        QPointF(480, 150),  // Midfielder 1
        QPointF(480, 225),  // Midfielder 2
        QPointF(480, 300),  // Midfielder 3
        QPointF(400, 125),  // Forward 1
        QPointF(400, 225),  // Forward 2
        QPointF(400, 325)   // Forward 3
    };

    // Place each player immediately in their formation position
    for (int i = 0; i < teamAPlayers.size(); ++i) {
        teamAPlayers[i]->setPos(teamAFormation[i]);
    }

    for (int i = 0; i < teamBPlayers.size(); ++i) {
        teamBPlayers[i]->setPos(teamBFormation[i]);
    }

    // Place ball in center
    ball->setPos(350, 225);
}

void MainWindow::moveBallRealistically()
{
    QPointF ballPos = ball->pos();
    const qreal possessionDistance = 30.0;

    // Check if current holder is still close enough to the ball
    if (!currentBallHolder || QLineF(ballPos, currentBallHolder->pos()).length() > possessionDistance) {
        // Find new closest player
        qreal minDistance = 1000;
        QGraphicsItem* closest = nullptr;

        for (auto player : teamAPlayers + teamBPlayers) {
            qreal dist = QLineF(ballPos, player->pos()).length();
            if (dist < minDistance) {
                minDistance = dist;
                closest = player;
            }
        }

        if (minDistance < possessionDistance) {
            currentBallHolder = closest;
        } else {
            currentBallHolder = nullptr;
        }
    }

    // If someone has the ball, simulate passing
    if (currentBallHolder) {
        // Determine team
        bool isTeamA = teamAPlayers.contains(static_cast<QGraphicsEllipseItem*>(currentBallHolder));
        QList<QGraphicsEllipseItem*> teammates = isTeamA ? teamAPlayers : teamBPlayers;
        QList<QGraphicsEllipseItem*> opponents = isTeamA ? teamBPlayers : teamAPlayers;

        // Find nearest teammate in forward direction
        QGraphicsEllipseItem* bestPassTarget = nullptr;
        qreal bestScore = -1000;

        for (auto teammate : teammates) {
            if (teammate == currentBallHolder)
                continue;

            QPointF toTeammate = teammate->pos() - currentBallHolder->pos();
            if ((isTeamA && toTeammate.x() < 0) || (!isTeamA && toTeammate.x() > 0))
                continue; // Only forward passes

            qreal distance = QLineF(currentBallHolder->pos(), teammate->pos()).length();
            qreal score = toTeammate.x() - 0.5 * qAbs(toTeammate.y()); // prefer forward & centered

            if (score > bestScore && distance > 30 && distance < 200) {
                bestScore = score;
                bestPassTarget = teammate;
            }
        }

        // If a good target found, simulate pass
        if (bestPassTarget && QRandomGenerator::global()->bounded(100) < 15) { // 15% chance to pass
            QLineF passLine(ballPos, bestPassTarget->pos());
            passLine.setLength(12); // pass speed
            QPointF nextPos = ballPos + QPointF(passLine.dx(), passLine.dy());

            // Interception check
            for (auto opponent : opponents) {
                if (QLineF(nextPos, opponent->pos()).length() < 25) {
                    currentBallHolder = opponent;
                    ball->setPos(opponent->pos());
                    return;
                }
            }

            ball->setPos(nextPos);
            return;
        }

        // Otherwise, move ball with the player
        QPointF targetPos = currentBallHolder->pos();
        QLineF moveLine(ballPos, targetPos);
        moveLine.setLength(5);
        ball->setPos(moveLine.p2());
    } else {
        // No one has possession - ball rolls slowly toward nearest player
        QGraphicsItem* closest = nullptr;
        qreal minDist = 1000;
        for (auto player : teamAPlayers + teamBPlayers) {
            qreal dist = QLineF(ballPos, player->pos()).length();
            if (dist < minDist) {
                minDist = dist;
                closest = player;
            }
        }

        if (closest) {
            QLineF moveLine(ballPos, closest->pos());
            moveLine.setLength(4);
            ball->setPos(moveLine.p2());
        }
    }

    // Keep ball within boundaries
    QPointF newBallPos = ball->pos();
    newBallPos.setX(qMax(0.0, qMin(newBallPos.x(), 685.0)));
    newBallPos.setY(qMax(0.0, qMin(newBallPos.y(), 435.0)));
    ball->setPos(newBallPos);

    // Goal check
    if (newBallPos.x() <= 0) {
        // Team B scored
        currentBallHolder = nullptr;
        QTimer::singleShot(500, [this]() {
            ball->setPos(350, 225);
            // Reset players to formation
            initializePlayersInFormation();
        });
    } else if (newBallPos.x() >= 685) {
        // Team A scored
        currentBallHolder = nullptr;
        QTimer::singleShot(500, [this]() {
            ball->setPos(350, 225);
            // Reset players to formation
            initializePlayersInFormation();
        });
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
    QString score = ui->lineEdit_Score->text();
    QString typeMatch = ui->lineEdit_TypeMatch->text();
    QString spectateurs = ui->lineEdit_Spectateurs->text();

    // Validate inputs
    if (dateMatch.isEmpty() || lieu.isEmpty() || status.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Date, Lieu and Status fields must be filled");
        return;
    }

    // Using direct SQL to insert the record
    QSqlQuery query(connection->getConnection());
    query.prepare("INSERT INTO MATCHES (DATEMATCH, LIEU, STATUS, SCORE, TYPEMATCH, SPECTATEURS) VALUES (?, ?, ?, ?, ?, ?)");

    // Convert the date string to QDateTime and then to the database format
    QDateTime dateTime = QDateTime::fromString(dateMatch, "yyyy-MM-dd HH:mm");
    query.bindValue(0, dateTime);
    query.bindValue(1, lieu);
    query.bindValue(2, status);
    query.bindValue(3, score);
    query.bindValue(4, typeMatch);
    query.bindValue(5, spectateurs);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Match created successfully");
        refreshTable(); // Refresh to show the new data
        clearInputFields();

        // Update calendar if it's displayed
        refreshCalendar();
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

    // Need to map proxy index to source index
    QModelIndex sourceIndex = proxyModel->mapToSource(currentIndex);

    // Get the ID from the selected row (we'll keep this ID)
    QModelIndex idIndex = model->index(sourceIndex.row(), 0);
    int id = model->data(idIndex).toInt();

    // Get data from input fields
    QString dateMatch = ui->lineEdit_Column1->text();
    QString lieu = ui->lineEdit_Column2->text();
    QString status = ui->lineEdit_Column3->text();
    QString score = ui->lineEdit_Score->text();
    QString typeMatch = ui->lineEdit_TypeMatch->text();
    QString spectateurs = ui->lineEdit_Spectateurs->text();

    // Validate inputs
    if (dateMatch.isEmpty() || lieu.isEmpty() || status.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Date, Lieu and Status fields must be filled");
        return;
    }

    // Update the record using the original ID
    QSqlQuery query(connection->getConnection());
    query.prepare("UPDATE MATCHES SET DATEMATCH=?, LIEU=?, STATUS=?, SCORE=?, TYPEMATCH=?, SPECTATEURS=? WHERE IDMATCH=?");

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
    query.bindValue(3, score);
    query.bindValue(4, typeMatch);
    query.bindValue(5, spectateurs);
    query.bindValue(6, id);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Match updated successfully");
        refreshTable(); // Refresh to show the updated data
        clearInputFields();

        // Update calendar if it's displayed
        refreshCalendar();
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

    // Need to map proxy index to source index
    QModelIndex sourceIndex = proxyModel->mapToSource(currentIndex);

    // Get the ID from the selected row
    QModelIndex idIndex = model->index(sourceIndex.row(), 0);
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

            // Update calendar if it's displayed
            refreshCalendar();
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

    // Map from proxy model index to source model index
    QModelIndex sourceIndex = proxyModel->mapToSource(index);

    // Get the row of the clicked item
    int row = sourceIndex.row();

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
    ui->lineEdit_Score->setText(record.value("SCORE").toString());
    ui->lineEdit_TypeMatch->setText(record.value("TYPEMATCH").toString());
    ui->lineEdit_Spectateurs->setText(record.value("SPECTATEURS").toString());
}

void MainWindow::clearInputFields()
{
    ui->lineEdit_ID_2->clear();
    ui->lineEdit_Column1->clear();
    ui->lineEdit_Column2->clear();
    ui->lineEdit_Column3->clear();
    ui->lineEdit_Score->clear();
    ui->lineEdit_TypeMatch->clear();
    ui->lineEdit_Spectateurs->clear();
}

// Sorting methods
void MainWindow::sortDateAscending()
{
    // Sort by date column (column index 1) in ascending order
    proxyModel->sort(1, Qt::AscendingOrder);
}

void MainWindow::sortDateDescending()
{
    // Sort by date column (column index 1) in descending order
    proxyModel->sort(1, Qt::DescendingOrder);
}

void MainWindow::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    // This slot is called when the user clicks on a column header
    // You can add custom logic here if needed
    qDebug() << "Sorting by column" << logicalIndex << "in order" << order;
}

void MainWindow::exportToPdf()
{
    // Ask user for the file location to save PDF
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"),
                                                    QString(), tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty())
        return;

    // Make sure the file name has .pdf extension
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    // Create a printer object
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);

    // Create a text document to format our table
    QTextDocument doc;
    QTextCursor cursor(&doc);

    // Add a title
    QTextCharFormat titleFormat;
    titleFormat.setFontPointSize(16);
    titleFormat.setFontWeight(QFont::Bold);
    cursor.insertText("Matches Report\n\n", titleFormat);

    // Create a table with rows and columns matching our data model
    int rowCount = proxyModel->rowCount();
    int columnCount = proxyModel->columnCount();

    QTextTable *table = cursor.insertTable(rowCount + 1, columnCount);

    // Set table format
    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setCellPadding(5);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setAlignment(Qt::AlignCenter);
    table->setFormat(tableFormat);

    // Add column headers
    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setBackground(QColor(230, 230, 230));

    for (int col = 0; col < columnCount; ++col) {
        QTextTableCell cell = table->cellAt(0, col);
        cell.setFormat(headerFormat);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(proxyModel->headerData(col, Qt::Horizontal).toString());
    }

    // Add table data
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            QModelIndex idx = proxyModel->index(row, col);
            QVariant data = proxyModel->data(idx);

            // Format dates nicely if this is the date column (column 1)
            QString textData;
            if (col == 1 && data.typeId() == QMetaType::QDateTime) {

                textData = data.toDateTime().toString("yyyy-MM-dd HH:mm");
            } else {
                textData = data.toString();
            }

            QTextTableCell cell = table->cellAt(row + 1, col);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(textData);
        }
    }

    // Add timestamp at the bottom
    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n\nGenerated on: " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    // Print the document to the PDF file
    doc.print(&printer);

    QMessageBox::information(this, tr("PDF Export"), tr("PDF file exported successfully."));
}

// Method for statistics
void MainWindow::on_pushButton_MatchStats_clicked()
{
    // Call the method to calculate and display stats
    calculateAttendanceStats();
}

void MainWindow::showAttendanceStatsDialog()
{
    // Create a dialog to display the stats
    QDialog statsDialog(this);
    statsDialog.setWindowTitle("Holographic Attendance Statistics");
    statsDialog.setMinimumSize(800, 600);
    statsDialog.setStyleSheet("background-color: #0a192f;"); // Dark blue background for hologram effect

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(&statsDialog);

    // Add title
    QLabel *titleLabel = new QLabel("Holographic Attendance per Match Type", &statsDialog);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #00e5ff; margin: 10px;"); // Cyan text for hologram feel
    layout->addWidget(titleLabel);

    // Create graphics view for our hologram visualization
    QGraphicsView *graphicsView = new QGraphicsView(&statsDialog);
    QGraphicsScene *scene = new QGraphicsScene(graphicsView);
    graphicsView->setScene(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setBackgroundBrush(QBrush(QColor("#0a192f"))); // Dark blue background
    graphicsView->setFrameStyle(QFrame::NoFrame);

    // Modified SQL query to only include specific match types
    QSqlQuery query(connection->getConnection());
    query.prepare("SELECT TYPEMATCH, AVG(TO_NUMBER(SPECTATEURS)) as AverageAttendance, "
                  "COUNT(*) as MatchCount FROM MATCHES "
                  "WHERE TYPEMATCH IN ('compétitif', 'championnat', 'amicale') "
                  "AND TYPEMATCH IS NOT NULL "
                  "GROUP BY TYPEMATCH ORDER BY TYPEMATCH");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to calculate statistics: " + query.lastError().text());
        return;
    }

    // Collect data for visualization
    QStringList matchTypes;
    QVector<double> avgAttendances;
    QVector<int> matchCounts;
    double maxAttendance = 0;

    while (query.next()) {
        QString matchType = query.value("TYPEMATCH").toString();
        double avgAttendance = query.value("AverageAttendance").toDouble();
        int matchCount = query.value("MatchCount").toInt();

        matchTypes.append(matchType);
        avgAttendances.append(avgAttendance);
        matchCounts.append(matchCount);

        if (avgAttendance > maxAttendance) {
            maxAttendance = avgAttendance;
        }
    }

    // If no data was found, show a message and return
    if (matchTypes.isEmpty()) {
        QMessageBox::information(this, "No Data",
                                 "No matches found with the specified types (compétitif, championnat, amicale).");
        return;
    }

    // Calculate layout - use a grid layout for more predictable positioning
    const int MAX_ITEMS_PER_ROW = 3;
    const int MAX_DIAMETER = 200;
    const int SPACING = 50;

    // Calculate how many rows we need
    int numRows = (matchTypes.size() + MAX_ITEMS_PER_ROW - 1) / MAX_ITEMS_PER_ROW;

    // For each match type, create a hologram circle
    for (int i = 0; i < matchTypes.size(); ++i) {
        // Calculate grid position (row, column)
        int row = i / MAX_ITEMS_PER_ROW;
        int col = i % MAX_ITEMS_PER_ROW;

        // Calculate size proportional to attendance
        double sizeRatio = avgAttendances[i] / maxAttendance;
        int diameter = qMax(80, int(sizeRatio * MAX_DIAMETER));

        // Calculate position with even spacing
        int xCenter = col * (MAX_DIAMETER + SPACING) + MAX_DIAMETER/2;
        int yCenter = row * (MAX_DIAMETER + SPACING) + MAX_DIAMETER/2;

        // Create hologram bar with circle shape
        HologramBar *bar = new HologramBar(matchTypes[i], avgAttendances[i], matchCounts[i]);
        bar->setRect(xCenter - diameter/2, yCenter - diameter/2, diameter, diameter);

        // Add reflection effect
        QGraphicsEllipseItem *reflection = new QGraphicsEllipseItem(
            xCenter - diameter/4, yCenter + diameter/2 + 10,
            diameter/2, diameter/5);

        QLinearGradient reflectionGradient(0, yCenter + diameter/2, 0, yCenter + diameter/2 + diameter/5);
        reflectionGradient.setColorAt(0.0, QColor(0, 200, 255, 100));
        reflectionGradient.setColorAt(1.0, QColor(0, 100, 200, 0));
        reflection->setBrush(reflectionGradient);
        reflection->setPen(Qt::NoPen);

        scene->addItem(bar);
        scene->addItem(reflection);
    }

    // Set the scene rect to contain all items with padding
    scene->setSceneRect(scene->itemsBoundingRect().adjusted(-50, -50, 50, 50));

    // Add the graphics view to layout
    layout->addWidget(graphicsView);

    // Add export button
    QPushButton *exportButton = new QPushButton("Export to PDF", &statsDialog);
    exportButton->setStyleSheet("background-color: #003366; color: white; padding: 8px; border-radius: 5px;");
    connect(exportButton, &QPushButton::clicked, [this, scene]() {
        exportHologramStatsToPdf(scene);
    });
    layout->addWidget(exportButton);

    // Show the dialog
    statsDialog.exec();
}

void MainWindow::exportHologramStatsToPdf(QGraphicsScene *scene)
{
    // Ask user for the file location to save PDF
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Hologram Stats to PDF"),
                                                    QString(), tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty())
        return;

    // Ensure the file name has .pdf extension
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    // Set up printer for PDF export
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);

    // Use a page size that matches your desired aspect ratio
    QPageSize customSize(QSizeF(800, 600), QPageSize::Point, "Custom");
    printer.setPageSize(customSize);

    // Set minimal margins
    printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

    // Create painter for the PDF
    QPainter pdfPainter(&printer);
    pdfPainter.setRenderHint(QPainter::Antialiasing, true);
    pdfPainter.setRenderHint(QPainter::TextAntialiasing, true);
    pdfPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Get printable area
    QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);

    // Fill the background with white color
    pdfPainter.fillRect(pageRect, Qt::white);

    // Draw the title with the holographic cyan color
    QFont titleFont("Arial", 18, QFont::Bold);
    pdfPainter.setFont(titleFont);
    pdfPainter.setPen(QColor("#00e5ff"));
    pdfPainter.drawText(QRectF(0, 20, pageRect.width(), 60),
                        Qt::AlignHCenter, "Holographic Attendance per Match Type");

    // Create a separate image for the scene content only
    QImage sceneImage(pageRect.width() - 20, pageRect.height() - 150, QImage::Format_ARGB32);
    sceneImage.fill(Qt::transparent); // Make it transparent

    QPainter scenePainter(&sceneImage);
    scenePainter.setRenderHint(QPainter::Antialiasing, true);
    scenePainter.setRenderHint(QPainter::TextAntialiasing, true);

    // Calculate the content area size
    QRectF sceneRect = scene->itemsBoundingRect();

    // Scale and position to center the content
    double xScale = (sceneImage.width() / sceneRect.width());
    double yScale = (sceneImage.height() / sceneRect.height());
    double scale = qMin(xScale, yScale) * 0.9; // Scale down slightly to leave margins

    // Ensure we have enough space and adjust scaling
    if (scale < 1.0) {
        scenePainter.translate(sceneImage.width() / 2, sceneImage.height() / 2);
        scenePainter.scale(scale, scale);
        scenePainter.translate(-sceneRect.center().x(), -sceneRect.center().y());
    }

    // Only render the items (not the background)
    scene->render(&scenePainter, QRectF(), sceneRect, Qt::KeepAspectRatio);
    scenePainter.end();

    // Draw the scene image to the PDF, adjusting the position
    pdfPainter.drawImage(QPointF(10, 80), sceneImage); // Adjusted position

    // Draw the "Export to PDF" button at the bottom
    QRectF buttonRect(pageRect.width() * 0.25, pageRect.height() - 40,
                      pageRect.width() * 0.5, 30);

    // Draw button background
    pdfPainter.setPen(Qt::NoPen);
    pdfPainter.setBrush(QColor("#003366"));
    pdfPainter.drawRoundedRect(buttonRect, 5, 5);

    // Draw button text
    pdfPainter.setPen(Qt::white);
    QFont buttonFont("Arial", 10);
    pdfPainter.setFont(buttonFont);
    pdfPainter.drawText(buttonRect, Qt::AlignCenter, "Export to PDF");

    pdfPainter.end();

    // Notify user of success
    QMessageBox::information(this, tr("PDF Export"), tr("Hologram statistics exported to PDF successfully."));
}
// Update the calculateAttendanceStats method to call the hologram version
void MainWindow::calculateAttendanceStats()
{
    // Create a query to calculate average attendance per match type
    QSqlQuery query(connection->getConnection());

    // Use the correct column name from your database (SPECTATEURS)
    query.prepare("SELECT TYPEMATCH, AVG(TO_NUMBER(SPECTATEURS)) as AverageAttendance, "
                  "COUNT(*) as MatchCount FROM MATCHES GROUP BY TYPEMATCH ORDER BY TYPEMATCH");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to calculate statistics: " + query.lastError().text());
        return;
    }

    // Display the results in the hologram dialog
    showAttendanceStatsDialog();
}
// Calendar functions
void MainWindow::on_pushButton_Calendar_clicked()
{
    showCalendarDialog();
}

void MainWindow::showCalendarDialog()
{
    // Create a dialog to display the calendar
    QDialog calendarDialog(this);
    calendarDialog.setWindowTitle("Match Calendar");
    calendarDialog.setMinimumSize(400, 400);

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(&calendarDialog);

    // Add title
    QLabel *titleLabel = new QLabel("Match Schedule Calendar", &calendarDialog);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Create calendar widget
    calendar = new QCalendarWidget(&calendarDialog);
    layout->addWidget(calendar);

    // Add explanation labels
    QLabel *availableLabel = new QLabel("Green dates are available for scheduling.", &calendarDialog);
    availableLabel->setAlignment(Qt::AlignCenter);

    QLabel *unavailableLabel = new QLabel("Red dates have scheduled or played matches.", &calendarDialog);
    unavailableLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(availableLabel);
    layout->addWidget(unavailableLabel);

    // Remove weekend highlighting (to prevent Saturday and Sunday from showing in red)
    QTextCharFormat weekendFormat;
    calendar->setWeekdayTextFormat(Qt::Saturday, weekendFormat);
    calendar->setWeekdayTextFormat(Qt::Sunday, weekendFormat);

    // Load match dates and highlight them
    loadMatchDates();
    highlightMatchDates();

    // Connect date selection signal
    connect(calendar, &QCalendarWidget::clicked, this, &MainWindow::onCalendarClicked);

    // Show the dialog
    calendarDialog.exec();

    // Clean up
    delete calendar;
    calendar = nullptr;
}

void MainWindow::loadMatchDates()
{
    // Clear existing dates
    matchDates.clear();

    // Query the database for all match dates
    QSqlQuery query(connection->getConnection());
    query.prepare("SELECT DATEMATCH FROM MATCHES");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to load match dates: " + query.lastError().text());
        return;
    }

    // Populate the set with match dates
    while (query.next()) {
        QDateTime dateTime = query.value("DATEMATCH").toDateTime();
        if (dateTime.isValid()) {
            matchDates.insert(dateTime.date());
        }
    }
}

void MainWindow::highlightMatchDates()
{
    if (!calendar) return;

    // Get the first day of the month being displayed
    QDate firstDate = calendar->selectedDate().addDays(-(calendar->selectedDate().day() - 1));

    // Get the current date (for determining past vs future dates)
    QDate currentDate = QDate::currentDate();

    // Iterate through all days in the visible month
    for (int i = 0; i < 42; ++i) { // 6 weeks × 7 days = 42 possible visible cells
        QDate calDate = firstDate.addDays(i);

        // Get the text format for the date
        QTextCharFormat format = calendar->dateTextFormat(calDate);

        if (matchDates.contains(calDate)) {
            // This date has a match - highlight it in red
            format.setBackground(QColor(255, 200, 200)); // Light red
            format.setForeground(QColor(170, 0, 0)); // Dark red text
        } else if (calDate >= currentDate) {
            // Future date with no match scheduled - highlight it in green
            format.setBackground(QColor(200, 255, 200)); // Light green
            format.setForeground(QColor(0, 120, 0)); // Dark green text
        } else {
            // Past date with no match - use default format
            format = QTextCharFormat();
        }

        // Apply the format to this date
        calendar->setDateTextFormat(calDate, format);
    }
}

void MainWindow::onCalendarClicked(const QDate &date)
{
    // Get matches for the selected date
    QSqlQuery query(connection->getConnection());
    query.prepare("SELECT IDMATCH, DATEMATCH, LIEU, STATUS, SCORE, TYPEMATCH, SPECTATEURS FROM MATCHES "
                  "WHERE TRUNC(DATEMATCH) = :date ORDER BY DATEMATCH");

    // Convert QDate to the format expected by your database
    QString dateString = date.toString("yyyy-MM-dd");
    query.bindValue(":date", dateString);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to load matches for date: " + query.lastError().text());
        return;
    }

    // Check if there are any matches on this date
    if (!query.next()) {
        // No matches on this date
        if (date < QDate::currentDate()) {
            QMessageBox::information(this, "Past Date",
                                     "This is a past date with no scheduled matches.");
        } else {
            QMessageBox::information(this, "Available Date",
                                     "This date is available for scheduling a new match.");
        }
        return;
    }

    // Create a dialog to show matches on this date
    QDialog matchesDialog(this);
    matchesDialog.setWindowTitle("Matches on " + date.toString("yyyy-MM-dd"));
    matchesDialog.setMinimumSize(500, 300);

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(&matchesDialog);

    // Create a table widget to display the matches
    QTableWidget *matchesTable = new QTableWidget(&matchesDialog);
    matchesTable->setColumnCount(7);

    // Set headers
    QStringList headers;
    headers << "ID" << "Date & Time" << "Lieu" << "Status" << "Score" << "Type" << "Spectateurs";
    matchesTable->setHorizontalHeaderLabels(headers);
    matchesTable->horizontalHeader()->setStretchLastSection(true);
    matchesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Count the matches for this date
    int matchCount = 1; // We already have the first match from query.next()
    while (query.next()) {
        matchCount++;
    }

    matchesTable->setRowCount(matchCount);

    // Reset query to start from beginning for this date
    query.first();
    query.previous();

    // Fill the table with data
    int row = 0;
    while (query.next()) {
        matchesTable->setItem(row, 0, new QTableWidgetItem(query.value("IDMATCH").toString()));

        // Format date and time nicely
        QDateTime dateTime = query.value("DATEMATCH").toDateTime();
        QString formattedDateTime = dateTime.toString("yyyy-MM-dd HH:mm");
        matchesTable->setItem(row, 1, new QTableWidgetItem(formattedDateTime));

        matchesTable->setItem(row, 2, new QTableWidgetItem(query.value("LIEU").toString()));
        matchesTable->setItem(row, 3, new QTableWidgetItem(query.value("STATUS").toString()));
        matchesTable->setItem(row, 4, new QTableWidgetItem(query.value("SCORE").toString()));
        matchesTable->setItem(row, 5, new QTableWidgetItem(query.value("TYPEMATCH").toString()));
        matchesTable->setItem(row, 6, new QTableWidgetItem(query.value("SPECTATEURS").toString()));

        row++;
    }

    layout->addWidget(matchesTable);

    // Add a close button
    QPushButton *closeButton = new QPushButton("Close", &matchesDialog);
    connect(closeButton, &QPushButton::clicked, &matchesDialog, &QDialog::accept);
    layout->addWidget(closeButton);

    // Show the dialog
    matchesDialog.exec();
}

void MainWindow::refreshCalendar()
{
    if (calendar) {
        loadMatchDates();
        highlightMatchDates();
    }
}
