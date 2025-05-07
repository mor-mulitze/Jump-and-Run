#ifndef SCOREWINDOW_H
#define SCOREWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

class ScoreWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ScoreWindow(QWidget *parent = nullptr);
    ~ScoreWindow();

private:
    QTableWidget *tableWidget;  // Die Tabelle für den Score
};

#endif // SCOREWINDOW_H
