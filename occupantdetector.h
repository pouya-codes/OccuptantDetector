#ifndef OCCUPANTDETECTOR_H
#define OCCUPANTDETECTOR_H
#include <objectdetector.h>

class OccupantDetector : public ObjectDetector
{
public:
    OccupantDetector();
    std::vector<DetectionResult> detect(cv::Mat frame) ;
    std::vector<DetectionResult> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs ) ;
private:
    cv::dnn::Net net ;
};

#endif // OCCUPANTDETECTOR_H
