#include "caroccupancy.h"

CarOccupancy::CarOccupancy(cv::Mat front_image,cv::Mat back_image)
{

}
double CarOccupancy::GetBackOcuupanceConfidence() {
    double confidence = 0 ;
    for (OccupantResult result:BackOccupants)
        confidence+=result.Confidence ;
    return confidence ;

}

