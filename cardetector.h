#ifndef CARDETECTOR_H
#define CARDETECTOR_H
#include <objectdetector.h>

class CarDetector : public ObjectDetector
{
public:
    CarDetector();
    std::vector<DetectionResult> detect(cv::Mat frame) ;
    std::vector<DetectionResult> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs ) ;
private:
    cv::dnn::Net net ;
};

#endif // CARDETECTOR_H
