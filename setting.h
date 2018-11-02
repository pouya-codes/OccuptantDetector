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

static const std::string currentDateTime() {
    QDateTime a = QDateTime::currentDateTime() ;
    return a.toString("yyyy-MM-dd hh,mm,ss,zzz").toStdString();
}


static cv::String modelConfiguration = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-spp.cfg";
static cv::String modelBinary = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-spp.weights";
static float confidentThredshold = 0.1 ;
static bool detectDriver = false ;
static int numberOfDetection = 3 ;
const QString DRIVER("QSQLITE");
#endif // SETTING

