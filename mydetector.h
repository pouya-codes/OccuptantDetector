#ifndef DETECTOR_H
#define DETECTOR_H
#include <asmOpenCV.h>
#include <QLabel>
#include <QApplication>
#include "QBuffer"
#include "cardetector.h"
#include "windowsdetector.h"
#include "occupantdetector.h"
#include "dbmanager.h"

#include <thread>





class MyDetector
{

public:
    MyDetector(DBManager& dbmanager,AppSettings& settings);
    int runDetector(cv::String source1,cv::String source2) ;

private :
    cv::Mat rotateImage (cv::Mat frame,double angle) ;
    void getRandomColors(std::vector<cv::Scalar>& colors, int numColors);
    cv::Scalar getRandomColors() ;

    void processDetectionFront(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) ;
    void processDetectionBack(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) ;

    void drawPred(std::vector<DetectionResult> detection_results, cv::Mat& frame,int border = 1 , bool show_conf =false, cv::Point base = cv::Point()) ;
    int frame_width_camera_1 ;
    int frame_height_camera_1 ;
    int frame_width_camera_2 ;
    int frame_height_camera_2 ;

    AppSettings* settings ;


    std::vector<std::string> classes;
    cv::Size image_size ;
    QLabel* image ;
    bool run ;
    DBManager *dbmanager ;
    cv::Rect ROI_Camera_1,ROI_Camera_2 ;


    float calcBlurriness(const cv::Mat &frame);


};

#endif // DETECTOR_H
