#include "detector.h"
#include "QBuffer"

cv::RNG rng(1);


Detector::Detector(cv::String source,QSqlDatabase* db)
{

    if(source!="")
    {
        this->db = db ;
        this->source = source;
        run = true ;
        runDetector() ;
        if(QSqlDatabase::isDriverAvailable(DRIVER)) {
            QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
            db.setDatabaseName("results");
            if(!db.open())
                qWarning() << "ERROR: " << db.lastError();
        }
    }

}
void Detector::stopDetector() {
    //    this->run = false ;
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

void Detector::insertResult(CarOccupant occupant) {
    if (occupant.OccupantNumber>0 ) {
        QSqlQuery query = QSqlQuery(*db );
        query.prepare( "INSERT INTO result (occupant, date , imagedata) VALUES (:occupant, :date , :imageData)" );

        QPixmap inPixmap = ASM::cvMatToQPixmap(occupant.CarImage) ;
        QByteArray inByteArray;
        QBuffer inBuffer( &inByteArray );
        inBuffer.open( QIODevice::WriteOnly );
        inPixmap.save( &inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format

        query.bindValue( ":occupant", occupant.OccupantNumber );
        query.bindValue(":date", QString::fromStdString(currentDateTime()));
        query.bindValue( ":imageData", inByteArray );

        if( !query.exec() )
            qDebug() << "Error inserting image into table:\n" << query.lastError();
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

        if (car_occupants[idx].ROI.area()<30000 || (ROI & car_occupants[idx].ROI )!=car_occupants[idx].ROI) //|| !haveCar)// || (car_occupants[idx].ROI.x+car_occupants[idx].ROI.width + 20 > image.cols)
            //|| (car_occupants[idx].ROI.y+car_occupants[idx].ROI.height + 20 > image.rows))
        {

            insertResult(car_occupants[idx]);
            car_occupants.erase(car_occupants.begin()+ idx);
        }
    }


    for (auto detection_result:detection_results){
        if (detection_result.ClassIdx==2) {
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
            if(!exist && detection_result.ROI.area()>=30000 && (ROI & detection_result.ROI)==detection_result.ROI) {
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
        if (detection_result.ClassIdx==0)
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



        //            std::cout << ;

    }

}


void Detector::drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame,cv::Scalar color )
{
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), color);

    std::string label = cv::format("%.2f", conf);
    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId] + ": " + label;
    }

    int baseLine;
    cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

    top = cv::max(top, labelSize.height);
    cv::rectangle(frame, cv::Point(left, top - labelSize.height),cv::Point(left + labelSize.width, top + baseLine), cv::Scalar::all(255) , cv::FILLED);
    cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar());
}

std::vector<cv::String> Detector::getOutputsNames(const cv::dnn::Net& net)
{
    static std::vector<cv::String> names;
    if (names.empty())
    {
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<cv::String> layersNames = net.getLayerNames();
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

bool Detector::postprocesstiny(const std::vector<cv::Mat>& outs)
{

    for (size_t i = 0; i < outs.size(); ++i)
    {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            if (classIdPoint.x==2 && confidence > 0.5)
                return true ;
        }

    }
    return false ;

}



void Detector::postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs,float confThreshold,std::vector<DetectionResult>& detection_result)
{
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Network produces output blob with a shape NxC where N is a number of
        // detected objects and C is a number of classes + 4 where the first 4
        // numbers are [center_x, center_y, width, height]
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            if (confidence > confThreshold && (classIdPoint.x==0 || (classIdPoint.x==2 && confidence > 0.3)))
            {
                //                std::cout << confidence << ":" << classIdPoint.x << std::endl ;
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
                //                if(classIdPoint.x==2 && (cv::Rect(left, top, width, height).area() < 30000 || (left + width + 20 > frame.cols)
                //                    || (top +  height > frame.rows) ))
                //                    continue ;
                cv::Rect ROI_Detected = cv::Rect(left, top, width, height);
//                if ( classIdPoint.x==2 && (ROI & ROI_Detected).area()!=ROI_Detected.area() )
//                    continue ;
                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(ROI_Detected);




                //                ROIs.push_back(Rect(left > 0 ? left : 0 , top > 0 ? top : 0  , width, height));
            }
        }
    }
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, 0.4f, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        //        std::cout << Rect(box.x > 0 ? box.x : 0 , box.y > 0 ? box.y : 0  , box.width, box.height) << std::endl ;
        cv::Rect ROI = cv::Rect(box.x > 0 ? box.x : 0 ,
                                box.y > 0 ? box.y : 0 ,
                                box.width + (box.x > 0 ? box.x : 0) > frame.cols ? frame.cols-(box.x > 0 ? box.x : 0) : box.width,
                                box.height+ (box.y > 0 ? box.y : 0) > frame.rows ? frame.rows-(box.y > 0 ? box.y : 0) : box.height
                                                                                   ) ;

        detection_result.push_back({ROI,classIds[idx],confidences[idx]});
