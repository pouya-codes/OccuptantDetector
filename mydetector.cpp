#include "mydetector.h"
#include "QSysInfo"

#include <opencv2/cudacodec.hpp>
#include <opencv2/core/opengl.hpp>

cv::RNG rng(1);


MyDetector::MyDetector(DBManager& dbmanager,AppSettings& settings )
{

    this->dbmanager = &dbmanager ;
    this->settings = &settings;

}



float MyDetector::calcBlurriness(const cv::Mat &frame)
{

    cv::Mat Gx, Gy;
    cv::Sobel(frame, Gx, CV_32F, 1, 0);
    cv::Sobel(frame, Gy, CV_32F, 0, 1);
    double normGx = norm(Gx);
    double normGy = norm(Gy);
    double sumSq = normGx*normGx + normGy*normGy;
    return static_cast<float>(1. / (sumSq / frame.size().area() + 1e-6));
}



cv::Mat MyDetector::rotateImage (cv::Mat frame,double angle) {
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
void MyDetector::getRandomColors(std::vector<cv::Scalar>& colors, int numColors)
{

    for(int i=0; i < numColors; i++)
        colors.push_back(cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)));
}

// Retrun a random RGB color
cv::Scalar MyDetector::getRandomColors()
{
    return cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
}

// Process detection results of front camera
void MyDetector::processDetectionFront(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) {
    // Scan all detection results
    for (uint idx = 0 ; idx < car_occupants.size(); idx++ ) {

        // if the area of detection is small or car's ROI has over lap with the first camera ROI,
        // it saves the result on database and remove it from array
        if (car_occupants[idx].ROI.area()<ROI_Camera_1.area()/20 || (ROI_Camera_1 & car_occupants[idx].ROI )!=car_occupants[idx].ROI)
        {

            car_occupants[idx].missed_frame -- ;
            if(car_occupants[idx].missed_frame==0) {
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
    }

    // Check weather the detected car already exist in the occupants array or not
    for (auto detection_result:car_detection_results){
        // Check the detection result is a car
        if (detection_result.ClassName==car) {

            bool exist= false ;
            // Check all the existing car in the car_occupants array
            for (auto& car_occupant:car_occupants){
                //                if ((car_occupant.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/1.5))
                if (car_occupant.track_id == detection_result.track_id || (car_occupant.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/1.5)){
                    exist=true;
                    car_occupant.missed_frame = 3;

                    car_occupant.ROI = detection_result.ROI;
                    car_occupant.ConfidentCar = detection_result.Confidence ;

                    bool overLap = (detection_result.ROI & cv::Rect(0, 0,image.cols,image.rows)).area() == detection_result.ROI.area();

                    if((car_occupant.Driver.Confidence + car_occupant.NextDriver.Confidence ==0) &&
                            overLap && detection_result.ROI.area()!= 0 &&
                            car_occupant.FrontCarImageBlurriness > calcBlurriness(image(detection_result.ROI))){
                        image(detection_result.ROI).copyTo(car_occupant.FrontCarImage);
                    }

                    break;
                }
            }
            // the car already has not been detected and the car size is larger than 1/20 of camera 1 ROI and the ROI of the car is in the Detection ROI
            if(!exist && detection_result.ROI.area()>=ROI_Camera_1.area()/20 && (ROI_Camera_1 & detection_result.ROI)==detection_result.ROI){

                CarOccupancy newCar ;
                newCar.ROI =detection_result.ROI ;
                newCar.Color =getRandomColors() ;
                image(detection_result.ROI).copyTo(newCar.FrontCarImage);
                newCar.PickImage = false ;
                newCar.FrontOccupantNumber= settings->getSetting(settings->KEY_DETECT_DRIVER).toBool() ? 0 : 1 ;
                newCar.BackOccupantNumber = 0 ;
                newCar.ConfidentCar = detection_result.Confidence ;
                newCar.track_id = detection_result.track_id ;
                newCar.FrontCarImageBlurriness = calcBlurriness(newCar.FrontCarImage) ;
                car_occupants.push_back(newCar);


            }

        }

    }

    if (car_occupants.size()==0)
        return ;
    for (auto& occupancy:car_occupants){

        for (auto windows_result:windows_detection_results){
            // check window is in the car area
            if (windows_result.ClassName==wind_window && (windows_result.ROI & occupancy.ROI).area()>windows_result.ROI.area()/2) {
                // store last detection confidence
                float last_confidence = 0 ;

                for (auto detection_result:car_detection_results){
                    if (detection_result.ClassName==occupant) {
                        // check occupant is in wind area
                        if ((windows_result.ROI & detection_result.ROI).area() > detection_result.ROI.area()/2) {
                            if((detection_result.ROI.x + detection_result.ROI.width/2) > (windows_result.ROI.x+windows_result.ROI.width/2) ){

                                if(detection_result.Confidence > occupancy.NextDriver.Confidence ) {
                                    last_confidence += occupancy.NextDriver.Confidence +0.001 ;
                                    occupancy.NextDriver.Confidence = detection_result.Confidence;
                                    occupancy.NextDriver.ROI = detection_result.ROI;
                                    image(detection_result.ROI).copyTo(occupancy.NextDriver.Image);
                                }
                                occupancy.NextDriver.ROI = detection_result.ROI ;

                            }
                            else {

                                if(detection_result.Confidence > occupancy.Driver.Confidence  ){
                                    last_confidence += occupancy.Driver.Confidence +0.001 ;
                                    occupancy.Driver.Confidence = detection_result.Confidence;
                                    occupancy.Driver.ROI = detection_result.ROI;
                                    image(detection_result.ROI).copyTo(occupancy.Driver.Image);
                                }
                                //                                qDebug() << "Driver" ;
                                occupancy.Driver.ROI = detection_result.ROI ;
                            }
                        }
                    }
                }
                // replace car image with a new one

                if(last_confidence!=0 && last_confidence < (occupancy.Driver.Confidence + occupancy.NextDriver.Confidence))
                {
//                    qDebug() << last_confidence << "-" << occupancy.Driver.Confidence + occupancy.NextDriver.Confidence ;
                    for (auto detection_result:car_detection_results){
                        //                        if (detection_result.ClassName==car && (occupancy.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/1.5))
                        if (detection_result.ClassName==car &&
                           (occupancy.track_id == detection_result.track_id || (occupancy.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/1.5))
                           && detection_result.ROI.x+detection_result.ROI.width<image.cols
                           && detection_result.ROI.y+detection_result.ROI.height<image.rows
                           ){

                            cv::Mat temp ;
                            image(detection_result.ROI).copyTo(occupancy.FrontCarImage) ;
                            image.copyTo(temp) ;
                            cv::rectangle(temp, occupancy.Driver.ROI, settings->getSettingColor(settings->KEY_COLOR_OCCUPANT),2);
                            cv::rectangle(temp, occupancy.NextDriver.ROI, settings->getSettingColor(settings->KEY_COLOR_OCCUPANT),2);
                            temp(detection_result.ROI).copyTo(occupancy.FrontCarImageProcessed) ;
//                            cv::imshow("sa",occupancy.FrontCarImageProcessed) ;
//                            cv::waitKey(0) ;

                        }
                    }
                }

            }

        }
    }
}

// Process detection results of back camera
void MyDetector::processDetectionBack(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) {
    for (auto& occupancy:car_occupants){
        for (auto windows_result:windows_detection_results){
            // check window is in the car area
            if (windows_result.ClassName==back_rear) {
                // calculate new confidence to replace back image with the new image
                float new_confidence = 0 ;
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
                            new_confidence += detection_result.Confidence + 0.001 ;
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

// Draw detection results
void MyDetector::drawPred(std::vector<DetectionResult> detection_results, cv::Mat& frame,int border , bool show_conf, cv::Point base )
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
            color = settings->front_rear_color ;
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



int MyDetector::runDetector(cv::String source1,cv::String source2){
    // create objects from detectors
    CarDetector cardetector(*settings);
    WindowsDetector windowsdetector(*settings) ;
    OccupantDetector occupantdetector(*settings) ;

    cv::TickMeter tm,tm_total;
    double time_decode_source_1=0,time_decode_source_2=0, time_total=0, time_occupant=0, time_windows=0 ;

    // set environment variable to use GPU for decoding video stream
    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "video_codec;h264_cuvid|rtsp_transport;tcp");

    // create VideoCapture objects for capturing video steams

    cv::VideoCapture cap_1,cap_2 ;
    cv::Ptr<cv::cudacodec::VideoReader> gcap_1,gcap_2 ;

    // create empty Mat for storing frame for cameras
    cv::Mat frame_1,frame_2,frame_concat;
    cv::cuda::GpuMat gframe_1,gframe_2;
    bool useGPUForDecode =  settings->getSetting(settings->KEY_USE_GPU_FOR_DECODE).toBool() ;
    if (useGPUForDecode){
//        cv::cuda::setGlDevice();

        gcap_1 = cv::cudacodec::createVideoReader(source1);

        // return with error code 1
        if (!gcap_1->nextFrame(gframe_1))
            return 1 ;

        // set the first camera width and height variables
        frame_width_camera_1 = gcap_1->format().width ;
        frame_height_camera_1 = gcap_1->format().height ;
    }
    else {
        cap_1=cv::VideoCapture (source1,cv::CAP_FFMPEG);
        // return with error code 1
        if (!cap_1.isOpened())
            return 1 ;

        // set the first camera width and height variables
        frame_width_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_WIDTH);
        frame_height_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_HEIGHT);
    }

    // set the first camera ROI
    cv::Point a;
    cv::Point b;
    ROI_Camera_1 = {cv::Point(settings->ROI_PAD,settings->ROI_PAD),
                    cv::Point(frame_width_camera_1  - (settings->ROI_PAD*2),
                    frame_height_camera_1 - (settings->ROI_PAD*2))} ;

    // initialize the second camera
    bool second_camera = source2!="" ;
    if(source2!="") {
        if (useGPUForDecode){
            gcap_2 = cv::cudacodec::createVideoReader(source2);


            // set the first camera width and height variables
            frame_width_camera_2 = gcap_2->format().width ;
            frame_height_camera_2 = gcap_2->format().height ;


        }
        else {

            cap_2 = cv::VideoCapture (source2,cv::CAP_FFMPEG);

            // check the second camera status
            second_camera = cap_2.isOpened() ;
            if (second_camera) {
                // set the second camera width and height variables
                frame_width_camera_2 = cap_2.get(cv::CAP_PROP_FRAME_WIDTH);
                frame_height_camera_2 = cap_2.get(cv::CAP_PROP_FRAME_HEIGHT);

            }

        }


    }

    // set the second camera ROI
    ROI_Camera_2 = {cv::Point(settings->ROI_PAD,settings->ROI_PAD),
                    cv::Point(frame_width_camera_2  - (settings->ROI_PAD*2),
                    frame_height_camera_2 - (settings->ROI_PAD*2))} ;


    // create an empty vector for storing detection result
    std::vector<CarOccupancy> car_occupancy ;
    run=true ;


    tm_total.reset(); tm_total.start();
    for ( ;; ){
        if(!run) {
            cv::destroyAllWindows() ;
            return 0 ;


        }
        tm_total.reset(); tm_total.start();
        tm.reset(); tm.start();
        // return with error code 2

        if((!useGPUForDecode && !cap_1.read(frame_1)) || (useGPUForDecode && !gcap_1->nextFrame(gframe_1))) {
            qDebug() << "Can not grab images from camera 1." << endl;
            cv::destroyAllWindows() ;
            return 2;
        }
        tm.stop();
        time_decode_source_1 = tm.getTimeMilli();
        //        cap_1.retrieve( frame_1 ) ;


        // stop the program if there is no more images
        if(frame_1.rows==0 && gframe_1.rows==0 ){
            cv::destroyAllWindows() ;
            return 0 ;
        }

        if(useGPUForDecode)
            gframe_1.download(frame_1);

        // Run tiny-yolo car detector on the input frame
        //                std::vector<DetectionResult> car_results_camera_1 =
        //                        cardetector.detect(frame_1);


        // Create empty vectors for front and back windows detection results
        std::vector<DetectionResult> windows_results_front,windows_results_back ;

        // Process the input frame to detect front windows
        tm.reset(); tm.start();
        windows_results_front = windowsdetector.detect(frame_1);
        tm.stop();
        time_windows = tm.getTimeMilli();

        // Create empty vectors for front and back occupants results
        std::vector<DetectionResult> occupant_results_front , occupant_results_back;

        // Process the input frame from the first camera to detect front occupants
        tm.reset(); tm.start();
        occupant_results_front = occupantdetector.detect(frame_1);

        tm.stop();
        time_occupant = tm.getTimeMilli();

        // Process detection result
        processDetectionFront(occupant_results_front,windows_results_front,car_occupancy,frame_1) ;

        // Check wheater there is the second camera or not
        if(second_camera) {

            // if there is no response from the second camera retrun 4 and exit
            tm.reset(); tm.start();
            if((!useGPUForDecode && !cap_2.read(frame_2)) || (useGPUForDecode && !gcap_2->nextFrame(gframe_2))) {
                qDebug() << "Can not grab images from camera 2." << endl;
                cv::destroyAllWindows() ;
                return 4;
            }
            if(useGPUForDecode)
                gframe_2.download(frame_2);
            tm.stop();
            time_decode_source_2 = tm.getTimeMilli();


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
        tm_total.stop();
        time_total = tm_total.getTimeMilli();
        if(settings->getSetting(settings->KEY_SHOW_TIMES).toBool()) {

            cv::putText(frame_concat,"Docede Time Camera 1   : "+ std::to_string(time_decode_source_1),cv::Point(10,30),cv::FONT_HERSHEY_COMPLEX,0.5,cv::Scalar(0,255,0),1) ;
            cv::putText(frame_concat,"Docede Time Camera 2   : "+ std::to_string(time_decode_source_2),cv::Point(10,50),cv::FONT_HERSHEY_COMPLEX,0.5,cv::Scalar(0,255,0),1) ;
            cv::putText(frame_concat,"Occupant Detector Time: "+ std::to_string(time_occupant),cv::Point(10,70),cv::FONT_HERSHEY_COMPLEX,0.5,cv::Scalar(0,255,0),1) ;
            cv::putText(frame_concat,"Windows Detector Time : "+ std::to_string(time_windows),cv::Point(10,90),cv::FONT_HERSHEY_COMPLEX,0.5,cv::Scalar(0,255,0),1) ;
            cv::putText(frame_concat,"FPS : "+ std::to_string(1000/time_total),cv::Point(10,110),cv::FONT_HERSHEY_COMPLEX,0.5,cv::Scalar(0,255,0),1) ;
        }
        cv::rectangle(frame_concat,ROI_Camera_1,cv::Scalar(0,255,0),3) ;

        // Show output frame
        imshow("tracker",frame_concat);
        qApp->processEvents();

        //quit on ESC button
        if(cv::waitKey(1)==27){
            cap_1.release();
            cap_2.release();
            gcap_1.release();
            gcap_2.release();
            cv::destroyAllWindows();
            break;
        }
    }
    return 0;
}


