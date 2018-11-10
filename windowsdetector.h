#ifndef WINDOWSDETECTOR_H
#define WINDOWSDETECTOR_H
#include <appsettings.h>
#include <objectdetector.h>
class WindowsDetector : public ObjectDetector
{
public:
    WindowsDetector(AppSettings& settings );
    std::vector<DetectionResult> detect(cv::Mat frame) ;
    std::vector<DetectionResult> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs ) ;
private:
    cv::dnn::Net net ;
    AppSettings* settings ;
};

#endif // WINDOWSDETECTOR_H
