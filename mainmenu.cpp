#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>

#include "gamewindow.h"
#include "mainmenu.h"
#include "scorewindow.h"

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(1024, 512);
    // Menüleiste erstellen
    QMenu *fileMenu = menuBar()->addMenu("Datei");
    fileMenu->addAction("Beenden", this, &QWidget::close);

    // Zentrales Widget mit Layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    button1 = new QPushButton("Play", this);
    button1->setFixedSize(300,100);
    button2 = new QPushButton("Score", this);
    button2->setFixedSize(300,100);

    layout->addStretch();
    layout->addWidget(button1, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(button2, 0, Qt::AlignHCenter);

    layout->addStretch();

    connect(button1, &QPushButton::clicked, this, [=]() {
        GameWindow *gameWindow = new GameWindow();
        gameWindow->show();
    });
    connect(button2, &QPushButton::clicked, this, [=]() {
        ScoreWindow *scoreFenster = new ScoreWindow();
        scoreFenster->show();
    });

    setCentralWidget(centralWidget);
}

void MainMenu::onButton1Clicked()
{
    auto newWindow = new GameWindow();
    newWindow->show();
}

void MainMenu::onButton2Clicked()
{
    auto newWindow = new ScoreWindow();
    newWindow->show();
}


MainMenu::~MainMenu()
{
}

void MainMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap background(":/graphics/Background/Background_v2-1.png");
    painter.drawPixmap(0,0,width(),height(), background);
}
