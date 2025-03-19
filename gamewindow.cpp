#include "gamewindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QRandomGenerator>
#include "obstacle.h"
#include <QDebug>
#include <QPixmap>  // Für QPixmap zum Laden des Bildes

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
    viereckX(100), viereckY(500), viereckB(50), viereckH(50), isJumping(false), geschwindigkeitY(3), onGround(true), geschwindigkeitX(0)
{
    setFixedSize(1024, 512);  // Setzt die Fenstergröße
    startTimer(9);  // wie schnell das Spiel ist

    // Initiales Hindernis
    obstacles.append(Obstacle(500, 512, 50, 50));  // Erstes Hindernis rechts außerhalb des Bildschirms

    // Das Bild für den Spieler (Deklaration)
    QPixmap playerImage;
    // Bild für den Spieler laden
    playerImage = QPixmap(":/Grafik/Spieler.jpg");  // Stelle sicher, dass der Pfad korrekt ist
    if (playerImage.isNull()) {
        qDebug() << "Fehler: Spielerbild konnte nicht geladen werden!";
    }

    // Das Bild für den Gegner (Deklaration)
    QPixmap obstacleImage;
    // Bild für das Hindernis laden
    obstacleImage = QPixmap(":/Grafik/Gegner.jpg");  // Hindernisbild laden
    if (obstacleImage.isNull()) {
        qDebug() << "Fehler: Gegnerbild konnte nicht geladen werden!";
    }
}

GameWindow::~GameWindow() {}

void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter Farbe(this); // QPainter ist von Qt selbst

    // Bild für den Hintergrund aus den Ressourcen laden
    QPixmap background(":/Grafik/Background.png");
    // Das Bild als Hintergrund zeichnen
    Farbe.drawPixmap(0, 0, width(), height(), background);  // Das Bild auf die gesamte Fläche skalieren

    // Das Bild für den Spieler (Deklaration)
    QPixmap playerImage;
    // Das Bild für den Spieler zeichnen (anstatt des Rechtecks)
    if (!playerImage.isNull()) {
        Farbe.drawPixmap(viereckX, viereckY, viereckB, viereckH, playerImage);  // Bild zeichnen
    }

    // Zeichne die Hindernisse und die jeweilige Farbe
    Farbe.setBrush(Qt::green);
    for (const Obstacle &obstacle : obstacles) {
        Farbe.drawRect(obstacle.getRect());
    }

  /*
    // Das Bild für das Hindernis zeichnen
    for (const Obstacle &obstacle : obstacles) {
        // Hindernis als Bild zeichnen
        if (!obstacleImage.isNull()) {
            Farbe.drawPixmap(obstacle.getX(), obstacle.getY(), obstacle.getWidth(), obstacle.getHeight(), obstacleImage);
        }
    }
*/
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && !isJumping && onGround) {
        // Nur springen, wenn das Viereck auf dem Boden ist und nicht bereits springt
        isJumping = true;
        geschwindigkeitY = -30;  // Anfangsgeschwindigkeit des Sprungs (angepasst)
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
    // Leertaste loslassen: Verhindern, dass das Viereck weiter nach oben schießt
    if (event->key() == Qt::Key_Space && geschwindigkeitY < 0) {
        geschwindigkeitY = 0;  // Stoppe das Springen, wenn die Taste losgelassen wird
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) {
        geschwindigkeitX = 0; // Stoppe Bewegung, wenn Taste losgelassen wird
    }
}

void GameWindow::timerEvent(QTimerEvent *event)
{
    // Schwerkraft anwenden (geschwindigkeitY wird in jedem Frame erhöht)
    if (!onGround) {
        geschwindigkeitY += 1;  // Schwerkraft anwenden (angepasst)
    }

    // Position des Vierecks aktualisieren
    viereckY += geschwindigkeitY;
    viereckX += geschwindigkeitX;

    // Begrenzung, damit das Rechteck nicht aus dem Fenster läuft
    if (viereckX < 0) viereckX = 0;
    if (viereckX > width() - viereckB) viereckX = width() - viereckB;

    // Wenn das Viereck den Boden erreicht hat
    if (viereckY >= height() - 50) {
        viereckY = height() - 50;  // Das Viereck darf nicht unter den Boden gehen
        onGround = true;  // Es steht jetzt auf dem Boden
        geschwindigkeitY = 0;  // Stoppe die Bewegung in Y-Richtung
        isJumping = false;  // Jetzt kann wieder gesprungen werden
    }

    // Hindernisse bewegen (von rechts nach links)
    for (Obstacle &obstacle : obstacles) {
        obstacle.move();  // Bewege das Hindernis
        obstacle.reset(width(), height());  // Setze es neu, wenn es den Bildschirm verlässt
    }

    QRect playerRect(viereckX, viereckY, viereckB, viereckH);

    // Überprüfen, ob der Spieler mit einem Hindernis kollidiert
    for (const Obstacle &obstacle : obstacles) {
        if (playerRect.intersects(obstacle.getRect())) {
            // Kollision erkannt!
            qDebug() << "Kollision erkannt!";
        }
    }
    update();  // Das Fenster neu zeichnen
}


