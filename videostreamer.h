#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H
#include "appsettings.h"
#include "QString"
#include "opencv2/videoio.hpp"

class VideoStreamer
{
private:
    cv::Mat frame ;
    cv::VideoCapture* cap;
    cv::Size videoSize;
public:
    cv::Mat get_frame() ;
    int StartCapture(cv::String source) ;
};

#endif // VIDEOSTREAMER_H
