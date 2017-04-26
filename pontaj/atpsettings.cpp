#include "atpsettings.h"
#include "ui_atpsettings.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDateTime>


atpSettings::atpSettings(atpDb *dbParent, QWidget *parent) : QDialog(parent), ui(new Ui::atpSettings) {
    ui->setupUi(this);

    myIniSettings = new atpIni;
    myDb = dbParent;

    b_infoDelay = false; b_JobName = false; b_Address = false; b_jobRef = false;
    b_jobPostcode = false; b_jobStart = false; b_jobEnd = false; modified = false;

    ui->editDbPath->setText(myIniSettings->get_dbPath());
    ui->spinSeconds->setValue(myIniSettings->get_infoDelay());
    ui->editAddres->setText(myIniSettings->get_jobAddres());
    ui->editJobRef->setText(myIniSettings->get_jobReference());
    ui->editPostcode->setText(myIniSettings->get_jobPostcode());
    ui->editSiteName->setText(myIniSettings->get_siteName());
    ui->dateJobStart->setDate(QDate::fromString(myIniSettings->get_dateStartJob(), "dd/MM/yyyy"));
    ui->dateJobFinish->setDate(QDate::fromString(myIniSettings->get_dateEndJob(), "dd/MM/yyyy"));


    watcher = new QDeviceWatcher;
    watcher->appendEventReceiver(this);
    connect(watcher, SIGNAL(deviceAdded(QString)), this, SLOT(slotDeviceAdded(QString)), Qt::DirectConnection);
    connect(watcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slotDeviceRemoved(QString)), Qt::DirectConnection);

    watcher->isRunning();
    watcher->start();

    ui->buttoneDbExport->setEnabled(false);

}

atpSettings::~atpSettings() {
    delete ui;
}

void atpSettings::on_buttonCancel_clicked() {
    this->reject();
}

void atpSettings::on_buttonAccept_clicked() {
    if(modified){
        if(b_infoDelay){myIniSettings->set_infoDelay(ui->spinSeconds->value());}
        if(b_JobName){myIniSettings->set_siteName(ui->editSiteName->text());}
        if(b_jobRef){myIniSettings->set_jobReference(ui->editJobRef->text());}
        if(b_jobPostcode){myIniSettings->set_jobPostcode(ui->editPostcode->text());}
        if(b_Address){myIniSettings->set_jobAddres(ui->editAddres->toPlainText());}
        if(b_jobStart){myIniSettings->set_dateStartJob(ui->dateJobStart->text());}
        if(b_jobEnd){myIniSettings->set_dateEndJob(ui->dateJobFinish->text());}
    }
    this->accept();
}

void atpSettings::on_buttoneSelectDb_clicked() {
    ui->editDbPath->clear();
    QFileDialog *mySaveFileDialog = new QFileDialog(this,
                                                    tr("Open or creaye new atp dataBase file"),
                                                     QDir::currentPath(),
                                                     tr("atp dataBase(*.atpDb)"));
    mySaveFileDialog->setFileMode(QFileDialog::AnyFile);
    mySaveFileDialog->setViewMode(QFileDialog::Detail);
    mySaveFileDialog->setNameFilter(tr("atp dataBase(*.atpDb)"));
    mySaveFileDialog->setDefaultSuffix("atpDb");
    mySaveFileDialog->setAcceptMode(QFileDialog::AcceptSave);
    mySaveFileDialog->exec();
    QStringList dbFileName = mySaveFileDialog->selectedFiles();

    qDebug() << "lista save open: " << dbFileName;

   if(mySaveFileDialog->Accepted && !dbFileName.isEmpty()){
        ui->editDbPath->setText(dbFileName[0]);
        if(atpDb::atp_create_new_atpdb(dbFileName[0])){
            myIniSettings->set_dbPath(dbFileName[0]);
            myDb->setupNewConnection();
        }
    }

}

void atpSettings::on_buttonOpenDb_clicked(){
    QString oldPath = ui->editDbPath->text();
    QString dbFileName = QFileDialog::getOpenFileName(this,
                               tr("Find dataBase file"),
                                QDir::currentPath(),
                                tr("atp dataBase(*.atpDb)"));

    if (!dbFileName.isEmpty() && oldPath != dbFileName) {
        ui->editDbPath->setText(dbFileName);

        myIniSettings->set_dbPath(dbFileName);
        myDb->setupNewConnection();

        qDebug() << "nu e gol si e diferit";
    }
}

void atpSettings::on_editJobRef_textEdited(const QString &arg1){
    Q_UNUSED(arg1);
    modified = true;
    b_jobRef = true;
}

void atpSettings::on_editSiteName_textEdited(const QString &arg1){
    Q_UNUSED(arg1);
    modified = true;
    b_JobName = true;
}

void atpSettings::on_editAddres_textChanged(){
    modified = true;
   b_Address = true;
}

void atpSettings::on_editPostcode_textEdited(const QString &arg1){
    Q_UNUSED(arg1);
    modified = true;
    b_jobPostcode = true;
}

void atpSettings::on_dateJobStart_dateChanged(const QDate &date){
    Q_UNUSED(date);
    modified = true;
   b_jobStart = true;
}

void atpSettings::on_dateJobFinish_dateChanged(const QDate &date){
    Q_UNUSED(date);
    modified = true;
    b_jobEnd = true;
}

void atpSettings::on_spinSeconds_valueChanged(int arg1){
    Q_UNUSED(arg1);
    modified = true;
    b_infoDelay = true;
}

void atpSettings::slotDeviceAdded(QString devName){
    ui->lbl_device->setText(devName);
    ui->buttoneDbExport->setEnabled(true);
}

void atpSettings::slotDeviceRemoved(QString devName){
    ui->lbl_device->setText(devName);
    ui->buttoneDbExport->setEnabled(false);
}

void atpSettings::on_buttoneDbExport_clicked(){
    bool exportOK = false;
    ui->lbl_device->setText("Do not remove the USB drive!!!");
    QString currentDbPath = myIniSettings->get_dbPath();
    if (currentDbPath.endsWith(".atpDb")){
        qDebug() << "in if";
        QString backupDbPath = currentDbPath;
        backupDbPath.remove(".atpDb");
        backupDbPath.append(QDateTime::currentDateTime().toString("dd-MM-yyyy hh-mm-ss"));
        backupDbPath.append(".atpDb");
        qDebug() << currentDbPath << " *** " << backupDbPath;
        if(QFile::copy(currentDbPath, backupDbPath)){
            ui->lbl_device->setText("Copy in progress...");
            myIniSettings->set_dbPath(backupDbPath);
            if(myDb->setupNewConnection()){
                myDb->atpDelete("1", "tbl_attendance", "time_worked IS NULL OR time_worked = ''");
                QFileInfo fi(backupDbPath);
                QString usbFileName = "/media/usb/";
                usbFileName.append(fi.fileName());
                if(QFile::copy(backupDbPath, usbFileName)){
                    ui->lbl_device->setText("Export finished!");
                    exportOK = true;
                }
            }
        }
    }
    myIniSettings->set_dbPath(currentDbPath);
    if(myDb->setupNewConnection()){
        if(exportOK){
            myDb->atpDelete("1", "tbl_attendance", "time_worked <> ''");
            ui->lbl_device->setText("Export 100% finished!");
        }
    }

}
