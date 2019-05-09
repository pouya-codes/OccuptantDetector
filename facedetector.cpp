#include "facedetector.h"

FaceDetector::FaceDetector(AppSettings& settings)
{
    net = new Detector(QString(QDir::currentPath() + "/Model/face/yolov3-face.cfg").toStdString(),
                       QString(QDir::currentPath() + "/Model/face/yolov3-face.weights").toStdString());
    this->settings = &settings ;
     ;
}

std::vector<DetectionResult> FaceDetector::detect(cv::Mat frame) {
    std::vector<bbox_t> result_vec = net->detect(frame, 0.1);
    std::vector<DetectionResult> detection_results = postprocess(result_vec);
    return detection_results;
}
std::vector<DetectionResult> FaceDetector::postprocess(std::vector<bbox_t> result_vec) {
    std::vector<DetectionResult> detection_result ;
    for (auto &i : result_vec) {
        if (i.prob > settings->getSetting(settings->KEY_OCCUPANT_THREADSHOLD).toDouble())// || i.obj_id==7 ))
        {
            cv::Rect ROI = cv::Rect(i.x, i.y, i.w, i.h) ;
            detection_result.push_back({ROI,car,i.prob});
            }
        }
    return detection_result ;
}
