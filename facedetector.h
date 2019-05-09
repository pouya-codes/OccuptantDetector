// This class inherits ObjectDetector and Detects car in the input frame

#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H
#include <objectdetector.h>
#include <appsettings.h>

class FaceDetector : public ObjectDetector
{
public:
    FaceDetector(AppSettings& settings);
    std::vector<DetectionResult> detect(cv::Mat frame) ;
    std::vector<DetectionResult> postprocess(std::vector<bbox_t> result_vec) ;
private:
    Detector* net ;
    AppSettings* settings ;
};

#endif // FACEDETECTOR_H
