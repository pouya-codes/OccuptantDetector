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


static cv::String modelConfigurationTiny = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-tiny.cfg";
static cv::String modelBinaryTiny = "/home/pouya/Develop/Sarbazi/yolo/darknet/yolov3-tiny.weights";

//static cv::String modelConfiguration = "c:/yolov3-spp.cfg";
//static cv::String modelBinary = "c:/yolov3-spp.weights";

//static cv::String modelConfiguration = "/home/pouya/Develop/Sarbazi/yolo/darknet/Train/yolov3-tiny.cfg";
//static cv::String modelBinary = "/media/pouya/My Passport/yolov3-tiny_20000.weights";

static cv::String modelConfigurationWindows = "/home/pouya/Develop/Sarbazi/yolo/darknet/Train/yolov3-tiny.cfg";
static cv::String modelBinaryWindows  = "/media/pouya/My Passport/yolov3-tiny_20000.weights";


static float WINDOW_THREADSHOLD = 0.3 ;
static float CAR_THREADSHOLD = 0.8 ;
static float TINY_CAR_THREADSHOLD = 0.3 ;
static float OCCUPANT_THREADSHOLD = 0.1 ;

static cv::Scalar car_color = cv::Scalar(255,0,0);
static cv::Scalar occupant_color = cv::Scalar(0,255,0);
static cv::Scalar windwindow_color = cv::Scalar(0,0,255);
static cv::Scalar back_rear_color = cv::Scalar(100,100,100);
static cv::Scalar front_rear_color = cv::Scalar(200,200,200);


static float confidentThredshold = 0.2 ;
static bool detectDriver = false ;
static int numberOfDetection = 3 ;
static int ROI_PAD = 20 ;
const QString DRIVER("QSQLITE");
#endif // SETTING

