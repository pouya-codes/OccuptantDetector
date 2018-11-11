#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"



void MainWindow::on_tableViewSelectionModel_currentRowChanged(QModelIndex index1, QModelIndex index2){
    int id = index1.sibling(index1.row(), 0).data().toInt();
     ui->label->setPixmap(dbmanager->getPicture(id));
}



void MainWindow::queryData() {
        ui->tableView->setModel(dbmanager->getDataModel());
        QItemSelectionModel *sm = ui->tableView->selectionModel();
        connect(sm,SIGNAL(currentRowChanged(QModelIndex,QModelIndex) ),this,SLOT(on_tableViewSelectionModel_currentRowChanged(QModelIndex,QModelIndex)) );
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new AppSettings() ;
    dbmanager = new DBManager();
    queryData();

//        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    if(settings->getSetting(settings->KEY_SOURCE_1).toString()=="") {
//        QMessageBox messageBox;
//        messageBox.critical(0,"خطا","آدرس های ورودی را چک کنید.");
//        messageBox.setFixedSize(500,200);
//        return ;
//    }
    detector = new Detector(settings->getSetting(settings->KEY_SOURCE_1).toString().toStdString(),*dbmanager,*settings);
    queryData();
}

void MainWindow::on_pushButton_Stop_clicked()
{
    AppSettingsDialog *objMyNewDialog;
    objMyNewDialog= new AppSettingsDialog(*settings,this);
    objMyNewDialog->setModal(true) ;
    objMyNewDialog->show();
}



void MainWindow::on_pushButton_Browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "Select a file", QDir::currentPath() ,"Videos (*.mp4);;All Files (*)");

    if(fileName!="") {
        ui->lineEdit_url->setText(fileName);

    }
}
