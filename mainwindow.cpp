#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"




void MainWindow::on_tableViewSelectionModel_currentRowChanged(QModelIndex index1, QModelIndex index2){
    int id = index1.sibling(index1.row(), 0).data().toInt();
    setPicture(id) ;
}



void MainWindow::queryData() {
    //    editSelectedRowId = -2 ;
        dataModel = new QSqlTableModel();
//        connect(dataModel, SIGNAL( dataChanged(QModelIndex,QModelIndex,QVector<int>)), SLOT(handleAfterEdit(QModelIndex,QModelIndex,QVector<int>)));

        dataModel->setTable("result");
        dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //    if (ui->lineEditSearch->text()!="") {
    //        QString filter_txt = "code like '" + ui->lineEditSearch->text() + "%'";
    //        dataModel->setFilter(filter_txt);
    //    }

        dataModel->select();
        dataModel->removeColumn(3);

        dataModel->setHeaderData(0, Qt::Orientation::Horizontal, tr("ID"));
        dataModel->setHeaderData(1,  Qt::Orientation::Horizontal, tr("Occupant"));
        dataModel->setHeaderData(2,  Qt::Orientation::Horizontal, tr("Date"));
    //    dataModel->setHeaderData(3,  Qt::Orientation::Horizontal, tr("Date"));



        ui->tableView->setModel(dataModel);
        QItemSelectionModel *sm = ui->tableView->selectionModel();
        connect(sm,SIGNAL(currentRowChanged(QModelIndex,QModelIndex) ),this,SLOT(on_tableViewSelectionModel_currentRowChanged(QModelIndex,QModelIndex)) );
}

void MainWindow::makeDatabase() {
    db = QSqlDatabase::addDatabase(DRIVER) ;
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
//        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName("results.db");
        if(!db.open())
          qWarning() << "ERROR: " << db.lastError();
        QSqlQuery query("CREATE TABLE IF NOT EXISTS result (id INTEGER PRIMARY KEY AUTOINCREMENT,occupant INTEGER,date varchar(50), imagedata BLOB)");
        if(!query.isActive())
            qWarning() << "ERROR: " << query.lastError().text();

    }
    settings = new AppSettings() ;


}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    makeDatabase() ;
    queryData();

//        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    detector = new Detector(ui->lineEdit_url->text().toStdString(),&db,*settings);
    makeDatabase();
    queryData();
}

void MainWindow::on_pushButton_Stop_clicked()
{
    AppSettingsDialog *objMyNewDialog;
    objMyNewDialog= new AppSettingsDialog(*settings,this);
    objMyNewDialog->setModal(true) ;
    objMyNewDialog->show();
}

void MainWindow::setPicture (int id) {
//    if (running) return ;
//    int selectedRowId = id ;
    QString querytxt ;
    querytxt = "SELECT imagedata FROM result WHERE id = ?";
    QSqlQuery query;
    query.prepare(querytxt);
    query.bindValue(0, id);
    query.exec() ;
    query.first() ;

    QByteArray outByteArray = query.value( 0 ).toByteArray();
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData( outByteArray );
    ui->label->setPixmap(outPixmap);


}

void MainWindow::on_pushButton_Browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "Select a file", QDir::currentPath() ,"Videos (*.mp4);;All Files (*)");

    if(fileName!="") {
        ui->lineEdit_url->setText(fileName);

    }
}
