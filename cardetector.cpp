#include "cardetector.h"

CarDetector::CarDetector()
{
    net = cv::dnn::readNetFromDarknet(modelConfigurationTiny, modelBinaryTiny);
}

std::vector<DetectionResult> CarDetector::detect(cv::Mat frame) {
    cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1 / 255.F , cv::Size(416, 416), cv::Scalar(), true, false); //Convert Mat to batch of images
    net.setInput(inputBlob);
    std::vector<cv::Mat> outs;
    net.forward(outs, getOutputsNames(net));  //compute output
    std::vector<DetectionResult> detection_results = postprocess(frame, outs);
    return detection_results;

}
std::vector<DetectionResult> CarDetector::postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs ) {
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

            if (confidence > TINY_CAR_THREADSHOLD && classIdPoint.x==2)
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
    cv::dnn::NMSBoxes(boxes, confidences, TINY_CAR_THREADSHOLD, 0.4f, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        cv::Rect ROI = cv::Rect(box.x > 0 ? box.x : 0 ,
                                box.y > 0 ? box.y : 0 ,
                                box.width + (box.x > 0 ? box.x : 0) > frame.cols ? frame.cols-(box.x > 0 ? box.x : 0) : box.width,
                                box.height+ (box.y > 0 ? box.y : 0) > frame.rows ? frame.rows-(box.y > 0 ? box.y : 0) : box.height
                                                                                   ) ;

        detection_result.push_back({ROI,car,confidences[idx]});
    }
    return detection_result ;
}