#ifndef ATPINOROUT_H
#define ATPINOROUT_H

#include <QDialog>
#include<QMap>
#include <atpdb.h>
#include<atpini.h>

namespace Ui {
class atpInOrOut;
}

class atpInOrOut : public QDialog {
    Q_OBJECT

public:
    explicit atpInOrOut(QMap<QString, QString> infoToDisplay, atpDb *mydb, QWidget *parent = 0);
    ~atpInOrOut();

signals:
    void amDeschis();
    void amTerminat();

public slots:
    void selfCloseAfterTime();
    void checkDetails();
    void displayInfo();

    void updateDb();
    void logUser();
    void cantec();

private:
    Ui::atpInOrOut *ui;
    bool update;
    atpIni *myIniSettings;
    atpDb *myATPdb;
    QMap<QString, QString> infoCard;
    QMap<QString, QVariant> *selectMap, *logInsert, *firstInsert, *secondInsert;
    QString acctiuneaDesfasurata, timpul;
    int idToUpdate;

};

#endif // ATPINOROUT_H
