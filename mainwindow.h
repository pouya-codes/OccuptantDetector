#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mydetector.h>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QtSql/qsqldatabase.h>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <appsettings.h>
#include <appsettingsdialog.h>
#include <dbmanager.h>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_Stop_clicked();
    void on_tableViewSelectionModel_currentRowChanged(QModelIndex index1,QModelIndex index2);
//    void on_tableViewSelectionModel_currentRowChanged(QModelIndex index1,QModelIndex index2);


    void updateData() ;
    void on_rb_orginal_clicked();

    void on_rb_processed_clicked();

    void front_image_clicked();
    void back_image_clicked();

    void on_pb_save_images_clicked();

    void on_comboBox_date_currentIndexChanged(const QString &arg1);

private:
    QString table_date ;
    void queryData(QString date);
    void setPictures() ;
    void connectToDB() ;
    int selected_row_id ;
    Ui::MainWindow *ui;
    MyDetector* detector ;
    AppSettings* settings ;
    DBManager* dbmanager ;
    QPixmap Front_Image_Raw, Back_Image_Raw, Front_Image_Processed, Back_Image_Processed ;
    QTimer * timer;
    QString initalDate ;




//    QSqlDatabase *db ;
};

#endif // MAINWINDOW_H
