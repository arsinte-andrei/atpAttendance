#include "atpini.h"
#include <QDebug>

atpIni::atpIni(QObject *parent) : QSettings(parent) {
	mySettings = new QSettings("atp.ini", QSettings::IniFormat);
}

//DataBase path
QString atpIni::get_dbPath(){
	return mySettings->value("DB/Path").toString();
}

void atpIni::set_dbPath(QString value){
	mySettings->setValue("DB/Path", value);
}

//Database type
QString atpIni::get_dbType(){
	return mySettings->value("DB/Type", "QSQLITE").toString();
}

void atpIni::set_dbType(QString value){
    mySettings->setValue("DB/Type", value);
}

int atpIni::get_infoDelay(){
    return mySettings->value("Info/Delay", 4).toInt();
}

void atpIni::set_infoDelay(int value){
    mySettings->setValue("Info/Delay", value);
}

QString atpIni::get_jobAddres(){
    return mySettings->value("Job/Address", "").toString();
}

void atpIni::set_jobAddres(QString value){
    mySettings->setValue("Job/Address", value);
}

QString atpIni::get_jobPostcode(){
    return mySettings->value("Job/Postcode", "").toString();
}

void atpIni::set_jobPostcode(QString value){
    mySettings->setValue("Job/Postcode", value);
}

QString atpIni::get_jobReference(){
    return mySettings->value("Job/Reference", "").toString();
}

void atpIni::set_jobReference(QString value){
    mySettings->setValue("Job/Reference", value);
}

QString atpIni::get_siteName(){
    return mySettings->value("Job/Name", "").toString();
}

void atpIni::set_siteName(QString value){
    mySettings->setValue("Job/Name", value);
}

QString atpIni::get_dateStartJob(){
    return mySettings->value("Job/startDate", "").toString();
}

void atpIni::set_dateStartJob(QString value){
    mySettings->setValue("Job/startDate", value);
}

QString atpIni::get_dateEndJob(){
    return mySettings->value("Job/endDate", "").toString();
}

void atpIni::set_dateEndJob(QString value){
    mySettings->setValue("Job/endDate", value);
}
