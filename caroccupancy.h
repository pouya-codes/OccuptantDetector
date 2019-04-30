#ifndef CAROCCUPANT_H
#define CAROCCUPANT_H
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
    CarOccupancy();
    double GetBackOcuupanceConfidence () ;

    int FrontOccupantNumber ;
    int BackOccupantNumber ;

    OccupantResult Driver,NextDriver ;
    std::vector<OccupantResult> BackOccupants ;

    cv::Scalar Color ;
    float ConfidentCar ;
    cv::Rect ROI ;
    cv::Mat FrontCarImage ;
    cv::Mat FrontCarImageProcessed ;
    cv::Mat BackCarImage ;
    cv::Mat BackCarImageProcessed ;

    float FrontCarImageBlurriness ;
    float BackCarImageBlurriness ;

    bool PickImage  ;

    int track_id ;

    int missed_frame = 3 ;




};

#endif // CAROCCUPANT_H
