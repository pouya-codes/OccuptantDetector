#include "cardetector.h"

CarDetector::CarDetector(AppSettings& settings)
{
    net = new Detector(settings.getSetting(settings.KEY_TINY_CFG).toString().toStdString()
                                      ,settings.getSetting(settings.KEY_TINY_WEIGHTS).toString().toStdString());
    this->settings = &settings ;
}

std::vector<DetectionResult> CarDetector::detect(cv::Mat frame) {
    std::vector<bbox_t> result_vec = net->detect(frame, 0.1);
    std::vector<DetectionResult> detection_results = postprocess(result_vec);
    return detection_results;

}
std::vector<DetectionResult> CarDetector::postprocess(std::vector<bbox_t> result_vec) {


    std::vector<DetectionResult> detection_result ;
    for (auto &i : result_vec) {
        if (i.prob > settings->getSetting(settings->KEY_TINY_CAR_THREADSHOLD).toDouble() && (i.obj_id==2 || i.obj_id==7 ))
        {
            cv::Rect ROI = cv::Rect(i.x, i.y, i.w, i.h) ;
            detection_result.push_back({ROI,car,i.prob});

            }
        }

    return detection_result ;
}
