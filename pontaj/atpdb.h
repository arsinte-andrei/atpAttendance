#ifndef ATPDB_H
#define ATPDB_H

#include <QMap>
#include <QVariant>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlQuery>
#include "atpini.h"

class atpDb : public QObject, public QSqlDatabase{
        Q_OBJECT
    private:
        QSqlDatabase mydb;
        bool openStatus;
        atpIni *atpIniSettings;
        QString dbPath, dbType;

    public:
        atpDb(QObject *parent = 0);
        static bool atp_create_new_atpdb(QString dbPath);

    public slots:
        QSqlQuery atpSelect(QString transId, QString sqlQuerry, QMap<QString, QVariant> *myArray = NULL);
        QSqlQuery atpInsert(QString transId, QString tableName, QMap<QString, QVariant> *data );
        QSqlQuery atpUpdate(QString transId, QString tableName, QMap<QString, QVariant> *data, QString whereTo);
        QSqlQuery atpDelete(QString transId, QString tableName, QString whereTo);

        bool setupNewConnection();
        bool isOpen();
        bool isDriverAvailable();
        void setDataBasePath(QString newPath);
    signals:
        void dbRefresh(bool);
        void atpSelected(QString, QSqlQuery);
        void atpInserted(QString, QSqlQuery);
        void atpUpdated(QString, QSqlQuery);
        void atpDeleted(QString, QSqlQuery);
};

#endif // ATPDB_H
