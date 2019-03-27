#ifndef OCCUPANTDETECTOR_H
#define OCCUPANTDETECTOR_H
#include <objectdetector.h>
#include <appsettings.h>


class OccupantDetector : public ObjectDetector
{
public:
    OccupantDetector(AppSettings& settings);
    std::vector<DetectionResult> detect(cv::Mat frame) ;
    std::vector<DetectionResult> postprocess(std::vector<bbox_t> result_vec ) ;
private:
    Detector* net ;
    AppSettings* settings ;

};

#endif // OCCUPANTDETECTOR_H
