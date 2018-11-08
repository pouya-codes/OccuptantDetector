#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H
#include <setting.h>

enum ClassNames {wind_window,front_rear,back_rear,car,occupant} ;

struct DetectionResult {
    //    CarOccupant() : driver_detected(true) , next_driver_detected(false) {}
    cv::Rect ROI ;
    ClassNames ClassName ;
    float Confidence ;
};



class ObjectDetector
{
public:
    virtual std::vector<DetectionResult> detect(cv::Mat frame) = 0;
    virtual std::vector<DetectionResult> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs ) = 0;
    virtual ~ObjectDetector() {}
    std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net)
    {
        static std::vector<cv::String> names;
    //    if (names.empty())
    //    {
            std::vector<int> outLayers = net.getUnconnectedOutLayers();
            std::vector<cv::String> layersNames = net.getLayerNames();
            names.resize(outLayers.size());
            for (size_t i = 0; i < outLayers.size(); ++i)
                names[i] = layersNames[outLayers[i] - 1];
    //    }
        return names;
    }
    cv::Mat equalizeIntensity(const cv::Mat& inputImage)
    {
        if(inputImage.channels() >= 3)
        {
            cv::Mat ycrcb;

            cv::cvtColor(inputImage,ycrcb,cv::COLOR_BGR2YCrCb);

            std::vector<cv::Mat> channels;
            cv::split(ycrcb,channels);

            cv::equalizeHist(channels[0], channels[0]);

            cv::Mat result;
            cv::merge(channels,ycrcb);

            cv::cvtColor(ycrcb,result,cv::COLOR_YCrCb2BGR);

            return result;
        }
        return cv::Mat();
    }



};

#endif // OBJECTDETECTOR_H

