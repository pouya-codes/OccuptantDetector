#ifndef SETTING
#define SETTING
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <time.h>
#include <QDateTime>
#include <QDir>



static const std::string currentDateTime() {
    QDateTime a = QDateTime::currentDateTime() ;
    return a.toString("yyyy-MM-dd hh,mm,ss,zzz").toStdString();
}

//const cv::String modelConfiguration = QString(QDir::currentPath() + "/Model/spp/yolov3-spp.cfg").toStdString();
//const cv::String modelBinary =QString(QDir::currentPath() + "/Model/spp/yolov3-spp.weights").toStdString();


//const cv::String modelConfigurationTiny = QString(QDir::currentPath() + "/Model/tiny/yolov3-tiny.cfg").toStdString();
//const cv::String modelBinaryTiny = QString(QDir::currentPath() + "/Model/tiny/yolov3-tiny.weights").toStdString();

//const cv::String modelConfigurationWindows = QString(QDir::currentPath() + "/Model/windows/yolov3-tiny.cfg").toStdString();
//const cv::String modelBinaryWindows  = QString(QDir::currentPath() +  "/Model/windows/yolov3-tiny_20000.weights").toStdString() ;


//const float WINDOW_THREADSHOLD = 0.3 ;
//const float CAR_THREADSHOLD = 0.5 ;
//const float TINY_CAR_THREADSHOLD = 0.3 ;
//const float OCCUPANT_THREADSHOLD = 0.1 ;

//const cv::Scalar car_color = cv::Scalar(255,0,0);
//const cv::Scalar occupant_color = cv::Scalar(0,255,0);
//const cv::Scalar windwindow_color = cv::Scalar(0,0,255);
const cv::Scalar back_rear_color = cv::Scalar(0,0,200);
const cv::Scalar front_rear_color = cv::Scalar(0,200,0);

const float confidentThredshold = 0.2 ;
const bool detectDriver = false ;
const int numberOfDetection = 3 ;
const int ROI_PAD = 20 ;
const QString DRIVER("QSQLITE");
#endif // SETTING

