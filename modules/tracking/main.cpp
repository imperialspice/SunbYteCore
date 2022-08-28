#include "main.hpp"

using namespace cv;


void tracking::debug_file(std::string path) {
    //free(this->capture);
    this->capture = new cv::VideoCapture(path);


    if (!this->capture->isOpened()) {
        std::cerr << "ERROR: Could not open file" << std::endl;
        exit(-1);
    }


}

void tracking::setup() {

    // settings setup
    settings.registerSetting("tracking/limit", 5, Int);
    settings.registerSetting("tracking/exposure", 10, Int);
    //limit = (settings.getPointer("tracking/limit")).;



    if(this->cameraID == -1){
        this->capture = new cv::VideoCapture(this->cameraPath);
        this->capture->set(cv::CAP_PROP_FRAME_HEIGHT, 500);
        this->capture->set(cv::CAP_PROP_FRAME_WIDTH, 500);

    }
    else{
        this->capture = new cv::VideoCapture(this->cameraID, cv::CAP_V4L2);
        this->capture->set(cv::CAP_PROP_AUTO_EXPOSURE, 3);
        this->capture->set(cv::CAP_PROP_AUTO_EXPOSURE, 1);
        this->capture->set(cv::CAP_PROP_EXPOSURE, settings.getInt("tracking/exposure"));
    }

    capture->set(cv::CAP_PROP_FPS, 30);
    if(!capture->isOpened()){
        std::cout << "Unable to open the camera object" << std::endl;
        exit(-1);
    }
    if(this->debug){
        std::cout << "Creating Debug Window" << std::endl;
        cv::namedWindow("Grey3", WINDOW_AUTOSIZE);
        cv::namedWindow("TrackingDebug", WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | WINDOW_AUTOSIZE);
        cv::pollKey();
    }


}



void tracking::task_loop() {


}
/* This is run every iteration
 *  Performs a camera read and then processes it to generate the offset values
 *
 */
void tracking::process_loop(){
    int limit = settings.getInt("tracking/limit");
    this->capture->set(cv::CAP_PROP_EXPOSURE, settings.getInt("tracking/exposure"));


    capture->read(frame); // read from the camera object
    frameCount++; // iterate frame count
    double avg_x, avg_y;
    // frame center
    frame_center = {frame.cols/2, frame.rows/2};

    // clear the ellipses and circles list

    ellipses.clear();
    circles.clear();
    contours.clear();
    rtn.clear();
    // convert to grey to make it easier
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
    // blur the image to ensure frame is clear
    cv::GaussianBlur(grey, grey2, cv::Size(51,51), 0, 0);
    // covert to binary
    cv::threshold(grey2, grey3, 250, 255, cv::THRESH_BINARY);
    messageList.set("tracking/grey", grey3);
    //collect moments
    moments = cv::moments(grey3, true);
    // Points
    cv::Point2d center_moment = {moments.m10 / moments.m00, moments.m01 / moments.m00};
    // draw a circle
    cv::circle(frame, center_moment, limit, cv::Scalar(255, 0, 0), 3);
    // find countors
    cv::findContours(grey3, contours, cv::RETR_LIST, CHAIN_APPROX_NONE);
    // iterate though the contours
    for(auto& contour : contours){
        // get the center of the contour
        cv::Point2f c; float r;
        /* These contours are interesting as the sun can be picked out from large regions of sky.
         *
        */
        cv::minEnclosingCircle(contour, c, r);
        // calculate the distance from the center of the contour to the center of the moment.
        double distance = sqrt(pow((center_moment.x - c.x),2) + pow((center_moment.y - c.y),2));
        if(this->debug){
            std::cout << distance << std::endl;
            std::cout << "Moment X: " << center_moment.x << " Moment Y: " << center_moment.y << std::endl;
            std::cout << "Contour X: " << c.x << " Contour Y: " << c.y << std::endl;
            std::cout << "Radius : " << r << std::endl;
        }
        // determine if it should be included as a target for the vector

          if(distance < limit && (distance+limit) < r) { // if the distance to the center_moment is small enough then include it in the vector
              noSunCount = 0;
              if (this->debug) { // only draw in debug mode
                  circle(frame, c, r, Scalar(0, 255, 0), 1);
                  circle(frame, c, 1, Scalar(0, 255, 0), 1);
              }
              //std::cout << "Center is " << c.x - frame_center.x << " : " << c.y - frame_center.y << std::endl;
              avg_x = (center_moment.x + c.x)/2; avg_y = (center_moment.y + c.y)/2;
              rtn = "Possible sun found: \nX: " + std::to_string(avg_x)  + "  Y:  " + std::to_string(avg_y);
              break;
          }
          else{
            noSunCount++;
            if(noSunCount > noneBeforeSearch){
                std::cout << "SunSearch Method Enabled" << std::endl;
            }
          }
    }
    if(contours.empty()){
        noSunCount++;
        if(noSunCount > noneBeforeSearch){
            std::cout << "SunSearch Method Enabled" << std::endl;
        }
    }
    //cv::resize(frame, smallframe, Size(), 1, 1);
    if(this->debug){
        cv::imshow("Grey3", grey3);
        cv::imshow("TrackingDebug", frame);
        cv::pollKey();
    }
    if(!rtn.empty()) std::cout << "Response: " << rtn << std::endl;


}

