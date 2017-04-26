#include "atplogdetails.h"
#include "ui_atplogdetails.h"

atpLogDetails::atpLogDetails(atpDb *dbParent, QWidget *parent) : QDialog(parent), ui(new Ui::atpLogDetails){
    ui->setupUi(this);
    myDb = dbParent;
    myModel = new QSqlTableModel;

    myModel->setTable("tbl_logs");
    myModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    myModel->setHeaderData(5, Qt::Horizontal,"Time");
    myModel->setHeaderData(7, Qt::Horizontal,"First Name");
    myModel->setHeaderData(8, Qt::Horizontal,"Second Name");
    myModel->setHeaderData(9, Qt::Horizontal,"Acction");
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(myModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(1, true);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->setColumnHidden(4, true);
    ui->tableView->setColumnHidden(6, true);
    ui->tableView->setColumnWidth(5, 250);
    ui->tableView->setColumnWidth(7, 150);
    ui->tableView->setColumnWidth(8, 150);
    ui->tableView->setColumnWidth(9, 150);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
    myModel->select();


}

atpLogDetails::~atpLogDetails(){
    delete ui;
}

void atpLogDetails::on_buttonOk_clicked(){
    this->accept();
}
