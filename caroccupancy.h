#ifndef CAROCCUPANT_H
#define CAROCCUPANT_H
#include "setting.h"
#include "objectdetector.h"

struct OccupantResult {
    //    CarOccupant() : driver_detected(true) , next_driver_detected(false) {}
    cv::Mat Image ;
    cv::Rect ROI ;
//    ClassNames ClassName ;
    float Confidence = 0 ;
};


class CarOccupancy
{

public:
    CarOccupancy(cv::Mat front_image= cv::Mat(),cv::Mat back_image=cv::Mat());

    int FrontOccupantNumber ;
    int BacktOccupantNumber ;

    OccupantResult Driver,NextDriver ;

    cv::Scalar Color ;
    float ConfidentCar ;
    cv::Rect ROI ;
    cv::Mat FrontCarImage ;
    cv::Mat FrontCarImageProcessed ;
    cv::Mat BackCarImage ;
    cv::Mat BackCarImageProcessed ;

    bool PickImage  ;

private :
    void DetectFrontOccupant () ;
    void DetectBackOccupant () ;

    std::vector <DetectionResult> FrontOccupantROIs ;
    std::vector <DetectionResult> BackOccupantROIs ;


};

#endif // CAROCCUPANT_H
