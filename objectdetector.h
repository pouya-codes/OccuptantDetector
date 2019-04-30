#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H
#define OPENCV

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <QString>
#include "yolo_v2_class.hpp"


enum ClassNames {wind_window,front_rear,back_rear,car,occupant} ;

struct DetectionResult {
    //    CarOccupant() : driver_detected(true) , next_driver_detected(false) {}
    cv::Rect ROI ;
    ClassNames ClassName ;
    float Confidence ;
    int track_id = -1 ;
};



class ObjectDetector
{
public:
    virtual std::vector<DetectionResult> detect(cv::Mat frame) = 0;
    virtual std::vector<DetectionResult> postprocess(std::vector<bbox_t> result_vec) = 0;
    virtual ~ObjectDetector() {}

    cv::Mat equalizeIntensity(const cv::Mat& inputImage)
    {
        if(inputImage.channels() >= 3)
        {
            cv::Mat ycrcb;

            cv::cvtColor(inputImage,ycrcb,cv::COLOR_BGR2YCrCb);

            std::vector<cv::Mat> channels;
            cv::split(ycrcb,channels);

            cv::equalizeHist(channels[0], channels[0]);

            cv::Mat result;
            cv::merge(channels,ycrcb);

            cv::cvtColor(ycrcb,result,cv::COLOR_YCrCb2BGR);

            return result;
        }
        return cv::Mat();
    }



};

#endif // OBJECTDETECTOR_H

