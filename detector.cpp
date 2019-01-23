#include "detector.h"
#include "QSysInfo"
#include "videostreamer.h"

cv::RNG rng(1);


Detector::Detector(DBManager& dbmanager,AppSettings& settings )
{

    this->dbmanager = &dbmanager ;
    this->settings = &settings;

}
void Detector::stopDetector() {
    this->run = false ;
}


cv::Mat Detector::rotateImage (cv::Mat frame,double angle) {
    // get rotation matrix for rotating the image around its center in pixel coordinates
    cv::Point2f center((frame.cols-1)/2.0, (frame.rows-1)/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), frame.size(), angle).boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0,2) += bbox.width/2.0 - frame.cols/2.0;
    rot.at<double>(1,2) += bbox.height/2.0 - frame.rows/2.0;

    cv::Mat dst;
    cv::warpAffine(frame, dst, rot, bbox.size());
    return dst ;
}



// Fill the vector with random colors
void Detector::getRandomColors(std::vector<cv::Scalar>& colors, int numColors)
{

    for(int i=0; i < numColors; i++)
        colors.push_back(cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)));
}

// Retrun a random RGB color
cv::Scalar Detector::getRandomColors()
{
    return cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
}

// Process detection results of front camera
void Detector::processDetectionFront(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) {
    // Scan all detection results
    for (uint idx = 0 ; idx < car_occupants.size(); idx++ ) {

        // if the area of detection is small or car's ROI have over lap with the first camera ROI,
        // it save the result on database and remove it from array
        if (car_occupants[idx].ROI.area()<30000 || (ROI_Camera_1 & car_occupants[idx].ROI )!=car_occupants[idx].ROI)
        {
//          qDebug() << car_occupants[idx].NextDriver.Confidence << car_occupants[idx].Driver.Confidence ;

            // Compare confidence of next driver occupant confidence with 0
            if (settings->getSetting(settings->KEY_DETECT_DRIVER).toBool() && car_occupants[idx].NextDriver.Confidence!=0 )
                car_occupants[idx].FrontOccupantNumber+= 1 ;

            // Check weather detecting driver is enable or not
            // Compare confidence of driver confidence with 0
            if (car_occupants[idx].Driver.Confidence!=0 )
                car_occupants[idx].FrontOccupantNumber+= 1 ;

            // Store data on DB
            dbmanager->insertResult(car_occupants[idx]) ;

            // Remove saved result from occupants array
            car_occupants.erase(car_occupants.begin()+ idx);

        }
    }

    // Check weather the detected car already exist in the occupants array or not
    bool car_exist = false ;
    for (auto detection_result:car_detection_results){
        if (detection_result.ClassName==car) {

            bool exist= false ;
            for (auto& car_occupant:car_occupants){
                if ((car_occupant.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/1.5)) {
                    exist=true;
                    car_occupant.ROI = detection_result.ROI;
                    car_occupant.ConfidentCar = detection_result.Confidence ;
                    car_exist = true ;
                    break;
                }
            }
            // the car is already has not been detected
            if(!exist && detection_result.ROI.area()>=ROI_Camera_1.area()/20 && (ROI_Camera_1 & detection_result.ROI)==detection_result.ROI){

                CarOccupancy newCar ;
                newCar.ROI =detection_result.ROI ;
                newCar.Color =getRandomColors() ;
                image(detection_result.ROI).copyTo(newCar.FrontCarImage);
                newCar.PickImage = false ;
                newCar.FrontOccupantNumber= settings->getSetting(settings->KEY_DETECT_DRIVER).toBool() ? 0 : 1 ;
                newCar.ConfidentCar = detection_result.Confidence ;
                car_occupants.push_back(newCar);

                car_exist = true ;
            }

        }

    }

    if (car_occupants.size()==0)
        return ;
    for (auto& occupancy:car_occupants){

        for (auto windows_result:windows_detection_results){

            // check window is in the car area
            if (windows_result.ClassName==wind_window && (windows_result.ROI & occupancy.ROI).area()>windows_result.ROI.area()/2) {
                double last_confidence = 0 ;
                //                DetectionResult* car_result;
                for (auto detection_result:car_detection_results){
                    if (detection_result.ClassName==occupant) {

                        // check occupant is in wind area
                        if ((windows_result.ROI & detection_result.ROI).area() > detection_result.ROI.area()/2) {


                            if((detection_result.ROI.x + detection_result.ROI.width/2) > (windows_result.ROI.x+windows_result.ROI.width/2) ){

                                if(detection_result.Confidence > occupancy.NextDriver.Confidence ) {
                                    last_confidence += occupancy.NextDriver.Confidence ;
                                    occupancy.NextDriver.Confidence = detection_result.Confidence;
                                    occupancy.NextDriver.ROI = detection_result.ROI;
                                    image(detection_result.ROI).copyTo(occupancy.NextDriver.Image);


                                }
                                occupancy.NextDriver.ROI = detection_result.ROI ;
                            }


                            else {

                                if(detection_result.Confidence > occupancy.Driver.Confidence  ){
                                    last_confidence += occupancy.Driver.Confidence ;
                                    occupancy.Driver.Confidence = detection_result.Confidence;
                                    occupancy.Driver.ROI = detection_result.ROI;
                                    image(detection_result.ROI).copyTo(occupancy.Driver.Image);
                                }
                                occupancy.Driver.ROI = detection_result.ROI ;


                            }
                        }

                    }

                }
                if(last_confidence != 0 && last_confidence < (occupancy.Driver.Confidence + occupancy.NextDriver.Confidence))
                {
                    for (auto detection_result:car_detection_results){
                        if (detection_result.ClassName==car && (occupancy.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/1.5)) {
                            cv::Mat temp ;
                            image(detection_result.ROI).copyTo(occupancy.FrontCarImage) ;
                            image.copyTo(temp) ;
                            cv::rectangle(temp, occupancy.Driver.ROI, settings->getSettingColor(settings->KEY_COLOR_OCCUPANT),2);
                            cv::rectangle(temp, occupancy.NextDriver.ROI, settings->getSettingColor(settings->KEY_COLOR_OCCUPANT),2);
                            temp(detection_result.ROI).copyTo(occupancy.FrontCarImageProcessed) ;

                        }
                    }

                }


            }
        }
    }

}

