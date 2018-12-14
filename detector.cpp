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

cv::Scalar Detector::getRandomColors()
{
    //    RNG rng(0);
    return cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
}


void Detector::processDetectionFront(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) {

    for (uint idx = 0 ; idx < car_occupants.size(); idx++ ) {

        //        std::cout<< car_occupants[idx].ROI.area() << std::endl ;
        //        bool haveCar = false ;
        //        for (auto detection_result:detection_results){
        //            if ((car_occupants[idx].ROI & detection_result.ROI).area()>(car_occupants[idx].ROI.area()/2)){
        //                haveCar = true ;
        //                break;
        //            }

        //        }

        if (car_occupants[idx].ROI.area()<30000 || (ROI_Camera_1 & car_occupants[idx].ROI )!=car_occupants[idx].ROI) //|| !haveCar)// || (car_occupants[idx].ROI.x+car_occupants[idx].ROI.width + 20 > image.cols)
            //|| (car_occupants[idx].ROI.y+car_occupants[idx].ROI.height + 20 > image.rows))
        {
            qDebug() << car_occupants[idx].NextDriver.Confidence << car_occupants[idx].Driver.Confidence ;
            //            insertResult(car_occupants[idx]);
            if (car_occupants[idx].NextDriver.Confidence!=0 && settings->getSetting(settings->KEY_DETECT_DRIVER).toBool())
                car_occupants[idx].FrontOccupantNumber+= 1 ;

            if (car_occupants[idx].Driver.Confidence!=0 )
                car_occupants[idx].FrontOccupantNumber+= 1 ;
            dbmanager->insertResult(car_occupants[idx]) ;
            car_occupants.erase(car_occupants.begin()+ idx);

        }
    }



    bool car_exist = false ;
    for (auto detection_result:car_detection_results){
        if (detection_result.ClassName==car) {

            bool exist= false ;
            for (auto& car_occupant:car_occupants){
                if ((car_occupant.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/1.5)) {
                    exist=true;

                    //                    if (
                    //                            (car_occupant.ConfidentCar < detection_result.Confidence)
                    //                            //                            ||
                    //                            //                         ( (cv::countNonZero(car_occupant.next_driver_detected)==numberOfDetection ||
                    //                            //                            cv::countNonZero(car_occupant.driver_detected)==numberOfDetection))
                    //                            ){
                    //                        image(car_occupant.ROI).copyTo(car_occupant.CarImage);
                    //                        car_occupant.ConfidentCar = detection_result.Confidence ;
                    //                    }


                    car_occupant.ROI = detection_result.ROI;
                    car_occupant.ConfidentCar = detection_result.Confidence ;
                    car_exist = true ;
                    break;
                }
            }
            if(!exist && detection_result.ROI.area()>=ROI_Camera_1.area()/20 && (ROI_Camera_1 & detection_result.ROI)==detection_result.ROI
                    ){
                //                qDebug() << "Sas";
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

                            //                            car_result = & detection_result ;
                            //
                            //                            cv::circle(image,cv::Point((detection_result.ROI.x + detection_result.ROI.width/2),detection_result.ROI.y),5,cv::Scalar(0,255,0),-1);
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

void Detector::processDetectionBack(std::vector<DetectionResult> car_detection_results,std::vector<DetectionResult> windows_detection_results,std::vector<CarOccupancy>& car_occupants,cv::Mat image) {


    for (auto& occupancy:car_occupants){

        for (auto windows_result:windows_detection_results){

            // check window is in the car area
            if (windows_result.ClassName==back_rear) {
                double new_confidence = 0 ;
                std::vector <OccupantResult> back_occupants ;
                for (DetectionResult detection_result:car_detection_results){
                    if (detection_result.ClassName==occupant) {

                        // check occupant is in the window area
                        if ((windows_result.ROI & detection_result.ROI).area() > detection_result.ROI.area()/2) {
                            //cv::circle(image,cv::Point((detection_result.ROI.x + detection_result.ROI.width/2),detection_result.ROI.y),5,cv::Scalar(0,255,0),-1);

                            OccupantResult temp ;
                            temp.Image = image(detection_result.ROI);
                            temp.ROI = detection_result.ROI ;
                            temp.Confidence = detection_result.Confidence  ;
                            back_occupants.push_back(temp);
                            new_confidence += (double) detection_result.Confidence ;
                        }

                    }

                }
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


void Detector::processDetection(std::vector<DetectionResult> detection_results,std::vector<CarOccupant>& car_occupants,cv::Mat image){

    for (uint idx = 0 ; idx < car_occupants.size(); idx++ ) {
        //        std::cout<< car_occupants[idx].ROI.area() << std::endl ;
        //        bool haveCar = false ;
        //        for (auto detection_result:detection_results){
        //            if ((car_occupants[idx].ROI & detection_result.ROI).area()>(car_occupants[idx].ROI.area()/2)){
        //                haveCar = true ;
        //                break;
        //            }

        //        }

        if (car_occupants[idx].ROI.area()<30000 || (ROI_Camera_1 & car_occupants[idx].ROI )!=car_occupants[idx].ROI) //|| !haveCar)// || (car_occupants[idx].ROI.x+car_occupants[idx].ROI.width + 20 > image.cols)
            //|| (car_occupants[idx].ROI.y+car_occupants[idx].ROI.height + 20 > image.rows))
        {

            //            insertResult(car_occupants[idx]);
            car_occupants.erase(car_occupants.begin()+ idx);
        }
    }


    for (auto detection_result:detection_results){
        if (detection_result.ClassName==car) {
            bool exist= false ;
            for (auto& car_occupant:car_occupants){
                if ((car_occupant.ROI & detection_result.ROI).area()>(detection_result.ROI.area()/3)) {
                    exist=true;

                    //                    if (
                    //                            (car_occupant.ConfidentCar < detection_result.Confidence)
                    //                            //                            ||
                    //                            //                         ( (cv::countNonZero(car_occupant.next_driver_detected)==numberOfDetection ||
                    //                            //                            cv::countNonZero(car_occupant.driver_detected)==numberOfDetection))
                    //                            ){
                    //                        image(car_occupant.ROI).copyTo(car_occupant.CarImage);
                    //                        car_occupant.ConfidentCar = detection_result.Confidence ;
                    //                    }


                    car_occupant.ROI = detection_result.ROI;
                    car_occupant.ConfidentCar = detection_result.Confidence ;
                    break;
                }
            }
            if(!exist && detection_result.ROI.area()>=30000 && (ROI_Camera_1 & detection_result.ROI)==detection_result.ROI) {
                CarOccupant newCar ;
                newCar.ROI =detection_result.ROI ;
                newCar.Color =getRandomColors() ;
                image(detection_result.ROI).copyTo(newCar.CarImage);
                newCar.OccupantNumber=  settings->getSetting(settings->KEY_DETECT_DRIVER).toBool() ? 0 : 1 ;
                newCar.ConfidentCar = detection_result.Confidence ;
                car_occupants.push_back(newCar);
            }

        }

    }

    for (auto detection_result:detection_results){
        if (detection_result.ClassName==occupant)
        {
            for (auto& car_occupant:car_occupants){
                if ( (car_occupant.ROI & detection_result.ROI)==detection_result.ROI
                     && detection_result.Confidence>confidentThredshold) {
                    //                    cv::Rect a = {int(car_occupant.ROI.x+ car_occupant.ROI.width/2),int(car_occupant.ROI.y),car_occupant.ROI.width/2,int(car_occupant.ROI.height)} ;
                    //                    cv::rectangle(image,a,cv::Scalar(100,100,100),-1) ;

                    if ((car_occupant.ROI.x+car_occupant.ROI.width/2)<detection_result.ROI.x && cv::countNonZero(car_occupant.next_driver_detected)!=numberOfDetection)
                    {
                        for (int i = 0 ; i < numberOfDetection ; i ++)
                            if (car_occupant.next_driver_detected.at<uint8_t>(i,0)==0)
                            {
                                car_occupant.next_driver_detected.at<uint8_t>(i,0)=1;
                                break ;
                            }
                        if (cv::countNonZero(car_occupant.next_driver_detected)==numberOfDetection)
                            car_occupant.OccupantNumber++ ;
                    }


                    else
                    {
                        if(cv::countNonZero(car_occupant.driver_detected)!=numberOfDetection)
                        {
                            for (int i = 0 ; i < numberOfDetection ; i ++)
                            {

                                if (car_occupant.driver_detected.at<uint8_t>(i,0)==0)
                                {
                                    car_occupant.driver_detected.at<uint8_t>(i,0)=1;

                                    break ;
                                }
                            }
                            if (cv::countNonZero(car_occupant.driver_detected)==numberOfDetection)
                                car_occupant.OccupantNumber++ ;
                        }


                    }
                    break;

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

void Detector::drawResult(cv::Mat& frame,std::vector<CarOccupant> &car_occupants )
{


    int idx = 0 ;
    for (auto car_occupant:car_occupants){
        if (!car_occupant.CarImage.empty()) {
            cv::Mat resized_car ;
            cv::resize(car_occupant.CarImage,resized_car,cv::Size(50,50)) ;
            cv::copyMakeBorder(resized_car,resized_car,3,3,3,3,cv::BORDER_CONSTANT,car_occupant.Color);
            cv::Mat destinationROI = frame( cv::Rect(frame.cols-60,70*idx,resized_car.cols,resized_car.rows) );
            resized_car.copyTo( destinationROI );
            cv::putText(frame,std::to_string(car_occupant.OccupantNumber),cv::Point(frame.cols-30,(70*idx)+25),cv::FONT_HERSHEY_SCRIPT_SIMPLEX,1,cv::Scalar(0,255,0),2) ;
            idx++ ;
        }

        rectangle(frame, car_occupant.ROI, car_occupant.Color,4);
        if(cv::countNonZero(car_occupant.driver_detected)==numberOfDetection)
            cv::circle(frame,cv::Point(car_occupant.ROI.x+10,car_occupant.ROI.y-10),5,cv::Scalar(0,0,255),-1);
        if(cv::countNonZero(car_occupant.next_driver_detected)==numberOfDetection)
            cv::circle(frame,cv::Point(car_occupant.ROI.x+30,car_occupant.ROI.y-10),5,cv::Scalar(0,0,255),-1);

    }

}

void Detector::drawPred(std::vector<DetectionResult> detection_results, cv::Mat& frame,int border , bool show_conf, cv::Point base )
{
    for(DetectionResult detection_result:detection_results) {
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
    //    std::cout << source << std::endl ;




    CarDetector cardetector(*settings);
    WindowsDetector windowsdetector(*settings) ;
    OccupantDetector occupantdetector(*settings) ;


#if defined(WIN32)
    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "video_codec;h264_cuvid|rtsp_transport;tcp");
#else
    //    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "video_codec;h264_cuvid|rtsp_transport;tcp");
#endif

    cv::VideoCapture cap_1(source1,cv::CAP_FFMPEG);
    cv::VideoCapture cap_2(source2,cv::CAP_FFMPEG);

    bool second_camera = cap_2.isOpened() ;
    if (second_camera) {
        frame_width_camera_2 = cap_2.get(cv::CAP_PROP_FRAME_WIDTH);
        frame_height_camera_2 = cap_2.get(cv::CAP_PROP_FRAME_HEIGHT);

        ROI_Camera_2 = {cv::Point(ROI_PAD,ROI_PAD),
                        cv::Point(frame_width_camera_2  - (ROI_PAD*2),
                        frame_height_camera_2 - (ROI_PAD*2))} ;
    }
    //    int frame_rate = cap_1.get(cv::CAP_PROP_FPS) > cap_2.get(cv::CAP_PROP_FPS) ?
    //                cap_2.get(cv::CAP_PROP_FPS) : cap_1.get(cv::CAP_PROP_FPS) ;
    if (!cap_1.isOpened())
        return 1 ;
//    if (!cap_2.isOpened())
//            return 2 ;
    frame_width_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::Point a;
    cv::Point b;
    ROI_Camera_1 = {cv::Point(ROI_PAD,ROI_PAD),
                    cv::Point(frame_width_camera_1  - (ROI_PAD*2),
                    frame_height_camera_1 - (ROI_PAD*2))} ;

    //    if (!cap_2.isOpened())
    //        return 2 ;




    //    std::cout << QSysInfo::productType().toStdString() << std::endl ;

    //    int ex = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC)); // Get Codec Type- Int form
    //    std::string::size_type pAt = source1.find_last_of('.');
    //    const std::string NAME = source1.substr(0, pAt) + "_result.avi";   // Form the new name with container
    //    cv::VideoWriter outputVideo;
    //    outputVideo.open(NAME, cv::VideoWriter::fourcc('W','M','V','2'),
    //                     frame_rate, cv::Size(frame_width_camera_1,frame_height_camera_1), true);

    int frame_counter = 1;
    std::vector<CarOccupant> car_occupant ;
    std::vector<CarOccupancy> car_occupancy ;
    run=true ;
    cv::Mat frame_1,frame_2
            ,frame_concat;
    for ( ;; ){
        if(!run) {
            break ;
            cv::destroyAllWindows() ;

        }

        //        cap_1 >> frame_1;
        //        cap_2 >> frame_2;
        if(!cap_1.grab()) {
            qDebug() << "Can not grab images from camera 1." << endl;
            return 3;
        }
        //        if(!cap_2.grab()) {
        //            qDebug() << "Can not grab images from camera 2." << endl;
        //            return 4;
        //        }

        //        cap_1.retrieve( frame_1, cv::CAP_OPENNI_IR_IMAGE ) ;
        //        cap_2.retrieve( frame_1, cv::CAP_OPENNI_IR_IMAGE ) ;

        cap_1.retrieve( frame_1 ) ;
        //        cap_2.retrieve( frame_2 ) ;


        //        cv::hconcat(frame_1,frame_2,frame_concat) ;

        frame_counter++ ;

        // stop the program if no more images
        if(frame_1.rows==0 )//|| frame_2.rows==0)
            return 5;
        std::vector<DetectionResult> car_results_camera_1 =
                cardetector.detect(frame_1);


            std::vector<DetectionResult> windows_results_front,windows_results_back ;
            windows_results_front = windowsdetector.detect(frame_1);
            std::vector<DetectionResult> occupant_results_front , occupant_results_back;
            occupant_results_front = occupantdetector.detect(frame_1);
            processDetectionFront(occupant_results_front,windows_results_front,car_occupancy,frame_1) ;
            if(second_camera) {
                if(!cap_2.grab()) {
                    qDebug() << "Can not grab images from camera 2." << endl;
                    return 4;
                }
                cap_2.retrieve( frame_2 ) ;
                if(car_occupancy.size()==1){
                    windows_results_back = windowsdetector.detect(frame_2);
                    int is_back_rear = 0 ;
                    for(DetectionResult window_result:windows_results_back){
                        if (window_result.ClassName==back_rear){
                            is_back_rear += 1 ;
                        }
                    }
                    if (is_back_rear==1) {
                        occupant_results_back = occupantdetector.detect(frame_2);
                        processDetectionBack(occupant_results_back,windows_results_back,car_occupancy,frame_2) ;
                    }

                }

                drawPred(windows_results_front,frame_1,2,true);
                drawPred(occupant_results_front,frame_1,2,true);
                drawPred(windows_results_back,frame_2,2,true);
                drawPred(occupant_results_back,frame_2,2,true);

                cv::hconcat(frame_1,frame_2,frame_concat) ;

            }
            else {
                drawPred(windows_results_front,frame_1,2,true);
                drawPred(occupant_results_front,frame_1,2,true);
                frame_1.copyTo(frame_concat) ;
            }




        //        outputVideo << frame_concat;
        //        QImage qt_img = ASM::cvMatToQImage( frame );
        //        image.setPixmap(QPixmap::fromImage(qt_img));
        //        image.setText("salalalam");
        //        cv::rectangle(frame_concat,ROI,cv::Scalar(0,255,0),5) ;
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


