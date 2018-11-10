#-------------------------------------------------
#
# Project created by QtCreator 2018-11-01T16:20:27
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OccupantDetectorV1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    windowsdetector.cpp \
    detector.cpp \
    cardetector.cpp \
    occupantdetector.cpp \
    caroccupancy.cpp \
    appsettingsdialog.cpp \
    appsettings.cpp \
    clickablelabel.cpp

HEADERS  += mainwindow.h \
    detector.h \
    setting.h \
    asmOpenCV.h \
    windowsdetector.h \
    objectdetector.h \
    cardetector.h \
    occupantdetector.h \
    caroccupancy.h \
    appsettingsdialog.h \
    appsettings.h \
    clickablelabel.h

FORMS    += mainwindow.ui \
    appsettingsdialog.ui

unix {
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4
}

win32 {
INCLUDEPATH += D:/Develop/OpenCV/buildMSVC/install/include

LIBS += -LD:/Develop/OpenCV/buildMSVC/install/x64/vc15/lib \
-lopencv_core400 \
-lopencv_imgproc400 \
-lopencv_highgui400 \
-lopencv_imgcodecs400 \
-lopencv_dnn400 \
-lopencv_videoio400


}
