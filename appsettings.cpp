#include "appsettings.h"
#include "QDebug"
AppSettings::AppSettings()
{
    settingsFilePath = QDir::currentPath() + "/settings.ini";
    settings = new QSettings(settingsFilePath, QSettings::IniFormat);
    loadSettings() ;

}
// get setting value by key
QVariant AppSettings::getSetting(QString key) {
        return settings->value(key) ;
}
// get colors from setting by key
cv::Scalar AppSettings::getSettingColor(QString key) {
    return qcolor2scalar(settings->value(key).value<QColor>()) ;
}
// set setting value by key
void AppSettings::setSetting(QString key,QVariant value) {
    settings->setValue(key,value) ;
}
// load settings
void AppSettings::loadSettings() {
    //set default setting
    if (settings->value(KEY_OCCUPANT_CFG).toString()=="")
        settings->setValue(KEY_OCCUPANT_CFG, QString(QDir::currentPath() + "/Model/spp/yolov3-spp.cfg"));

    if (settings->value(KEY_OCCUPANT_WEIGHTS).toString()=="")
        settings->setValue(KEY_OCCUPANT_WEIGHTS, QString(QDir::currentPath() + "/Model/spp/yolov3-spp.weights"));

    if (settings->value(KEY_TINY_CFG).toString()=="")
        settings->setValue(KEY_TINY_CFG, QString(QDir::currentPath() + "/Model/tiny/yolov3-tiny.cfg"));

    if (settings->value(KEY_TINY_WEIGHTS).toString()=="")
        settings->setValue(KEY_TINY_WEIGHTS, QString(QDir::currentPath() + "/Model/tiny/yolov3-tiny.weights"));

    if (settings->value(KEY_WINDOWS_CFG).toString()=="")
        settings->setValue(KEY_WINDOWS_CFG, QString(QDir::currentPath() + "/Model/windows/yolov3-tiny.cfg"));

    if (settings->value(KEY_WINDOWS_WEIGHTS).toString()=="")
        settings->setValue(KEY_WINDOWS_WEIGHTS, QString(QDir::currentPath() +  "/Model/windows/yolov3-tiny_20000.weights"));

    if (settings->value(KEY_SAVE_VIDEOS_PATH).toString()=="")
        settings->setValue(KEY_SAVE_VIDEOS_PATH, QString(QDir::currentPath() +  "/"));


    if (settings->value(KEY_WINDOW_THREADSHOLD).toString()=="")
        settings->setValue(KEY_WINDOW_THREADSHOLD, 0.3);

    if (settings->value(KEY_CAR_THREADSHOLD).toString()=="")
        settings->setValue(KEY_CAR_THREADSHOLD, 0.5);

    if (settings->value(KEY_TINY_CAR_THREADSHOLD).toString()=="")
        settings->setValue(KEY_TINY_CAR_THREADSHOLD, 0.3);

    if (settings->value(KEY_OCCUPANT_THREADSHOLD).toString()=="")
        settings->setValue(KEY_OCCUPANT_THREADSHOLD, 0.2);

    if (settings->value(KEY_SOURCE_1).toString()=="")
        settings->setValue(KEY_SOURCE_1, QString("/home/pouya/Trafic.MP4"));

    if (settings->value(KEY_SOURCE_2).toString()=="")
        settings->setValue(KEY_SOURCE_2, "");

    if (settings->value(KEY_COLOR_CAR).toString()=="")
        settings->setValue(KEY_COLOR_CAR, QColor(255,0,0));

    if (settings->value(KEY_COLOR_OCCUPANT).toString()=="")
        settings->setValue(KEY_COLOR_OCCUPANT, QColor(0,255,0));

    if (settings->value(KEY_COLOR_WINDOW).toString()=="")
        settings->setValue(KEY_COLOR_WINDOW, QColor(0,0,255));

    if (settings->value(KEY_COLOR_WINDOW_BACK).toString()=="")
        settings->setValue(KEY_COLOR_WINDOW_BACK, QColor(0,255,255));

    if (settings->value(KEY_SAVE_VIDEOS).toString()=="")
        settings->setValue(KEY_SAVE_VIDEOS, false);

    if (settings->value(KEY_DETECT_DRIVER).toString()=="")
        settings->setValue(KEY_DETECT_DRIVER, false);

    if (settings->value(KEY_USE_GPU_FOR_DECODE).toString()=="")
        settings->setValue(KEY_USE_GPU_FOR_DECODE, false);





}
// swap RGB-->BGR
cv::Scalar AppSettings::qcolor2scalar(QColor color)
{
    int r,g,b;
    color.getRgb(&r, &g, &b);
    return cv::Scalar(b,g,r);
}
// swap RGB-->BGR
QColor AppSettings::scalar2qcolor(cv::Scalar color)
{
    return QColor(color[2],color[1],color[0]);
}
