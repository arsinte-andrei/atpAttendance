#include "atpinorout.h"
#include "ui_atpinorout.h"
#include <QTimer>
#include <QSqlQuery>
#include <QDebug>
#include <bcm2835.h>
#include<QThread>

#define  PINBEEP RPI_V2_GPIO_P1_11

atpInOrOut::atpInOrOut(QMap<QString, QString> infoToDisplay, atpDb *mydb, QWidget *parent) : QDialog(parent), ui(new Ui::atpInOrOut) {
    ui->setupUi(this);
    myIniSettings = new atpIni(this);
    myATPdb = mydb;

    qDebug() << "Pas 1";

    infoCard = infoToDisplay;
    timpul = "";
    update = false;

    qDebug() << "Pas 3";
    checkDetails();

    qDebug() << "Pas 4";
    selfCloseAfterTime();
    qDebug() << "Pas 5";
}

atpInOrOut::~atpInOrOut() {
    delete ui;
}

/*
 * functie care inchide automat dialogul dupa afisarea informatiilor
 */
void atpInOrOut::selfCloseAfterTime() {
    QTimer *timer1 = new QTimer(this);
    timer1->setSingleShot(true);
    connect(timer1, SIGNAL(timeout()), this, SLOT(updateDb()));
    timer1->start(2000);
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(accept()));
    timer->start(myIniSettings->get_infoDelay()*1000);
}

void atpInOrOut::checkDetails(){


    logInsert = new QMap<QString, QVariant>;
    selectMap = new QMap<QString, QVariant>;

    selectMap->clear();
    selectMap->insert(":user_id", infoCard["user_id"]);
    QSqlQuery q = myATPdb->atpSelect("1", "SELECT * FROM tbl_attendance WHERE user_id = :user_id AND time_worked is NULL OR time_worked = '' ", selectMap);

    if(!q.first()){

        qDebug() << "Pas 35";
        firstInsert = new QMap<QString, QVariant>;;
        qDebug() << "Se introduce un nou camp in baza de date";
        firstInsert->clear();
        firstInsert->insert(":job_id", infoCard["job_id"]);
        firstInsert->insert(":card_id", infoCard["card_id"]);
        firstInsert->insert(":user_id", infoCard["user_id"]);
        firstInsert->insert(":first_name", infoCard["user_first_name"]);
        firstInsert->insert(":second_name", infoCard["user_second_name"]);
        firstInsert->insert(":time_in", infoCard["time"]);
        firstInsert->insert(":time_int_in", infoCard["timeInt"]);
        timpul = infoCard["time"];
        acctiuneaDesfasurata = "Logged In";
        update = false;
        qDebug() << "Pas 36";

    } else {
        qDebug() << "Pas 37";
        secondInsert = new QMap<QString, QVariant>;;
        qDebug() << "De facut calculele la Update";
        idToUpdate = q.value("pi_id").toInt();
        int timeWorked, oldTime, newTime;
        QString timeWorkedText;
        oldTime = q.value("time_int_in").toInt();
        newTime = infoCard["timeInt"].toInt();
        timeWorked = newTime - oldTime;

        qDebug() << timeWorked  <<" - " << newTime <<" - " << oldTime;

        if(timeWorked > 86400){
            //zile lucrate
            int zileL = timeWorked / 86400;
            int restR = timeWorked % 86400;
            int oreL = restR / 3600;
            int restS = restR & 3600;
            int minL = restS / 60;

            timeWorkedText = QString::number(zileL).append(" zile ").append(QString::number(oreL).append(" ore ").append(QString::number(minL).append(" min ")));

        } else if(timeWorked > 3600){
            //ore lucrate
            int oreL = timeWorked / 3600;
            int restS = timeWorked & 3600;
            int minL = restS / 60;

            timeWorkedText = QString::number(oreL).append(" ore ").append(QString::number(minL).append(" min "));

        } else {
            //minute lucrate
            int minL = timeWorked / 60;
            timeWorkedText = QString::number(minL).append(" min ");
        }

        secondInsert->clear();
        secondInsert->insert(":pi_id", idToUpdate);
        secondInsert->insert(":time_out", infoCard["time"]);
        secondInsert->insert(":time_int_out", infoCard["timeInt"]);
        secondInsert->insert(":time_worked", timeWorkedText);
        secondInsert->insert(":time_int_worked", timeWorked);
        timpul = infoCard["time"] + " - " + timeWorkedText;
        acctiuneaDesfasurata = "Logged Out";
        update = true;

        qDebug() << "Pas 38";
    }
    qDebug() << "Pas 39";
    displayInfo();
}

void atpInOrOut::displayInfo(){
    qDebug() << "Pas 311";
    QString numeleComplet;
    numeleComplet = infoCard["user_first_name"] + " " + infoCard["user_second_name"];

    ui->lbl_greeting->setText("Welcome...");
    ui->lbl_name->setText(numeleComplet);
    ui->lbl_time->setText(timpul);
    ui->lbl_info->setText(acctiuneaDesfasurata);
    qDebug() << "Pas 312";

    cantec();

    emit amDeschis();

}

void atpInOrOut::updateDb() {
    if (update){
        qDebug() << "Modifica camp vech";
        myATPdb->atpUpdate("1", "tbl_attendance", secondInsert, "pi_id = :pi_id");
    } else {
        qDebug() << "Insereaza camp nou";
        QSqlQuery q = myATPdb->atpInsert("1", "tbl_attendance", firstInsert);
        idToUpdate = q.lastInsertId().toInt();
    }
       logUser();
}

void atpInOrOut::logUser() {
    qDebug() << "Pas 331";
    logInsert->clear();
    logInsert->insert(":att_id", idToUpdate);
    logInsert->insert(":job_id", infoCard["job_id"]);
    logInsert->insert(":card_id", infoCard["card_id"]);
    logInsert->insert(":user_id", infoCard["user_id"]);
    logInsert->insert(":timet", infoCard["time"]);
    logInsert->insert(":timei", infoCard["timeInt"]);
    logInsert->insert(":first_name", infoCard["user_first_name"]);
    logInsert->insert(":second_name", infoCard["user_second_name"]);
    logInsert->insert(":acction", acctiuneaDesfasurata);
    myATPdb->atpInsert("log", "tbl_logs", logInsert);
    emit amTerminat();
    qDebug() << "Pas 332";
}

void atpInOrOut::cantec() {
    bcm2835_gpio_write(PINBEEP, HIGH);
    bcm2835_delay(500);
    bcm2835_gpio_write(PINBEEP, LOW);
    bcm2835_delay(100);
    bcm2835_gpio_write(PINBEEP, HIGH);
    bcm2835_delay(500);
    bcm2835_gpio_write(PINBEEP, LOW);
}
