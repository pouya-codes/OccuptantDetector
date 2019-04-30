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
include(qtxlsx/src/xlsx/qtxlsx.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    windowsdetector.cpp \
    cardetector.cpp \
    occupantdetector.cpp \
    caroccupancy.cpp \
    appsettingsdialog.cpp \
    appsettings.cpp \
    clickablelabel.cpp \
    dbmanager.cpp \
    qdateconvertor.cpp \
    mydetector.cpp

HEADERS  += mainwindow.h \
    asmOpenCV.h \
    windowsdetector.h \
    objectdetector.h \
    cardetector.h \
    occupantdetector.h \
    caroccupancy.h \
    appsettingsdialog.h \
    appsettings.h \
    clickablelabel.h \
    dbmanager.h \
    qdateconvertor.h \
    mydetector.h

FORMS    += mainwindow.ui \
    appsettingsdialog.ui

unix {
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4
}

win32 {
#INCLUDEPATH += D:/Develop/OpenCV/BulidMSCV4/install/include

INCLUDEPATH += D:/Develop/OpenCV/opencv-4.1.0/build/install/include
INCLUDEPATH += D:/Develop/darknet/include
LIBS += -LD:/Develop/darknet/build/darknet/x64/ \
        -lyolo_cpp_dll
LIBS += -LD:/Develop/OpenCV/opencv-4.1.0/build/install/x64/vc15/lib \
-lopencv_core410 \
-lopencv_imgproc410 \
-lopencv_highgui410 \
-lopencv_imgcodecs410 \
#-lopencv_dnn410 \
-lopencv_videoio410 \
-lopencv_cudacodec410

#LIBS += -LD:/Develop/OpenCV/buildMSVC4.0.1/bin/Debug

}
