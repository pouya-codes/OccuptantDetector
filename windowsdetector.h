#ifndef WINDOWSDETECTOR_H
#define WINDOWSDETECTOR_H
#include <appsettings.h>
#include <objectdetector.h>
class WindowsDetector : public ObjectDetector
{
public:
    WindowsDetector(AppSettings& settings );
    std::vector<DetectionResult> detect(cv::Mat frame) ;
    std::vector<DetectionResult> postprocess(std::vector<bbox_t> result_vec) ;
private:
    Detector* net ;
    AppSettings* settings ;
};

#endif // WINDOWSDETECTOR_H
