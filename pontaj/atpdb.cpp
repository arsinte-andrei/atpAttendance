#include "atpdb.h"

#include <QtSql>
#include <QFile>
#include <QMessageBox>
#include <QApplication>

atpDb::atpDb(QObject *parent) : QObject(parent){
    openStatus = false;
//reading ini file
    atpIniSettings = new atpIni;
    dbPath = atpIniSettings->get_dbPath();
    dbType = atpIniSettings->get_dbType();
}

bool atpDb::isDriverAvailable(){
//is driver available
    if(!QSqlDatabase::isDriverAvailable(dbType)){
        QMessageBox::critical(0, qApp->tr("Driver is not available"),
        qApp->tr("Unable to establish a database connection.\n"
        "This app needs SQLite support. Please read "
        "the Qt SQL driver documentation for information how "
        "to build it.\n\n"
        "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}

bool atpDb::setupNewConnection(){
    dbPath = atpIniSettings->get_dbPath();
//check for the file exist
    QFile db_file(dbPath);
    if (!db_file.exists()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
        qApp->tr("Database file not found. Create or import one!"), QMessageBox::Ok);
        emit dbRefresh(false);
        return false;
    }
    mydb = QSqlDatabase::addDatabase(dbType);
    mydb.setDatabaseName(dbPath);
    if (!mydb.open()){
        qCritical() << "couldn't connect to database Error[" << mydb.lastError().text() << "]"  << dbPath;
        emit dbRefresh(false);
        return false;
    } else {
        qDebug() << "succsessfully connected to database " << dbPath;
        openStatus = true;
        emit dbRefresh(true);
        return true;
    }
}

bool atpDb::isOpen(){
    return openStatus;
}

void atpDb::setDataBasePath(QString newPath){
    dbPath = newPath;
}

QSqlQuery atpDb::atpSelect(QString transId, QString sqlQuerry, QMap<QString, QVariant> *myArray ) {
    QSqlQuery myQuerry;
    myQuerry.prepare(sqlQuerry);
//	qDebug() << myArray;
    if (myArray && !myArray->isEmpty()){
        QMap<QString, QVariant>::const_iterator i;
        for (i = myArray->constBegin(); i != myArray->constEnd(); ++i) {
            myQuerry.bindValue(i.key(), i.value());
            //myQuerry.bindValue(i.key().toUtf8().data(), i.value().toString().toUtf8().data());
        }
    }
//	qDebug() << myQuerry.boundValues();
    myQuerry.exec();
//	qDebug() <<  myQuerry.lastQuery() <<myQuerry.lastError();
    emit atpSelected(transId, myQuerry);
    return myQuerry;
}

QSqlQuery atpDb::atpInsert(QString transId, QString tableName, QMap<QString, QVariant> *data ) {
    QString fieldNames, fieldValues, temp;
    QMap<QString, QVariant>::const_iterator i1;
    for (i1 = data->constBegin(); i1 != data->constEnd(); ++i1) {
        temp = i1.key();
        fieldValues += temp + ", ";
        fieldNames += temp.remove(0,1) + ", ";
    }
    fieldNames.remove(fieldNames.lastIndexOf(","),1);
    fieldValues.remove(fieldValues.lastIndexOf(","),1);
//	qDebug() << fieldNames << "** - **" << fieldValues;

    QSqlQuery myQuerry;
    myQuerry.prepare("INSERT INTO " + tableName + " ( " + fieldNames + ") VALUES ( " + fieldValues + ")");
//	qDebug() << data;
    QMap<QString, QVariant>::const_iterator i;
    for (i = data->constBegin(); i != data->constEnd(); ++i) {
        myQuerry.bindValue(i.key(), i.value());
        //myQuerry.bindValue(i.key().toUtf8().data(), i.value().toString().toUtf8().data());
    }
//	qDebug() << myQuerry.boundValues();
    myQuerry.exec();
//	qDebug() <<  myQuerry.lastQuery() <<myQuerry.lastError();
    emit atpInserted(transId, myQuerry);
    return myQuerry;
}

QSqlQuery atpDb::atpUpdate(QString transId, QString tableName, QMap<QString, QVariant> *data, QString whereTo) {
    QString fieldDetails, fieldNames, fieldValues, temp;
    QMap<QString, QVariant>::const_iterator i1;
    for (i1 = data->constBegin(); i1 != data->constEnd(); ++i1) {
        temp = i1.key();
        fieldValues = temp;
        fieldNames = temp.remove(0,1);
        fieldDetails += fieldNames + " = " + fieldValues + ", ";
    }
    fieldDetails.remove(fieldDetails.lastIndexOf(","),1);
    QSqlQuery myQuerry;
    myQuerry.prepare("UPDATE " + tableName + " SET " + fieldDetails + " WHERE " + whereTo);
    QMap<QString, QVariant>::const_iterator i;
    for (i = data->constBegin(); i != data->constEnd(); ++i) {
        myQuerry.bindValue(i.key(), i.value());
    }
    myQuerry.exec();
//	qDebug() <<  myQuerry.lastQuery() <<myQuerry.lastError();
    emit atpUpdated(transId, myQuerry);
    return myQuerry;
}

QSqlQuery atpDb::atpDelete(QString transId, QString tableName, QString whereTo){
    QSqlQuery myQuerry;
    myQuerry.prepare("DELETE FROM " + tableName + " WHERE " + whereTo);
    myQuerry.exec();
//	qDebug() <<  myQuerry.lastQuery() <<myQuerry.lastError();
    emit atpDeleted(transId, myQuerry);
    return myQuerry;
}

bool atpDb::atp_create_new_atpdb(QString dbPath) {
    atpIni *atpIniSettings = new atpIni;
    QString dbType;
    dbType = atpIniSettings->get_dbType();
    QSqlDatabase mydb;
    mydb = QSqlDatabase::addDatabase(dbType);
    mydb.setDatabaseName(dbPath);
    mydb.open();
    QVector<QString> tables(0);
    tables.append("CREATE TABLE tbl_attendance (pi_id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, job_id integer, card_id integer, user_id integer, first_name text, second_name text, time_in text, time_out text, time_worked text, time_int_in integer, time_int_out integer, time_int_worked integer);");
    tables.append("CREATE TABLE tbl_logs (log_id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, att_id integer, job_id integer, card_id integer, user_id integer, timet text, timei integer, first_name text, second_name text, acction text);");
    QSqlQuery myquerry;
    for (int i = 0; i < tables.size(); ++i) {
        myquerry.exec(tables[i]);
    }
    return true;
}
