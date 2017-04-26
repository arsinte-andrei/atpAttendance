#ifndef ATPSETTINGS_H
#define ATPSETTINGS_H

#include <QDialog>
#include <atpdb.h>
#include <atpini.h>
#include "qdevicewatcher.h"

namespace Ui {
class atpSettings;
}
//class QDeviceWatcher;
class atpSettings : public QDialog {
    Q_OBJECT

public:
    explicit atpSettings(atpDb *dbParent, QWidget *parent = 0);
    ~atpSettings();

private slots:
    void on_buttonCancel_clicked();
    void on_buttonAccept_clicked();
    void on_buttoneSelectDb_clicked();
    void on_buttonOpenDb_clicked();

    void on_editJobRef_textEdited(const QString &arg1);
    void on_editSiteName_textEdited(const QString &arg1);
    void on_editAddres_textChanged();
    void on_editPostcode_textEdited(const QString &arg1);
    void on_dateJobStart_dateChanged(const QDate &date);
    void on_dateJobFinish_dateChanged(const QDate &date);
    void on_spinSeconds_valueChanged(int arg1);

    void slotDeviceAdded(QString devName);
    void slotDeviceRemoved(QString devName);

    void on_buttoneDbExport_clicked();

private:
    Ui::atpSettings *ui;
    atpIni *myIniSettings;
    atpDb *myDb;
    int spinValue;
    bool modified, b_infoDelay, b_JobName, b_Address, b_jobRef, b_jobPostcode, b_jobStart, b_jobEnd;
    QDeviceWatcher *watcher;
};

#endif // ATPSETTINGS_H
