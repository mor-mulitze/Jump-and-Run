#include "gamewindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QRandomGenerator>
#include "obstacle.h"
#include <QDebug>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
    viereckX(100), viereckY(500), viereckB(50), viereckH(50), isJumping(false), geschwindigkeitY(3), onGround(true), geschwindigkeitX(0)
{
    setFixedSize(1024, 512);  // Setzt die Fenstergröße
    startTimer(9);  // wie schnell das Spiel ist

    // Initiales Hindernis
    obstacles.append(Obstacle(500, 512, 50, 50));  // Erstes Hindernis rechts außerhalb des Bildschirms
}

GameWindow::~GameWindow() {}

void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter Farbe(this); // QPainter ist von Qt selbst

    // Zeichne das Viereck (Spieler) und die jeweilige Farbe
    Farbe.setBrush(Qt::green);
    Farbe.drawRect(viereckX, viereckY, viereckB, viereckH);

    // Zeichne die Hindernisse und die jeweilige Farbe
    Farbe.setBrush(Qt::red);
    for (const Obstacle &obstacle : obstacles) {
        Farbe.drawRect(obstacle.getRect());
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && !isJumping && onGround) {
        isJumping = true;
        geschwindigkeitY = -30;
        onGround = false;
    }
    if (event->key() == Qt::Key_A) {
        geschwindigkeitX = -5; // Bewege nach links
    }
    if (event->key() == Qt::Key_D) {
        geschwindigkeitX = 5; // Bewege nach rechts
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && geschwindigkeitY < 0) {
        geschwindigkeitY = 0;
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        geschwindigkeitX = 0; // Stoppe Bewegung, wenn Taste losgelassen wird
    }
}

void GameWindow::timerEvent(QTimerEvent *event)
{
    if (!onGround) {
        geschwindigkeitY += 1;
    }

    viereckY += geschwindigkeitY;
    viereckX += geschwindigkeitX;

    // Begrenzung, damit das Rechteck nicht aus dem Fenster läuft
    if (viereckX < 0) viereckX = 0;
    if (viereckX > width() - viereckB) viereckX = width() - viereckB;

    if (viereckY >= height() - 50) {
        viereckY = height() - 50;
        onGround = true;
        geschwindigkeitY = 0;
        isJumping = false;
    }

    for (Obstacle &obstacle : obstacles) {
        obstacle.move();
        obstacle.reset(width(), height());
    }

    QRect playerRect(viereckX, viereckY, viereckB, viereckH);
    for (const Obstacle &obstacle : obstacles) {
        if (playerRect.intersects(obstacle.getRect())) {
            qDebug() << "Kollision erkannt!";
        }
    }
    update();
}