// Process detection results of back camera
void Detector::processDetectionBack(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) {


    for (auto& occupancy:car_occupants){
        for (auto windows_result:windows_detection_results){

            // check window is in the car area
            if (windows_result.ClassName==back_rear) {
                // calculate new confidence to replace back image with the new image
                double new_confidence = 0 ;
                // create an empty vector to store back occupants detection results
                std::vector <OccupantResult> back_occupants ;
                for (DetectionResult detection_result:car_detection_results){
                    if (detection_result.ClassName==occupant) {

                        // check occupant is in the window area
                        if ((windows_result.ROI & detection_result.ROI).area() > detection_result.ROI.area()/2) {
                            // store new detection result in back_occupants array
                            OccupantResult temp ;
                            temp.Image = image(detection_result.ROI);
                            temp.ROI = detection_result.ROI ;
                            temp.Confidence = detection_result.Confidence  ;
                            back_occupants.push_back(temp);
                            new_confidence += (double) detection_result.Confidence ;
                        }

                    }

                }
                // check weather the new detection result is more accurate from the last result
                // if so it will replace the old result with the new one
                if(new_confidence != 0 && new_confidence > occupancy.GetBackOcuupanceConfidence())
                {
                    occupancy.BackOccupants = back_occupants ;
                    occupancy.BackOccupantNumber = back_occupants.size() ;
                    for (auto detection_result:car_detection_results){
                        if (detection_result.ClassName==car) {
                            cv::Mat temp ;
                            image(detection_result.ROI).copyTo(occupancy.BackCarImage) ;
                            image.copyTo(temp) ;
                            for(OccupantResult backOccupancy : occupancy.BackOccupants) {
                                cv::rectangle(temp, backOccupancy.ROI, settings->getSettingColor(settings->KEY_COLOR_OCCUPANT),2);
                            }
                            temp(detection_result.ROI).copyTo(occupancy.BackCarImageProcessed) ;

                        }
                    }

                }


            }
        }
    }

}



void Detector::drawResult(cv::Mat& frame,std::vector<CarOccupancy> &car_occupants )
{


    //    qDebug() << car_occupants.size() ;
    for (auto car_occupant:car_occupants){
        //        if (!car_occupant.CarImage.empty()) {
        //            cv::Mat resized_car ;
        //            cv::resize(car_occupant.CarImage,resized_car,cv::Size(50,50)) ;
        //            cv::copyMakeBorder(resized_car,resized_car,3,3,3,3,cv::BORDER_CONSTANT,car_occupant.Color);
        //            cv::Mat destinationROI = frame( cv::Rect(frame.cols-60,70*idx,resized_car.cols,resized_car.rows) );
        //            resized_car.copyTo( destinationROI );
        //            cv::putText(frame,std::to_string(car_occupant.OccupantNumber),cv::Point(frame.cols-30,(70*idx)+25),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,1,cv::Scalar(0,255,0),2) ;
        //            idx++ ;
        //        }

        //        rectangle(frame, car_occupant.ROI, settings->getSettingColor(settings->KEY_COLOR_CAR),4);
        rectangle(frame, car_occupant.ROI, car_occupant.Color,4);
        //        rectangle(frame, car_occupant.Driver.ROI, settings->getSettingColor(settings->KEY_COLOR_OCCUPANT),4);
        //        rectangle(frame, car_occupant.NextDriver.ROI, settings->getSettingColor(settings->KEY_COLOR_OCCUPANT),4);


        //        if(cv::countNonZero(car_occupant.driver_detected)==numberOfDetection)
        //            cv::circle(frame,cv::Point(car_occupant.ROI.x+10,car_occupant.ROI.y-10),5,cv::Scalar(0,0,255),-1);
        //        if(cv::countNonZero(car_occupant.next_driver_detected)==numberOfDetection)
        //            cv::circle(frame,cv::Point(car_occupant.ROI.x+30,car_occupant.ROI.y-10),5,cv::Scalar(0,0,255),-1);

    }

}

