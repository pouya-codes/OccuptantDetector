#include "windowsdetector.h"

WindowsDetector::WindowsDetector(AppSettings& settings)
{
    net =new Detector(settings.getSetting(settings.KEY_WINDOWS_CFG).toString().toStdString()
                                      ,settings.getSetting(settings.KEY_WINDOWS_WEIGHTS).toString().toStdString());
    this->settings = &settings ;

}

std::vector<DetectionResult> WindowsDetector::detect(cv::Mat frame) {
    std::vector<bbox_t> result_vec = net->detect(frame, 0.1);
    std::vector<DetectionResult> detection_results = postprocess(result_vec);
    return detection_results;

}
std::vector<DetectionResult> WindowsDetector::postprocess(std::vector<bbox_t> result_vec) {

    std::vector<DetectionResult> detection_result ;
    for (auto &i : result_vec) {
        if (i.prob > settings->getSetting(settings->KEY_WINDOW_THREADSHOLD).toDouble())
        {
            cv::Rect ROI = cv::Rect(i.x, i.y, i.w, i.h) ;
            switch (i.obj_id) {
            case 0:
                detection_result.push_back({ROI,wind_window,i.prob});
                break;
            case 1:
                detection_result.push_back({ROI,front_rear,i.prob});
                break;
            case 2:
                detection_result.push_back({ROI,back_rear,i.prob});
                break;
            }
        }

    }
    return detection_result ;

//    return detection_result ;
//    std::vector<int> classIds;
//    std::vector<float> confidences;
//    std::vector<cv::Rect> boxes;
//    std::vector<DetectionResult> detection_result ;
//    for (size_t i = 0; i < outs.size(); ++i)
//    {
//        float* data = (float*)outs[i].data;
//        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
//        {
//            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
//            cv::Point classIdPoint;
//            double confidence;
//            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

//            if (confidence > settings->getSetting(settings->KEY_WINDOW_THREADSHOLD).toDouble() )
//            {
//                int centerX = (int)(data[0] * frame.cols);
//                int centerY = (int)(data[1] * frame.rows);
//                int width = (int)(data[2] * frame.cols);
//                int height = (int)(data[3] * frame.rows);
//                int left = centerX - width / 2;
//                int top = centerY - height / 2;

//                cv::Rect ROI_Detected = cv::Rect(left, top, width, height);

//                classIds.push_back(classIdPoint.x);
//                confidences.push_back((float)confidence);
//                boxes.push_back(ROI_Detected);
//            }
//        }
//    }
//    std::vector<int> indices;
//    cv::dnn::NMSBoxes(boxes, confidences, settings->getSetting(settings->KEY_WINDOW_THREADSHOLD).toDouble(), 0.4f, indices);
//    for (size_t i = 0; i < indices.size(); ++i)
//    {
//        int idx = indices[i];
//        cv::Rect box = boxes[idx];
//        cv::Rect ROI = cv::Rect(box.x > 0 ? box.x : 0 ,
//                                box.y > 0 ? box.y : 0 ,
//                                box.width + (box.x > 0 ? box.x : 0) > frame.cols ? frame.cols-(box.x > 0 ? box.x : 0) : box.width,
//                                box.height+ (box.y > 0 ? box.y : 0) > frame.rows ? frame.rows-(box.y > 0 ? box.y : 0) : box.height
//                                                                                   ) ;


//    }
//    return detection_result ;
}
