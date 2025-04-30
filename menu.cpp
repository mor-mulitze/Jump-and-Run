#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPainter>

#include "gamewindow.h"

#include "menu.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), gameWindow(nullptr) {
    setWindowTitle("Game Menu");
    setFixedSize(1024, 512);

    //Button
    QPixmap background(":/Grafik/Background.png");

    this->setStyleSheet("QWidget {background-image: url(:/Grafik/Background.png);"
                        "background-position: center;"
                        "background-repeat: no-repeat;"
                        "background-size: cover; }");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *startGameButton = new QPushButton("Start Game", this);
    QPushButton *highscoreButton = new QPushButton("Highscore", this);

    startGameButton->setFixedSize(400,100);
    highscoreButton->setFixedSize(400,100);

    layout->addWidget(startGameButton,0, Qt::AlignHCenter);
    layout->addWidget(highscoreButton,0, Qt::AlignHCenter);
    startGameButton->setFixedSize(400, 100);
    highscoreButton->setFixedSize(400, 100);

    layout->addWidget(startGameButton, 0, Qt::AlignHCenter);
    layout->addWidget(highscoreButton, 0, Qt::AlignHCenter);

    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(highscoreButton, &QPushButton::clicked, this, &MainWindow::showHighscore);
}

MainWindow::~MainWindow() {
    delete gameWindow;
}

void MainWindow::startGame() {
    if (!gameWindow) {
        gameWindow = new GameWindow();
    }
    gameWindow->show();
}

void MainWindow::showHighscore() {
    QMessageBox::information(this, "Highscore", "Displaying high scores...");
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap background(":/grafics/Background/Background_v1-1.png");

    // Zeichnet das Bild über das gesamte Fenster
    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event); // Basis-Methode aufrufen
}

/*
 * Hinzufügen des Hintergrundes.

/*void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter Farbe(this); // QPainter ist von Qt selbst

    // Zeichne das Viereck (Spieler)und die jeweilige Farbe
    Farbe.setBrush(Qt::green);
    Farbe.drawRect(viereckX, viereckY, viereckB, viereckH);

    // Zeichne die Hindernisse und die jeweilige Farbe
    Farbe.setBrush(Qt::red);
    for (const Obstacle &obstacle : obstacles) {
        Farbe.drawRect(obstacle.getRect());
    }
*/
