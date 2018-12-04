#include "videostreamer.h"
#include "QDebug"

int VideoStreamer::StartCapture(cv::String source) {
    cap = new cv::VideoCapture(source,cv::CAP_FFMPEG) ;
    videoSize.width = cap->get(cv::CAP_PROP_FRAME_WIDTH) ;
    videoSize.height = cap->get(cv::CAP_PROP_FRAME_HEIGHT) ;

    if (!cap->isOpened())
        return 1 ;
    while (true) {
        cap->grab() ;
        cap->retrieve( frame ) ;
    }
}
cv::Mat VideoStreamer::get_frame() {

    return frame ;
}