void tracking::sunScan() {
    // assume the current position is zero, then
    // get information from the Rpi sensors about rotation and other information



}

int main() {

/*   // open the first webcam plugged in the computer
//    cv::VideoCapture camera(0);
//    //cv::VideoCapture camera("/home/midd1eton/Projects/sunbytecore/modules/tracking/res/sun_no_clouds.mp4");
//
//
//
//
//
//    // create a window to display the images from the webcam
//    cv::namedWindow("Webcam", WINDOW_NORMAL| WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
//
//    // this will contain the image from the webcam
//
//
//
//    // capture the next frame from the webcam
//
//
//    // display the frame until you press a key
//    while (1) {
//        camera.read(frame);
//        frameCount++;
//
//        if(update_int != update_frequency){
//            update_int++;
//            continue;
//
//        }
//        update_int = 0;
//        frame_center = {frame.cols/2, frame.rows/2};
//
//
//        // show the image on the window
//        ellipses.clear();
//        circles.clear();
//        // convert to grey
//        cv::cvtColor(frame, greyroot, cv::COLOR_BGR2GRAY);
//        cv::GaussianBlur(greyroot, greyblur, cv::Size(51, 51), 0, 0);
//        //cv::imshow("blur", greyblur);
//        // cvt to binary image
//        cv::threshold(greyblur, greyframe, 200, 200, cv::THRESH_BINARY);
//        //cv::imshow("binary", greyframe);
//        //Moments
//        m1 = cv::moments(greyframe, true);
//        Point2d center_moment = {m1.m10 / m1.m00, m1.m01 / m1.m00};
//        circle(frame, center_moment, 10, Scalar(255, 0, 0), 3);
//
//        cv::findContours(greyframe, contours, cv::RETR_LIST, CHAIN_APPROX_NONE);
//
//
//        for(auto& contour : contours){
//            Point2f c; float r;
//            minEnclosingCircle(contour, c, r);
//            circles.push_back((_circle){c, r});
//        }
//
//        for(auto&  ci : circles){
//            circle(frame, ci.center, ci.radius, Scalar(0, 255, 0), 1);
//            circle(frame, ci.center, 1, Scalar(0, 255, 0), 1);
//
//            double distance = sqrt((center_moment.x-ci.center.x)*(center_moment.x-ci.center.x) + (center_moment.y-ci.center.y)*(center_moment.y-ci.center.y));
//            if(distance < limit){
//                centers.push_back(ci.center);
//            }
//        }
//
//
//
//        for(auto& c : centers){
//            std::cout << "Center Point: x :: " << c.x - frame_center.x << " y :: " << c.y - frame_center.y << std::endl;
//        }
//
//
//        //cv::resize(frame, smallframe, Size(), 1, 1);
//        cv::imshow("Webcam", frame);
//        // wait (10ms) for a key to be pressed
//
//        if (cv::pollKey() != -1) break;
*/
    int count = 0; int update = 100;
    tracking * track = new tracking(0, true);

    //tracking * track = new tracking(0, true);
    track->setup();
    while(1){
        if(count % update == 0){
            track->process_loop();
        }
        count++;
    }

    return 0;

}

//std::unique_ptr<generic> tracking_factory(std::string path, bool debug){
//    std::unique_ptr<generic> rtn = std::make_unique<tracking_module>(path, debug);
//}
//
//std::unique_ptr<generic> tracking_factory(int id, bool debug){
//    std::unique_ptr<generic> rtn = std::make_unique<tracking_module>(id, debug);
//}

[[maybe_unused]] std::unique_ptr<generic> tracking_factory(){
    return std::make_unique<tracking_module>(0, true);
}

