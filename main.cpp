#include <QApplication>
#include "menu.h"

<<<<<<< HEAD
// main.cpp
#include <QApplication>
#include "menu.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
=======
int main(int argc, char *argv[])
{
    QApplication a{argc, argv};
    GameWindow w;
    w.show();           // Öffne das Spiel-Fenster
    return a.exec();    // Starte die Event-Schleife
>>>>>>> upstream/main
}
