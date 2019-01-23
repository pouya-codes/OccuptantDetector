#include "occupantdetector.h"

OccupantDetector::OccupantDetector(AppSettings& settings)
{
     net = cv::dnn::readNetFromDarknet(settings.getSetting(settings.KEY_OCCUPANT_CFG).toString().toStdString()
                                       ,settings.getSetting(settings.KEY_OCCUPANT_WEIGHTS).toString().toStdString());
     net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
     net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);

     this->settings = &settings ;
}

std::vector<DetectionResult> OccupantDetector::detect(cv::Mat frame) {
    cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1 / 255.F , cv::Size(416, 416), cv::Scalar(), true, false); //Convert Mat to batch of images
    net.setInput(inputBlob);
    std::vector<cv::Mat> outs;
    net.forward(outs, getOutputsNames(net));  //compute output
    std::vector<DetectionResult> detection_results = postprocess(frame, outs);
    return detection_results;

}
std::vector<DetectionResult> OccupantDetector::postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs ) {
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<DetectionResult> detection_result ;
    for (size_t i = 0; i < outs.size(); ++i)
    {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (
                    (confidence > settings->getSetting(settings->KEY_OCCUPANT_THREADSHOLD).toDouble()  && classIdPoint.x==0)
                    ||
                    (confidence > settings->getSetting(settings->KEY_CAR_THREADSHOLD).toDouble()  && (classIdPoint.x==2 || classIdPoint.x==7 ))
                    )
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                cv::Rect ROI_Detected = cv::Rect(left, top, width, height);

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(ROI_Detected);
            }
        }
    }
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, settings->getSetting(settings->KEY_OCCUPANT_THREADSHOLD).toDouble() , 0.4f, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        cv::Rect ROI = cv::Rect(box.x > 0 ? box.x : 0 ,
                                box.y > 0 ? box.y : 0 ,
                                box.width + (box.x > 0 ? box.x : 0) > frame.cols ? frame.cols-(box.x > 0 ? box.x : 0) : box.width,
                                box.height+ (box.y > 0 ? box.y : 0) > frame.rows ? frame.rows-(box.y > 0 ? box.y : 0) : box.height
                                                                                   ) ;

        detection_result.push_back({ROI,classIds[idx]==0 ? occupant : car ,confidences[idx]});
    }
    return detection_result ;
}
