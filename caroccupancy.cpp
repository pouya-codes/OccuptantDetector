#include "caroccupancy.h"

CarOccupancy::CarOccupancy()
{


}
double CarOccupancy::GetBackOcuupanceConfidence() {
    double confidence = 0 ;
    for (OccupantResult result:BackOccupants)
        confidence+=result.Confidence ;
    return confidence ;

}

