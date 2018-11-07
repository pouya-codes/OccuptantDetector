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


//static cv::String modelConfiguration = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-spp.cfg";
//static cv::String modelBinary = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-spp.weights";


static cv::String modelConfigurationTiny = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-tiny.cfg";
static cv::String modelBinaryTiny = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-tiny.weights";

//static cv::String modelConfiguration = "c:/yolov3-spp.cfg";
//static cv::String modelBinary = "c:/yolov3-spp.weights";

static cv::String modelConfiguration = "/home/pouya/Develop/Sarbazi/yolo/darknet/Train/yolov3-tiny.cfg";
static cv::String modelBinary = "/media/pouya/My Passport/yolov3-tiny_20000.weights";



static float confidentThredshold = 0.2 ;
static bool detectDriver = false ;
static int numberOfDetection = 3 ;
const QString DRIVER("QSQLITE");
#endif // SETTING