//        drawPred(classIds[idx], confidences[idx], box.x, box.y,
//                 box.x + box.width, box.y + box.height, frame , cv::Scalar(0,255,0));
    }


}

void Detector::postprocessCar(cv::Mat& frame,cv::Mat& input, const std::vector<cv::Mat>& outs,float confThreshold,int classidx,cv::Rect carRect)
{
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Network produces output blob with a shape NxC where N is a number of
        // detected objects and C is a number of classes + 4 where the first 4
        // numbers are [center_x, center_y, width, height]
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold && classIdPoint.x==classidx)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, 0.4f, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], carRect.x + box.x, carRect.y + box.y,
                 carRect.x + box.x + box.width, carRect.y + box.y + box.height, input ,cv::Scalar(0,0,255));
    }


}

bool Detector::isThereAnyCar(cv::Mat image) {

    cv::Mat inputBlob = cv::dnn::blobFromImage(image, 1 / 255.F , cv::Size(416, 416), cv::Scalar(), true, false); //Convert Mat to batch of images
    net_tiny.setInput(inputBlob);                   //set the network input
    std::vector<cv::Mat> outs;

    net_tiny.forward(outs, getOutputsNames(net_tiny));  //compute output
    return postprocesstiny(outs);

}

//Mat equalizeIntensity(const Mat& inputImage)
//{
//    if(inputImage.channels() >= 3)
//    {
//        Mat ycrcb;

//        cvtColor(inputImage,ycrcb,CV_BGR2YCrCb);

//        vector<Mat> channels;
//        split(ycrcb,channels);

//        equalizeHist(channels[0], channels[0]);

//        Mat result;
//        merge(channels,ycrcb);

//        cvtColor(ycrcb,result,CV_YCrCb2BGR);

//        return result;
//    }
//    return Mat();
//}

int Detector::runDetector(){

    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelBinary);
    net_tiny = cv::dnn::readNetFromDarknet(modelConfigurationTiny, modelBinaryTiny);
    std::vector<CarOccupant> car_occupant ;




    cv::Mat frame;


    cv::VideoCapture cap(source);



    // Default resolution of the frame is obtained.The default resolution is system dependent.
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int frame_rate = cap.get(cv::CAP_PROP_FPS);

    cv::Point a;
    cv::Point b;
    a.x = 20;
    a.y = 20;
    b.x = frame_width - 40 ;
    b.y = frame_height - 40;
    ROI = cv::Rect(a,b);


    int ex = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC)); // Get Codec Type- Int form
    std::string::size_type pAt = source.find_last_of('.');
    const std::string NAME = source.substr(0, pAt) + "_result.avi";   // Form the new name with container
    cv::VideoWriter outputVideo;
    outputVideo.open(NAME, ex, frame_rate, cv::Size(frame_width,frame_height), true);

    time_t start, end;

    int frame_counter = 1;
    for ( ;; ){
        if(!run) {
            break ;
            cv::destroyAllWindows() ;

        }
        time(&start);
        // get frame from the video
        cap >> frame;


        //        frame = equalizeIntensity(frame) ;
        //        frame = rotateImage(frame, 12);


        frame_counter++ ;
        // stop the program if no more images
        if(frame.rows==0 || frame.cols==0)
            break;

        if (frame_counter%1==0
                //&& isThereAnyCar(frame)
                ) {
            cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1 / 255.F , cv::Size(416, 416), cv::Scalar(), true, false); //Convert Mat to batch of images
            net.setInput(inputBlob);                   //set the network input
            std::vector<cv::Mat> outs;

            std::vector<DetectionResult> detection_results;
            net.forward(outs, getOutputsNames(net));  //compute output
            postprocess(frame, outs,confidentThredshold,detection_results);
            processDetection(detection_results,car_occupant,frame) ;
            drawResult(frame,car_occupant) ;

        }
        time(&end);
        double seconds = difftime (end, start);

        //        putText(frame, std::to_string( 1/seconds), Point(25, 25), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,255,0));
        outputVideo << frame;
        //        QImage qt_img = ASM::cvMatToQImage( frame );
        //        image.setPixmap(QPixmap::fromImage(qt_img));
        //        image.setText("salalalam");
        cv::rectangle(frame,ROI,cv::Scalar(0,255,0),5) ;
        imshow("tracker",frame);
        qApp->processEvents();
        //quit on ESC button
        if(cv::waitKey(1)==27){
            cv::destroyAllWindows();
            break;
        }
    }
    return 0;
}


