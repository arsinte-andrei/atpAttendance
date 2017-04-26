/*
 * ------------------------------------------------*
 * Typical pin or GPIO layout used:                |
 * ------------------------------------------------*
 *             MFRC522      Raspberry Pi B+ &      |
 * Power &     Reader/PCD   2 B                    |
 * Signal      Pin          Pin           GPIO     |
 * ------------------------------------------------*
 * RST/Reset   RST          22            25       |
 * SPI SS      SDA(SS)      24            8        |
 * SPI MOSI    MOSI         19            10       |
 * SPI MISO    MISO         21            9        |
 * SPI SCK     SCK          23            11       |
 * POWER       3.3V         1  / or 17             |
 *             GND          39 / Any               |
 *             IRQ          NONE           NONE    |
 * ------------------------------------------------*
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <atpsettings.h>
#include <atpinorout.h>
#include <atplogdetails.h>

#define  PINBEEP RPI_V2_GPIO_P1_11


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    userNo = 0;

    showDateAndTime();

    myIni = new atpIni(this);
    mydb = new atpDb();
    if(mydb->isDriverAvailable()){
            mydb->setupNewConnection();
        } else {
            qDebug() << "Eroare de gasire la db";
        }

    if (!bcm2835_init()){
        qDebug() <<"Eroare de initiere BCM lib";
    }

    bcm2835_gpio_fsel(PINBEEP, BCM2835_GPIO_FSEL_OUTP);

    //connect(mydb, SIGNAL(dbRefresh(bool)), this, SLOT(enabledDisabledMainMenu(bool)));

    updateUserNo();

    mfrc522 = new MFRC522(this);
    runCardCheck();
}

MainWindow::~MainWindow(){
    delete ui;
    bcm2835_spi_end();
    bcm2835_close();
}

void MainWindow::on_pushButton_clicked(){
    this->close();
}

void MainWindow::showDateAndTime() {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(60000);

    showTime();

}

void MainWindow::showTime() {
    time = QTime::currentTime();
    date = QDate::currentDate();
    timeText = time.toString("hh:mm");
    dateText = date.toString("dd/MM/yyyy");
//-TODO secundar sa blinck
    /*    if ((time.second() % 2) == 0)
            timeText[2] = ' ';
*/       ui->label_ceas->setText(timeText);
    ui->label_data->setText(dateText);
}

void MainWindow::showInfoOnDysplay() {
    atpInOrOut *frmInOrOut = new atpInOrOut(info, mydb, this);
    frmInOrOut->setModal(false);
    connect(frmInOrOut, SIGNAL(amDeschis()), this, SLOT(cantec()));
    connect(frmInOrOut, SIGNAL(amTerminat()), this, SLOT(updateUserNo()));
    frmInOrOut->open();
}

void MainWindow::updateUserNo() {
    userNo = 0;
    QSqlQuery q = mydb->atpSelect("1", "SELECT * FROM tbl_attendance WHERE time_worked is NULL OR time_worked = '' ");

    while (q.next()) {
        userNo++;
    }

    ui->lblUserNo->setText(QString::number(userNo));
}

void MainWindow::newCardpresented(QMap<byte, QString> detaliiCard){
    qDebug() <<detaliiCard;
    QDateTime currentDateTime;
    currentDateTime = QDateTime::currentDateTime();

    info["job_id"] = myIni->get_jobReference();
    info["card_id"] = detaliiCard[100];
    info["user_id"] = detaliiCard[1];
    info["user_first_name"] = detaliiCard[4];
    info["user_second_name"] = detaliiCard[5];
    info["time"] = currentDateTime.toString("d-M-yyyy h:m");
    info["timeInt"] = QString::number(currentDateTime.toTime_t());
    qDebug() <<info;
    showInfoOnDysplay();
    updateUserNo();
}

/*
 * Run RC522 in a separate thread from the program in order
 * not to freez gui
*/
void MainWindow::runCardCheck() {

    mfrc522->postConstruct();
    mfrc522->PCD_Init();

    rc522Thread = new atpRc522Thread(mfrc522, this);
    connect(rc522Thread, SIGNAL(cardReadDetails(QMap<byte,QString>)), this, SLOT(newCardpresented(QMap<byte,QString>)));
    connect(rc522Thread, SIGNAL(finished()), rc522Thread, SLOT(deleteLater()));
    rc522Thread->start();

}

void MainWindow::on_buttonSettings_clicked() {
    atpSettings *mySettingsMenu = new atpSettings(mydb, this);
    mySettingsMenu->setModal(true);
    mySettingsMenu->show();

}

void MainWindow::on_buttonLogs_clicked(){
    atpLogDetails *myLogDetails = new atpLogDetails(mydb, this);
    myLogDetails->setModal(true);
    myLogDetails->exec();
}

void MainWindow::cantec() {
    bcm2835_gpio_write(PINBEEP, HIGH);
    bcm2835_delay(500);
    bcm2835_gpio_write(PINBEEP, LOW);
    bcm2835_delay(100);
    bcm2835_gpio_write(PINBEEP, HIGH);
    bcm2835_delay(500);
    bcm2835_gpio_write(PINBEEP, LOW);
}
