#ifndef DETECTOR_H
#define DETECTOR_H
#include <setting.h>
#include <asmOpenCV.h>
#include <QLabel>
#include <QApplication>
struct CarOccupant {
    //    CarOccupant() : driver_detected(true) , next_driver_detected(false) {}
    cv::Rect ROI ;
    cv::Scalar Color ;
    int OccupantNumber ;
    cv::Mat driver_detected =cv::Mat(numberOfDetection,1,CV_8U, cv::Scalar(detectDriver? 0 : 1));
    //    bool driver_detected = true ;
    cv::Mat next_driver_detected = cv::Mat(numberOfDetection,1,CV_8U, cv::Scalar(0));
    cv::Mat CarImage ;
    float ConfidentCar ;

};
struct DetectionResult {
    //    CarOccupant() : driver_detected(true) , next_driver_detected(false) {}
    cv::Rect ROI ;
    int ClassIdx ;
    float Confidence ;
};

class Detector
{

public:
    Detector(cv::String source,QSqlDatabase* db);
    void stopDetector() ;
    void resumeDetector() ;
private :
    cv::Mat rotateImage (cv::Mat frame,double angle) ;
    void getRandomColors(std::vector<cv::Scalar>& colors, int numColors);
    cv::Scalar getRandomColors() ;
    void processDetection(std::vector<DetectionResult> detection_results,std::vector<CarOccupant>& car_occupants,cv::Mat image) ;
    void drawResult(cv::Mat& frame,std::vector<CarOccupant> &car_occupants ) ;
    void drawPred(int classId, float conf, int left, int top, int right, int bottom,cv::Mat& frame,cv::Scalar color );
    std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net);
    void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs,float confThreshold,std::vector<DetectionResult>& detection_result);
    void postprocessCar(cv::Mat& frame,cv::Mat& input, const std::vector<cv::Mat>& outs,float confThreshold,int classidx,cv::Rect carRect);
    int runDetector() ;
    void insertResult(CarOccupant occupant);
    bool isThereAnyCar(cv::Mat image) ;
    bool postprocesstiny(const std::vector<cv::Mat>& outs) ;

    std::string source ;
    cv::dnn::Net net_spp ;
    cv::dnn::Net net_tiny ;
    std::vector<std::string> classes;
    QLabel* image ;
    bool run ;
    QSqlDatabase *db ;
    cv::Rect ROI ;
    int pad = 25 ;

//    cv::RNG rng(1);

};

#endif // DETECTOR_H
