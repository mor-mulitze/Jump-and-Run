#include "scorewindow.h"
#include <QPainter>


ScoreWindow::ScoreWindow(QWidget *parent)
    : QWidget(parent),
    tableWidget(new QTableWidget(this))  // Initialisiere die Tabelle
{
    // Fenstergröße festlegen
    setFixedSize(400, 300);

    // Setze die Anzahl der Zeilen und Spalten der Tabelle
    tableWidget->setRowCount(5);  // Beispiel: 5 Zeilen
    tableWidget->setColumnCount(2);  // Beispiel: 2 Spalten (Name und Score)

    // Setze die Header-Beschriftungen für die Spalten
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Name" << "Punkte");

    // Füge einige Beispiel-Daten hinzu (z.B. Spielername und Punktzahl)
    tableWidget->setItem(0, 0, new QTableWidgetItem("Spieler 1"));
    tableWidget->setItem(0, 1, new QTableWidgetItem("100"));
    tableWidget->setItem(1, 0, new QTableWidgetItem("Spieler 2"));
    tableWidget->setItem(1, 1, new QTableWidgetItem("80"));
    tableWidget->setItem(2, 0, new QTableWidgetItem("Spieler 3"));
    tableWidget->setItem(2, 1, new QTableWidgetItem("120"));

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
}

ScoreWindow::~ScoreWindow() {}
