/*
 Anna
 * */
#include <QApplication>
#include "gamewindow.h"

int main(int argc, char *argv[])
{
    QApplication a{argc, argv};
    GameWindow w;
    w.show();           // Öffne das Spiel-Fenster
    return a.exec();    // Starte die Event-Schleife
}
