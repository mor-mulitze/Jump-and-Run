#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QPushButton>

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *button1;
    QPushButton *button2;

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
};

#endif // MAINWINDOW_H
