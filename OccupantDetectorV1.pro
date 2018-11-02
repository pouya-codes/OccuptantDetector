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
    detector.cpp

HEADERS  += mainwindow.h \
    detector.h \
    setting.h \
    asmOpenCV.h

FORMS    += mainwindow.ui

unix {
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv
}
