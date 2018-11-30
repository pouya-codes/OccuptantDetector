#include "detector.h"
#include "QSysInfo"

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

//void Detector::insertResult(CarOccupant occupant) {
//    if (occupant.OccupantNumber>0 ) {
//        QSqlQuery query = QSqlQuery(*db );
//        query.prepare( "INSERT INTO result (occupant, date , imagedata) VALUES (:occupant, :date , :imageData)" );

//        QPixmap inPixmap = ASM::cvMatToQPixmap(occupant.CarImage) ;
//        QByteArray inByteArray;
//        QBuffer inBuffer( &inByteArray );
//        inBuffer.open( QIODevice::WriteOnly );
//        inPixmap.save( &inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format

//        query.bindValue( ":occupant", occupant.OccupantNumber );
//        query.bindValue(":date", QString::fromStdString(currentDateTime()));
//        query.bindValue( ":imageData", inByteArray );

//        if( !query.exec() )
//            qDebug() << "Error inserting image into table:\n" << query.lastError();
//    }

//}

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
                newCar.OccupantNumber= detectDriver? 0 : 1 ;
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
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "video_codec;h264_cuvid|rtsp_transport;tcp");
#endif


    cv::VideoCapture cap_1(source1,cv::CAP_FFMPEG);
    cv::VideoCapture cap_2(source2,cv::CAP_FFMPEG);
    int frame_rate = cap_1.get(cv::CAP_PROP_FPS) > cap_2.get(cv::CAP_PROP_FPS) ?
                cap_2.get(cv::CAP_PROP_FPS) : cap_1.get(cv::CAP_PROP_FPS) ;
    if (!cap_1.isOpened())
        return 1 ;
    if (!cap_2.isOpened())
        return 2 ;
    frame_width_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height_camera_1 = cap_1.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::Point a;
    cv::Point b;
    ROI_Camera_1 = {cv::Point(ROI_PAD,ROI_PAD),
                    cv::Point(frame_width_camera_1  - (ROI_PAD*2),
                    frame_height_camera_1 - (ROI_PAD*2))} ;

    if (!cap_2.isOpened())
        return 2 ;
    frame_width_camera_2 = cap_1.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height_camera_2 = cap_1.get(cv::CAP_PROP_FRAME_HEIGHT);

    ROI_Camera_2 = {cv::Point(ROI_PAD,ROI_PAD),
                    cv::Point(frame_width_camera_2  - (ROI_PAD*2),
                    frame_height_camera_2 - (ROI_PAD*2))} ;


    //    std::cout << QSysInfo::productType().toStdString() << std::endl ;

    //    int ex = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC)); // Get Codec Type- Int form
    std::string::size_type pAt = source1.find_last_of('.');
    const std::string NAME = source1.substr(0, pAt) + "_result.avi";   // Form the new name with container
    cv::VideoWriter outputVideo;
    outputVideo.open(NAME, cv::VideoWriter::fourcc('W','M','V','2'),
                     frame_rate, cv::Size(frame_width_camera_1,frame_height_camera_1), true);

    int frame_counter = 1;
    std::vector<CarOccupant> car_occupant ;
    run=true ;
    cv::Mat frame_1,frame_2,frame_concat;
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
        if(!cap_2.grab()) {
            qDebug() << "Can not grab images from camera 2." << endl;
            return 4;
        }

        //        cap_1.retrieve( frame_1, cv::CAP_OPENNI_IR_IMAGE ) ;
        //        cap_2.retrieve( frame_1, cv::CAP_OPENNI_IR_IMAGE ) ;

        cap_1.retrieve( frame_1 ) ;
        cap_2.retrieve( frame_2 ) ;


        cv::hconcat(frame_1,frame_2,frame_concat) ;
        frame_counter++ ;

        // stop the program if no more images
        if(frame_1.rows==0 || frame_2.rows==0)
            return 5;
        std::vector<DetectionResult> car_results_camera_1 =
                cardetector.detect(frame_1);
        if (frame_counter%1==0
                && car_results_camera_1.size()>0 && false
                ) {

            qDebug() << "Front Camera Detected a car" ;
            std::vector<DetectionResult> car_results_camera_2 = cardetector.detect(frame_2) ;
            while(car_results_camera_2.size()==0) {
                cap_1.grab();
                cap_2.grab();
                cap_2.retrieve( frame_2 ) ;
                car_results_camera_2 = cardetector.detect(frame_2);
            }

            qDebug() << "Rear Camera Detected a car" ;

            std::vector<DetectionResult> windows_results_front = windowsdetector.detect(frame_1);
            std::vector<DetectionResult> windows_results_rear  = windowsdetector.detect(frame_2);
            std::vector<DetectionResult> occupant_results = occupantdetector.detect(frame_2);
            drawPred(windows_results_front,frame_concat,2,true);
            drawPred(occupant_results,frame_concat,2,true);

            drawPred(windows_results_rear,frame_concat,2,true,cv::Point(frame_2.cols,0));





            //            processDetection(occupant_results,car_occupant,frame) ;
            //            drawResult(frame,car_occupant) ;

        }
        else {

            std::vector<DetectionResult> windows_results_front = windowsdetector.detect(frame_1);
//            std::vector<DetectionResult> windows_results_rear  = windowsdetector.detect(frame_2);
            std::vector<DetectionResult> occupant_results = occupantdetector.detect(frame_2);

//            drawPred(car_results_camera_1,frame_concat,2,true);
            drawPred(windows_results_front,frame_concat,2,true);
            drawPred(occupant_results,frame_concat,2,true);
        }

        outputVideo << frame_concat;
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


