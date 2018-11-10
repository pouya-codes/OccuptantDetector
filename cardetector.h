#ifndef CARDETECTOR_H
#define CARDETECTOR_H
#include <objectdetector.h>
#include <appsettings.h>

class CarDetector : public ObjectDetector
{
public:
    CarDetector(AppSettings& settings);
    std::vector<DetectionResult> detect(cv::Mat frame) ;
    std::vector<DetectionResult> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs ) ;
private:
    cv::dnn::Net net ;
    AppSettings* settings ;
};

#endif // CARDETECTOR_H
