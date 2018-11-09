#ifndef CAROCCUPANT_H
#define CAROCCUPANT_H
#include "setting.h"
#include "objectdetector.h"

class CarOccupancy
{
public:
    CarOccupancy(cv::Mat front_image= cv::Mat(),cv::Mat back_image=cv::Mat());
private :
    void DetectFrontOccupant () ;
    void DetectBackOccupant () ;

    std::vector <DetectionResult> FrontOccupantROIs ;
    std::vector <DetectionResult> BackOccupantROIs ;

    int FrontOccupantNumber ;
    int BacktOccupantNumber ;

    cv::Mat FrontCarImage ;
    cv::Mat BackCarImage ;
};

#endif // CAROCCUPANT_H
