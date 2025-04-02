#include "gamewindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include "obstacle.h"
#include <QDebug>
#include <QPixmap>  // Für QPixmap zum Laden des Bildes

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
    viereckX(100), viereckY(500), viereckB(50), viereckH(50), isJumping(false), geschwindigkeitY(3), onGround(true), geschwindigkeitX(0)
{
    setFixedSize(1024, 512);  // Setzt die Fenstergröße
    if (!playerSprite.load(":/graphics/Character/stand-still_v1-1.png")) {
        qDebug() << "❌ Fehler: Spieler-Sprite konnte nicht geladen werden!";
    } else {
        qDebug() << "✅ Spieler-Sprite erfolgreich geladen!";
    }
    startTimer(9);  // Geschwindigkeit des Spiels


    // Initiales Hindernis
    obstacles.append(Obstacle(500, 512, 50, 50));
}

GameWindow::~GameWindow() {}

void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter Farbe(this);


    // Hintergrundbild laden und zeichnen
    QPixmap background(":/graphics/Background/Background_v2-0.png"); // Hier wird das Bild aus Ressourcen geladen
    Farbe.drawPixmap(0, 0, width(), height(), background); // Das Bild wird auf die ganze Fenstergröße skaliert


    // Zeichne den Spieler (grünes Rechteck)
    Farbe.setBrush(Qt::green);
    Farbe.drawRect(viereckX, viereckY, viereckB, viereckH);

    // Zeichne Hindernisse (rote Rechtecke)
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
        geschwindigkeitX = -5;
    }

    if (event->key() == Qt::Key_D) {
        geschwindigkeitX = 5;
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && geschwindigkeitY < 0) {
        geschwindigkeitY = 0;
    }

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        geschwindigkeitX = 0;
    }
}

void GameWindow::timerEvent(QTimerEvent *event)
{

    if (!onGround) {

        geschwindigkeitY += 1;          // Schwerkraft anwenden (angepasst)
    }

    viereckY += geschwindigkeitY;
    viereckX += geschwindigkeitX;

    if (viereckX < 0) viereckX = 0;
    if (viereckX > width() - viereckB) viereckX = width() - viereckB;

    if (viereckY >= height() - 50) {

        viereckY = height() - 50;       // Das Viereck darf nicht unter den Boden gehen
        onGround = true;                // Es steht jetzt auf dem Boden
        geschwindigkeitY = 0;           // Stoppe die Bewegung in Y-Richtung
        isJumping = false;              // Jetzt kann wieder gesprungen werden
    }

    for (Obstacle &obstacle : obstacles) {

        obstacle.move();                         // Bewege das Hindernis
        obstacle.reset(width(), height());       // Setze es neu, wenn es den Bildschirm verlässt
    }

    QRect playerRect(viereckX, viereckY, viereckB, viereckH);

    for (const Obstacle &obstacle : obstacles) {
        if (checkCollisionPixelBased(playerRect, obstacle)) {
            qDebug() << "💥 Kollision erkannt! Spieler bei:" << viereckX << viereckY
                     << "| Hindernis bei:" << obstacle.getRect();
        } else {
            qDebug() << "❌ Keine Kollision. Spieler bei:" << viereckX << viereckY
                     << "| Hindernis bei:" << obstacle.getRect();
        }

    }

    update();
}

bool GameWindow::checkCollisionPixelBased(const QRect &playerRect, const Obstacle &obstacle)
{
    QRect obstacleRect = obstacle.getRect();

    // Erst eine grobe Rechteck-Kollisionserkennung
    if (!playerRect.intersects(obstacleRect)) {
        return false;  // Keine Kollision, also keine weitere Prüfung nötig
    }

    // Spieler-Sprite als Bild rendern
    QImage playerImage(viereckB, viereckH, QImage::Format_ARGB32);
    playerImage.fill(Qt::transparent);
    QPainter playerPainter(&playerImage);
    playerPainter.drawPixmap(0, 0, viereckB, viereckH, playerSprite);

    // Hindernis als Bild rendern
    QImage obstacleImage(obstacleRect.width(), obstacleRect.height(), QImage::Format_ARGB32);
    obstacleImage.fill(Qt::transparent);
    QPainter obstaclePainter(&obstacleImage);
    obstaclePainter.setBrush(Qt::red);
    obstaclePainter.drawRect(0, 0, obstacleRect.width(), obstacleRect.height());

    // Berechnung der überlappenden Region
    int startX = std::max(playerRect.left(), obstacleRect.left());
    int startY = std::max(playerRect.top(), obstacleRect.top());
    int endX = std::min(playerRect.right(), obstacleRect.right());
    int endY = std::min(playerRect.bottom(), obstacleRect.bottom());

    // Überprüfung nur in der überlappenden Region
    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            int playerLocalX = x - playerRect.left();
            int playerLocalY = y - playerRect.top();
            int obstacleLocalX = x - obstacleRect.left();
            int obstacleLocalY = y - obstacleRect.top();

            QColor playerColor = playerImage.pixelColor(playerLocalX, playerLocalY);
            QColor obstacleColor = obstacleImage.pixelColor(obstacleLocalX, obstacleLocalY);

            if (playerColor.alpha() > 0 && obstacleColor.alpha() > 0) {
                qDebug() << "🎯 Pixel-Kollision erkannt bei" << x << y;
                return true;
            }
        }
    }

    return false;
}

