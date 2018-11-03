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

win32 {
INCLUDEPATH += includes

LIBS += C:\opencv\opencv\build\install\x86\mingw\bin\libopencv_core400.dll
LIBS += C:\opencv\opencv\build\install\x86\mingw\bin\libopencv_highgui400.dll
LIBS += C:\opencv\opencv\build\install\x86\mingw\bin\libopencv_imgproc400.dll
LIBS += C:\opencv\opencv\build\install\x86\mingw\bin\libopencv_videoio400.dll
LIBS += C:\opencv\opencv\build\install\x86\mingw\bin\libopencv_dnn400.dll
LIBS += C:\opencv\opencv\build\install\x86\mingw\bin\libopencv_imgcodecs400.dll
LIBS += C:\opencv\opencv\build\install\x86\mingw\bin\libopencv_objdetect400.dll
}
