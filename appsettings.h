/* This class is defined to save and load program settings and variables */

#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#include <QSettings>
#include <QDir>
#include <opencv2/core.hpp>
#include <QColor>

class AppSettings
{
public:
    AppSettings();
    QVariant getSetting(QString key) ;
    cv::Scalar getSettingColor(QString key);
    void setSetting(QString key,QVariant value) ;

    QString KEY_OCCUPANT_WEIGHTS = "OCCUPANT_WEIGHTS" ;
    QString KEY_OCCUPANT_CFG     = "OCCUPANT_CFG" ;

    QString KEY_TINY_WEIGHTS = "TINY_WEIGHTS" ;
    QString KEY_TINY_CFG     = "TINY_CFG" ;

    QString KEY_WINDOWS_WEIGHTS = "WINDOWS_WEIGHTS" ;
    QString KEY_WINDOWS_CFG     = "WINDOWS_CFG" ;


    QString KEY_WINDOW_THREADSHOLD   = "WINDOW_THREADSHOLD" ;
    QString KEY_CAR_THREADSHOLD      = "CAR_THREADSHOLD" ;
    QString KEY_TINY_CAR_THREADSHOLD = "TINY_CAR_THREADSHOLD" ;
    QString KEY_OCCUPANT_THREADSHOLD = "OCCUPANT_THREADSHOLD" ;

    QString KEY_SOURCE_1 = "SOURCE_1" ;
    QString KEY_SOURCE_2 = "SOURCE_2" ;


    QString KEY_COLOR_CAR = "COLOR_CAR" ;
    QString KEY_COLOR_OCCUPANT= "COLOR_OCCUPANT" ;
    QString KEY_COLOR_WINDOW= "COLOR_WINDOWS" ;
    QString KEY_COLOR_WINDOW_BACK= "COLOR_WINDOW_BACK" ;

    QString KEY_USE_GPU_FOR_DECODE = "USE_GPU_FOR_DECODE" ;

    QString KEY_DETECT_DRIVER = "DETECT_DRIVER" ;
    QString KEY_SHOW_TIMES= "SHOW_TIMES" ;

    QString KEY_DATABASE_TYPE = "DATABASE_TYPE" ;
    QString KEY_SERVER_ADDRESS = "SERVER_ADDRESS" ;
    QString KEY_SERVER_USER = "SERVER_USER" ;
    QString KEY_SERVER_PASSWORD = "SERVER_PASSWORD" ;


private :
    void loadSettings();

    QSettings* settings ;
    QString settingsFilePath;




    const cv::Scalar car_color = cv::Scalar(255,0,0);
    const cv::Scalar occupant_color = cv::Scalar(0,255,0);
    const cv::Scalar windwindow_color = cv::Scalar(0,0,255);
    const cv::Scalar back_rear_color = cv::Scalar(100,100,100);
    const cv::Scalar front_rear_color = cv::Scalar(200,200,200);

    const float confidentThredshold = 0.2 ;
    const bool detectDriver = false ;
    const int numberOfDetection = 3 ;
    const int ROI_PAD = 20 ;


    cv::Scalar qcolor2scalar(QColor color);
    QColor scalar2qcolor(cv::Scalar color) ;

};

#endif // APPSETTINGS_H