//void Detector::drawResult(cv::Mat& frame,std::vector<CarOccupant> &car_occupants )
//{


//    int idx = 0 ;
//    for (auto car_occupant:car_occupants){
//        if (!car_occupant.CarImage.empty()) {
//            cv::Mat resized_car ;
//            cv::resize(car_occupant.CarImage,resized_car,cv::Size(50,50)) ;
//            cv::copyMakeBorder(resized_car,resized_car,3,3,3,3,cv::BORDER_CONSTANT,car_occupant.Color);
//            cv::Mat destinationROI = frame( cv::Rect(frame.cols-60,70*idx,resized_car.cols,resized_car.rows) );
//            resized_car.copyTo( destinationROI );
//            cv::putText(frame,std::to_string(car_occupant.OccupantNumber),cv::Point(frame.cols-30,(70*idx)+25),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,1,cv::Scalar(0,255,0),2) ;
//            idx++ ;
//        }

//        rectangle(frame, car_occupant.ROI, car_occupant.Color,4);
//        if(cv::countNonZero(car_occupant.driver_detected)==numberOfDetection)
//            cv::circle(frame,cv::Point(car_occupant.ROI.x+10,car_occupant.ROI.y-10),5,cv::Scalar(0,0,255),-1);
//        if(cv::countNonZero(car_occupant.next_driver_detected)==numberOfDetection)
//            cv::circle(frame,cv::Point(car_occupant.ROI.x+30,car_occupant.ROI.y-10),5,cv::Scalar(0,0,255),-1);

//    }

//}

