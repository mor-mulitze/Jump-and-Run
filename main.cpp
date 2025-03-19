#include <QApplication>
#include "menu.h"

// main.cpp
#include <QApplication>
#include "menu.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
