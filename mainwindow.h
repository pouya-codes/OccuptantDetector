#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <detector.h>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QtSql/qsqldatabase.h>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QSqlQueryModel>
#include <QSqlTableModel>

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

private:
    void queryData();
    void setPicture (int id);
    QSqlDatabase db ;
    void makeDatabase() ;
    Ui::MainWindow *ui;
    Detector *detector ;
    QSqlTableModel *dataModel;

//    QSqlDatabase *db ;
};

#endif // MAINWINDOW_H
