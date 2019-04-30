#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
//#include "xlsxdocument.h"

// Set images when TableView row changed
void MainWindow::on_tableViewSelectionModel_currentRowChanged(QModelIndex index1, QModelIndex index2){
    // get new row index
    selected_row_id = index1.sibling(index1.row(), 0).data().toInt();
    // get pictures from db
    DBManager::DetectionImages images = dbmanager->getPicture(selected_row_id,table_date) ;
    // enable save button
    ui->pb_save_images->setEnabled(true) ;
    // set Images
    Front_Image_Raw = images.Front_Image_Raw ;
    Back_Image_Raw = images.Back_Image_Raw ;
    Front_Image_Processed = images.Front_Image_Processed ;
    Back_Image_Processed = images.Back_Image_Processed ;
    setPictures() ;

}


// Query detction results for selected date
void MainWindow::queryData(QString date) {
    ui->tableView->setModel(dbmanager->getDataModel(date));
    QItemSelectionModel *sm = ui->tableView->selectionModel();
    connect(sm,SIGNAL(currentRowChanged(QModelIndex,QModelIndex) ),this,SLOT(on_tableViewSelectionModel_currentRowChanged(QModelIndex,QModelIndex)) );
}

// Set detection pictures
void MainWindow::setPictures() {
    if (ui->rb_orginal->isChecked()){
        ui->lable_front_image->setPixmap(Front_Image_Raw.scaled(ui->lable_front_image->width()-5,ui->lable_front_image->height()-5));
        ui->lable_back_image->setPixmap(Back_Image_Raw.scaled(ui->lable_back_image->width()-5,ui->lable_back_image->height()-5));
    }
    else{
        ui->lable_front_image->setPixmap(Front_Image_Processed.scaled(ui->lable_front_image->width()-5,ui->lable_front_image->height()-5));
        ui->lable_back_image->setPixmap(Back_Image_Processed.scaled(ui->lable_back_image->width()-5,ui->lable_back_image->height()-5));
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QXlsx::Document xlsx;
//    xlsx.renameSheet("Sheet1","aa") ;
//    xlsx.write("A1", "سلام");
//    xlsx.saveAs("D:/Test.xlsx");

    // Create AppSettings object
    settings = new AppSettings() ;

    // Connect image lable signals to slots
    connect(ui->lable_front_image, SIGNAL(clicked()), this, SLOT (front_image_clicked()));
    connect(ui->lable_back_image , SIGNAL(clicked()), this, SLOT (back_image_clicked()));

//    connect to the database
    connectToDB() ;

    // changes program theme
//    QFile style_file("/home/pouya/darkorange.stylesheet");
//    if(style_file.open(QIODevice::ReadOnly))
//        this->setStyleSheet(style_file.readAll());


}
void MainWindow::connectToDB() {
    // Create DBManager object
    dbmanager = new DBManager(settings->getSetting(settings->KEY_DATABASE_TYPE).toString());
    dbmanager->openDatabase(settings->getSetting(settings->KEY_SERVER_ADDRESS).toString(),
                            settings->getSetting(settings->KEY_SERVER_USER).toString(),
                            settings->getSetting(settings->KEY_SERVER_PASSWORD).toString());

    // Showing today date on the statusBar
    initalDate = dbmanager->currentDateTimeJalali().split(' ')[0] ;
    ui->statusBar->showMessage(initalDate);

    // Adding table names to combobox
    ui->comboBox_date->clear();
    ui->comboBox_date->addItems(dbmanager->GetTableNames());
    ui->comboBox_date->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
// update data
void MainWindow::updateData() {
    QString date = dbmanager->currentDateTimeJalali().split(' ')[0] ;
    if(date != initalDate ) {
        ui->comboBox_date->clear();
        ui->comboBox_date->addItems(dbmanager->GetTableNames());
        ui->comboBox_date->setCurrentIndex(0);
        initalDate = date ;
    }
    queryData(ui->comboBox_date->currentText());
}

// run decoder
void MainWindow::on_pushButton_clicked()
{
    // Start a timer to fetch inserted results every 2 seconds
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
    timer->start(2000);

    detector = new MyDetector(*dbmanager,*settings);
    detector->runDetector(settings->getSetting(settings->KEY_SOURCE_1).toString().toStdString(),settings->getSetting(settings->KEY_SOURCE_2).toString().toStdString()) ;

    timer->stop();
    queryData(ui->comboBox_date->currentText());

}
// Open Program Setting
void MainWindow::on_pushButton_Stop_clicked()
{
    AppSettingsDialog *objMyNewDialog;
    objMyNewDialog= new AppSettingsDialog(*settings,this);
    objMyNewDialog->setModal(true) ;
    objMyNewDialog->exec();
    connectToDB() ;
}






void MainWindow::on_rb_orginal_clicked()
{
    setPictures() ;
}

void MainWindow::on_rb_processed_clicked()
{
    setPictures() ;
}
// handle click on front image
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
// handle click on back image
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

// save images on the disk
void MainWindow::on_pb_save_images_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,QDir::currentPath()) ;
    if (path!="" ){
        std::string out_path = QString(path +"/").toStdString() ;
            if (!Front_Image_Raw.isNull())
                cv::imwrite(out_path+std::to_string(selected_row_id)+"_Front_Orginal.jpg",ASM::QPixmapToCvMat(Front_Image_Raw)) ;
            if (!Back_Image_Raw.isNull())
                cv::imwrite(out_path+std::to_string(selected_row_id)+"_Back_Orginal.jpg",ASM::QPixmapToCvMat(Back_Image_Raw)) ;
            if (!Front_Image_Processed.isNull())
                cv::imwrite(out_path+std::to_string(selected_row_id)+"_Front_Processed.jpg",ASM::QPixmapToCvMat(Front_Image_Processed)) ;
            if (!Back_Image_Processed.isNull())
                cv::imwrite(out_path+std::to_string(selected_row_id)+"_Back_Processed.jpg",ASM::QPixmapToCvMat(Back_Image_Processed)) ;

    }

}



void MainWindow::on_comboBox_date_currentIndexChanged(const QString &arg1)
{
    table_date = arg1 ;
    queryData(arg1);
}
