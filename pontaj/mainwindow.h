#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <bcm2835.h>
#include <MFRC522.h>
#include "atprc522thread.h"
#include "atpini.h"
#include "atpdb.h"
#include <QDate>
#include <QTime>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    QMap<QString, QString> info;

    MFRC522 *mfrc522; // Create MFRC522 instance.
    void runCardCheck();
    atpRc522Thread *rc522Thread;

public slots:
    void cantec();

private slots:
    void showDateAndTime();
    void showTime();
    void showInfoOnDysplay();
    void updateUserNo();
    void newCardpresented(QMap<byte, QString> detaliiCard);

    void on_pushButton_clicked();
    void on_buttonSettings_clicked();
    void on_buttonLogs_clicked();

private:
    Ui::MainWindow *ui;
    atpIni *myIni;
    atpDb *mydb;
    bool haveConection;
    QTime time;
    QDate date;
    QString timeText;
    QString dateText;
    int userNo;

};

#endif // MAINWINDOW_H