// Draw detection results
void Detector::drawPred(std::vector<DetectionResult> detection_results, cv::Mat& frame,int border , bool show_conf, cv::Point base )
{
    for(DetectionResult detection_result:detection_results) {
        // Set rectangle color
        cv::Scalar color ;
        switch (detection_result.ClassName) {
        case car:
            color = settings->getSettingColor(settings->KEY_COLOR_CAR) ;
            break;
        case occupant:
            color = settings->getSettingColor(settings->KEY_COLOR_OCCUPANT)  ;
            break;
        case wind_window:
            color = settings->getSettingColor(settings->KEY_COLOR_WINDOW)  ;
            break;
        case front_rear:
            color = front_rear_color ;
            break;
        case back_rear:
            color = settings->getSettingColor(settings->KEY_COLOR_WINDOW_BACK)  ;
            break;
        }
        if(base.x!=0 || base.y!=0) {
            cv::Rect draw_rect = {detection_result.ROI.x+base.x,detection_result.ROI.y+base.y
                                  ,detection_result.ROI.width,detection_result.ROI.height} ;
            cv::rectangle(frame, draw_rect , color,border);
        }
        else
            cv::rectangle(frame, detection_result.ROI , color,border);

        // show detection confidence
        if(show_conf){
            std::string label = cv::format("%.2f", detection_result.Confidence);
            int baseLine;
            cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

            int top = cv::max(detection_result.ROI.y, labelSize.height);
            cv::rectangle(frame, cv::Point(detection_result.ROI.x, top - labelSize.height),cv::Point(detection_result.ROI.x + labelSize.width, top + baseLine), cv::Scalar::all(255) , cv::FILLED);
            cv::putText(frame, label, cv::Point(detection_result.ROI.x, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar());
        }
    }


}



int Detector::runDetector(cv::String source1,cv::String source2){
    // create objects from detectors
    CarDetector cardetector(*settings);
    WindowsDetector windowsdetector(*settings) ;
    OccupantDetector occupantdetector(*settings) ;

    // set environment variable to use GPU for decoding video stream
    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "video_codec;h264_cuvid|rtsp_transport;tcp");

    // create VideoCapture objects for capturing video steams
    cv::VideoCapture cap_1(source1,cv::CAP_FFMPEG);

    // return with error code 1
    if (!cap_1.isOpened())
        return 1 ;

    // set the first camera width and height variables
    frame_width_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_HEIGHT);

    // set the first camera ROI
    cv::Point a;
    cv::Point b;
    ROI_Camera_1 = {cv::Point(ROI_PAD,ROI_PAD),
                    cv::Point(frame_width_camera_1  - (ROI_PAD*2),
                    frame_height_camera_1 - (ROI_PAD*2))} ;

    // initialize the second camera
    cv::VideoCapture cap_2(source2,cv::CAP_FFMPEG);

    // check the second camera status
    bool second_camera = cap_2.isOpened() ;
    if (second_camera) {
        // set the second camera width and height variables
        frame_width_camera_2 = cap_2.get(cv::CAP_PROP_FRAME_WIDTH);
        frame_height_camera_2 = cap_2.get(cv::CAP_PROP_FRAME_HEIGHT);

        // set the second camera ROI
        ROI_Camera_2 = {cv::Point(ROI_PAD,ROI_PAD),
                        cv::Point(frame_width_camera_2  - (ROI_PAD*2),
                        frame_height_camera_2 - (ROI_PAD*2))} ;
    }

    // create an empty vector for storing detection result
    std::vector<CarOccupancy> car_occupancy ;
    run=true ;

    // create empty Mat for storing frame for cameras
    cv::Mat frame_1,frame_2
            ,frame_concat;
    for ( ;; ){
        if(!run) {
            cv::destroyAllWindows() ;
            return 0 ;


        }
        // return with error code 2
        if(!cap_1.grab()) {
            qDebug() << "Can not grab images from camera 1." << endl;
            return 2;
        }

        cap_1.retrieve( frame_1 ) ;


        // stop the program if there is no more images
        if(frame_1.rows==0 ){
            cv::destroyAllWindows() ;
            return 0 ;
        }

        // Run tiny-yolo car detector on the input frame
//        std::vector<DetectionResult> car_results_camera_1 =
//                cardetector.detect(frame_1);

        // Create empty vectors for front and back windows detection results
        std::vector<DetectionResult> windows_results_front,windows_results_back ;

        // Process the input frame to detect front windows
        windows_results_front = windowsdetector.detect(frame_1);

        // Create empty vectors for front and back occupants results
        std::vector<DetectionResult> occupant_results_front , occupant_results_back;

        // Process the input frame from the first camera to detect front occupants
        occupant_results_front = occupantdetector.detect(frame_1);

        // Process detection result
        processDetectionFront(occupant_results_front,windows_results_front,car_occupancy,frame_1) ;

        // Check wheater there is the second camera or not
        if(second_camera) {

            // if there is no response from the second camera retrun 4 and exit
            if(!cap_2.grab()) {
                qDebug() << "Can not grab images from camera 2." << endl;
                return 4;
            }

            // get frame from the second camera
            cap_2.retrieve( frame_2 ) ;

            // check in the first camera exist just one car
            if(car_occupancy.size()==1){

                // detect back windows in frame from the second windows
                windows_results_back = windowsdetector.detect(frame_2);
                int is_back_rear = 0 ;

                // check there is only one back windows in the image
                for(DetectionResult window_result:windows_results_back){
                    if (window_result.ClassName==back_rear){
                        is_back_rear += 1 ;
                    }
                }

                if (is_back_rear==1) {

                    // Detect back occupants
                    occupant_results_back = occupantdetector.detect(frame_2);

                    // Process detection result
                    processDetectionBack(occupant_results_back,windows_results_back,car_occupancy,frame_2) ;
                }

            }

            // Draw detection results on the output frame
            drawPred(windows_results_front,frame_1,2,true);
            drawPred(occupant_results_front,frame_1,2,true);
            drawPred(windows_results_back,frame_2,2,true);
            drawPred(occupant_results_back,frame_2,2,true);

            // concat frames from cameras into a single image
            cv::hconcat(frame_1,frame_2,frame_concat) ;

        }
        else {

            // Draw detection results on the output frame
            drawPred(windows_results_front,frame_1,2,true);
            drawPred(occupant_results_front,frame_1,2,true);
            frame_1.copyTo(frame_concat) ;
        }


        //cv::rectangle(frame_concat,ROI,cv::Scalar(0,255,0),5) ;

        // Show output frame
        imshow("tracker",frame_concat);
        qApp->processEvents();

        //quit on ESC button
        if(cv::waitKey(1)==27){
            cv::destroyAllWindows();
            break;
        }
    }
    return 0;
}


