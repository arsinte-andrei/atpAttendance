#ifndef ATPLOGDETAILS_H
#define ATPLOGDETAILS_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <atpdb.h>

namespace Ui {
class atpLogDetails;
}

class atpLogDetails : public QDialog {
    Q_OBJECT

public:
    explicit atpLogDetails(atpDb *dbParent, QWidget *parent = 0);
    ~atpLogDetails();

private slots:
    void on_buttonOk_clicked();

private:
    Ui::atpLogDetails *ui;
    QSqlTableModel *myModel;
    QSortFilterProxyModel *proxyModel;
    atpDb *myDb;
};

#endif // ATPLOGDETAILS_H
