#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"



void MainWindow::on_tableViewSelectionModel_currentRowChanged(QModelIndex index1, QModelIndex index2){
    selected_row_id = index1.sibling(index1.row(), 0).data().toInt();
    DBManager::DetectionImages images = dbmanager->getPicture(selected_row_id) ;
    ui->pb_save_images->setEnabled(true) ;

    Front_Image_Raw = images.Front_Image_Raw ;
    Back_Image_Raw = images.Back_Image_Raw ;
    Front_Image_Processed = images.Front_Image_Processed ;
    Back_Image_Processed = images.Back_Image_Processed ;
    setPictures() ;

}



void MainWindow::queryData() {
    ui->tableView->setModel(dbmanager->getDataModel());
    QItemSelectionModel *sm = ui->tableView->selectionModel();
    connect(sm,SIGNAL(currentRowChanged(QModelIndex,QModelIndex) ),this,SLOT(on_tableViewSelectionModel_currentRowChanged(QModelIndex,QModelIndex)) );
}

void MainWindow::setPictures() {
    if (ui->rb_orginal->isChecked()){
        ui->lable_front_image->setPixmap(Front_Image_Raw);
        ui->lable_back_image->setPixmap(Back_Image_Raw);
    }
    else{
        ui->lable_front_image->setPixmap(Front_Image_Processed);
        ui->lable_back_image->setPixmap(Back_Image_Processed);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new AppSettings() ;
    dbmanager = new DBManager();
    queryData();
    connect(ui->lable_front_image, SIGNAL(clicked()), this, SLOT (front_image_clicked()));
    connect(ui->lable_back_image , SIGNAL(clicked()), this, SLOT (back_image_clicked()));

//    QFile style_file("/home/pouya/darkorange.stylesheet");
//    if(style_file.open(QIODevice::ReadOnly))
//        this->setStyleSheet(style_file.readAll());


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
    detector = new Detector(*dbmanager,*settings);
    detector->runDetector(settings->getSetting(settings->KEY_SOURCE_1).toString().toStdString(),settings->getSetting(settings->KEY_SOURCE_2).toString().toStdString()) ;
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

}


void MainWindow::on_rb_orginal_clicked()
{
    setPictures() ;
}

void MainWindow::on_rb_processed_clicked()
{
    setPictures() ;
}
void MainWindow::front_image_clicked(){
    cv::Mat image;
    if (ui->rb_orginal->isChecked())
        image = ASM::QPixmapToCvMat(Front_Image_Raw);
    else
        image = ASM::QPixmapToCvMat(Front_Image_Processed);
    if (!image.empty()) {
        cv::imshow("Front Image",image);
        if (cv::waitKey(0))
            cv::destroyAllWindows() ;
    }
}
void MainWindow::back_image_clicked(){
    cv::Mat image;
    if (ui->rb_orginal->isChecked())
        image = ASM::QPixmapToCvMat(Back_Image_Raw);
    else
        image = ASM::QPixmapToCvMat(Back_Image_Processed);
    if (!image.empty()) {
        cv::imshow("Back Image",image);
        if (cv::waitKey(0))
            cv::destroyAllWindows() ;
    }

}

void MainWindow::on_pb_save_images_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,QDir::currentPath()) ;
    if (path!="" ){
        std::string out_path = QString(path +"/").toStdString() ;
        if (ui->rb_orginal->isChecked()){
            cv::imwrite(out_path+std::to_string(selected_row_id)+"_Front_Orginal.jpg",ASM::QPixmapToCvMat(Front_Image_Raw)) ;
            cv::imwrite(out_path+std::to_string(selected_row_id)+"_Back_Orginal.jpg",ASM::QPixmapToCvMat(Back_Image_Raw)) ;
        }
        else
        {
            cv::imwrite(out_path+std::to_string(selected_row_id)+"_Front_Processed.jpg",ASM::QPixmapToCvMat(Front_Image_Processed)) ;
            cv::imwrite(out_path+std::to_string(selected_row_id)+"_Back_Processed.jpg",ASM::QPixmapToCvMat(Back_Image_Processed)) ;
        }
    }

}
