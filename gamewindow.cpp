#include "gamewindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include "obstacle.h"
#include <QDebug>
#include <QRect>
#include <QPixmap>  // Für QPixmap zum Laden des Bildes
#include <QElapsedTimer>
#include <QRandomGenerator>  // Für moderne Zufallszahlen


GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
    viereckX(100), viereckY(500), viereckB(50), viereckH(50), isJumping(false), geschwindigkeitY(3), onGround(true), geschwindigkeitX(0), gamePaused(false)
{
    setFixedSize(1024, 512);        // Setzt die Fenstergröße
    startTimer(15);                  // wie schnell das Spiel ist
    spielTimer.start();             // Timer wird getartet

    // Initiales Hindernis
    obstacles.append(Obstacle(500, 512, 50, 50));

    // 🆕 Erste Plattform zufällig erzeugen (breiter & nicht zu hoch)
    int breite = 150 + QRandomGenerator::global()->bounded(100); // 150–249 px
    int y = 300 + QRandomGenerator::global()->bounded(150);      // Höhe zwischen 300–449
    plattformen.append(QRect(width(), y, breite, 20));
}

GameWindow::~GameWindow() {}

void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter Farbe(this);

    // Hintergrundbild laden und zeichnen
    QPixmap background(":/graphics/Background/Background_v2-1.png"); // Hier wird das Bild aus Ressourcen geladen
    Farbe.drawPixmap(0, 0, width(), height(), background); // Das Bild wird auf die ganze Fenstergröße skaliert

    // Spielerbild laden
    QPixmap playerImage(":/graphics/Spieler/stand-still_v1-1.png");
    Farbe.drawPixmap(viereckX, viereckY, viereckB, viereckH, playerImage);

    // Gegnerbild laden
    QPixmap obstacleImage(":/graphics/Gegner/Kojote_v1-1.png");
    for (const Obstacle &obstacle : obstacles) {
    QRect rect = obstacle.getRect();
    Farbe.drawPixmap(rect, obstacleImage);
    }


    if (gamePaused) {
        // Setzt eine transparente Farbe für das Overlay
        Farbe.setBrush(QColor(0, 0, 0, 100));  // Transparente schwarze Farbe für das Overlay
        Farbe.drawRect(0, 0, width(), height()); // Über das gesamte Fenster

        // Setzt die Textfarbe auf weiß und zeigt die Pause-Nachricht an
        Farbe.setPen(Qt::white);
        Farbe.setFont(QFont("Arial", 20));
        Farbe.drawText(width() / 2 - 100, height() / 2, "Spiel Pausiert! Drücke 'R' zum Fortsetzen.");
    }

    // Zeit berechnen
    qint64 ms = spielTimer.elapsed();
    double sekunden = ms / 1000.0;

    // Text formatieren
    QString zeitText = QString("Zeit: %1 s").arg(QString::number(sekunden, 'f', 2));

    // Zeit oben rechts anzeigen
    Farbe.setPen(Qt::white);
    Farbe.setFont(QFont("Arial", 14, QFont::Bold));
    int textBreite = Farbe.fontMetrics().horizontalAdvance(zeitText);
    Farbe.drawText(width() - textBreite - 10, 30, zeitText);

    // 🆕 Zeichne Plattformen (blaue Rechtecke)
        QPixmap plattformImage(":/graphics/Plattformen/Erde-mittig.png");
        for (const QRect &plattform : plattformen) {
        Farbe.drawPixmap(plattform, plattformImage);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (gamePaused) {
        // Wenn das Spiel pausiert ist, drücke 'R', um fortzufahren
        if (event->key() == Qt::Key_R) {
            gamePaused = false;

            // Nach dem Fortsetzen, zurücksetzen der Schwerkraft und der Boden-Erkennung
            geschwindigkeitY = 3;
            onGround = true;

            update(); // Das Spiel wieder aktualisieren
        }
        return;
    }

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
    if (gamePaused) {
        return; // Wenn das Spiel pausiert ist, keine Updates durchführen
    }

    if (!onGround) {
        geschwindigkeitY += 1;          // Schwerkraft anwenden (angepasst)
    }

    viereckY += geschwindigkeitY;
    viereckX += geschwindigkeitX;

    // Grenzwerte für den Spieler
    if (viereckX < 0) viereckX = 0;
    if (viereckX > width() - viereckB) viereckX = width() - viereckB;

    // Überprüfen, ob der Spieler den Boden berührt
    if (viereckY >= height() - 50) {
        viereckY = height() - 50;       // Das Viereck darf nicht unter den Boden gehen
        onGround = true;                // Es steht jetzt auf dem Boden
        geschwindigkeitY = 0;           // Stoppe die Bewegung in Y-Richtung
        isJumping = false;              // Jetzt kann wieder gesprungen werden
    }

    // Bewegungen der Hindernisse
    for (Obstacle &obstacle : obstacles) {
        obstacle.move();                         // Bewege das Hindernis
        obstacle.reset(width(), height());       // Setze es neu, wenn es den Bildschirm verlässt
    }

    // 🆕 Plattformen bewegen und bei Bedarf neu generieren
    for (int i = 0; i < plattformen.size(); ++i) {
        plattformen[i].translate(-5, 0);  // Plattform nach links bewegen

        if (plattformen[i].right() < 0) {
            int neueBreite = 150 + QRandomGenerator::global()->bounded(100); // 150–249
            int neueY = 300 + QRandomGenerator::global()->bounded(150);      // 300–449
            plattformen[i] = QRect(width(), neueY, neueBreite, 20);
        }
    }

    QRect playerRect(viereckX, viereckY, viereckB, viereckH);

    // 🆕 Kollision mit Plattformen
    for (const QRect &plattform : plattformen) {
        if (playerRect.intersects(plattform) && geschwindigkeitY > 0) {
            viereckY = plattform.y() - viereckH; // Spieler landet auf Plattform
            onGround = true;
            geschwindigkeitY = 0;
            isJumping = false;
        }
    }

    // Überprüfe jede Kollision mit Hindernissen
    for (const Obstacle &obstacle : obstacles) {
        if (checkCollisionPixelBased(playerRect, obstacle)) {
            qDebug() << "💥 Kollision erkannt! Spieler bei:" << viereckX << viereckY
                     << "| Hindernis bei:" << obstacle.getRect();
            gamePaused = true;  // Spiel pausieren
            handleCollision(playerRect, obstacle); // Kollision behandeln
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

    // Berechnung der überlappenden Region
    int startX = std::max(playerRect.left(), obstacleRect.left());
    int startY = std::max(playerRect.top(), obstacleRect.top());
    int endX = std::min(playerRect.right(), obstacleRect.right());
    int endY = std::min(playerRect.bottom(), obstacleRect.bottom());

    // Überprüfung nur in der überlappenden Region
    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            if (playerRect.contains(x, y) && obstacleRect.contains(x, y)) {
                qDebug() << "🎯 Pixel-Kollision erkannt bei" << x << y;
                return true;  // Kollision erkannt
            }
        }
    }

    return false;  // Keine Kollision
}

void GameWindow::handleCollision(const QRect &playerRect, const Obstacle &obstacle)
{
    QRect obstacleRect = obstacle.getRect();

    if (playerRect.bottom() > obstacleRect.top() && playerRect.top() < obstacleRect.top()) {
        viereckY = obstacleRect.top() - viereckH;
        geschwindigkeitY = 0;
        onGround = true;
    }

    if (playerRect.right() > obstacleRect.left() && playerRect.left() < obstacleRect.right()) {
        if (geschwindigkeitX > 0) {
            viereckX = obstacleRect.left() - viereckB;
        } else if (geschwindigkeitX < 0) {
            viereckX = obstacleRect.right();
        }
    }
}
