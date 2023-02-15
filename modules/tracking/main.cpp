#include "main.hpp"
#include "ctime"
#include "iostream"
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
    settings.registerSetting("tracking/exposure", 500, Int);
    settings.registerSetting("tracking/enabled", true, Bool);
    settings.registerSetting("tracking/enableMotors", false, Bool);
    settings.registerSetting("tracking/sun", false, Bool);
    settings.registerSetting("tracking/sunScan", false, Bool);
    //limit = (settings.getPointer("tracking/limit")).;
    settings.registerSetting("tracking/noMovesFromSearch", 100, Int);



    if(this->cameraID == -1){
        this->capture = new cv::VideoCapture(this->cameraPath, cv::CAP_V4L2);
        this->capture->set(cv::CAP_PROP_FRAME_HEIGHT, 500);
        this->capture->set(cv::CAP_PROP_FRAME_WIDTH, 500);

    }
    else{
        this->capture = new cv::VideoCapture(this->cameraID, cv::CAP_V4L2);
        this->capture->set(cv::CAP_PROP_AUTO_EXPOSURE, 3);
        this->capture->set(cv::CAP_PROP_AUTO_EXPOSURE, 1);
        this->capture->set(cv::CAP_PROP_EXPOSURE, settings.getInt("tracking/exposure"));
        x_res = this->capture->get(cv::CAP_PROP_FRAME_WIDTH);
        y_res = this->capture->get(cv::CAP_PROP_FRAME_HEIGHT);
    }


    capture->set(cv::CAP_PROP_FPS, 30);
    if(!capture->isOpened()){
        std::cout << "Unable to open the camera object" << std::endl;
        exit(-1);
    }
//    if(this->debug){
//        std::cout << "Creating Debug Window" << std::endl;
//        cv::namedWindow("Grey3", WINDOW_AUTOSIZE);
//        cv::namedWindow("TrackingDebug", WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | WINDOW_AUTOSIZE);
//        cv::pollKey();
//    }


}



void tracking::task_loop() {


}
/* This is run every iteration
 *  Performs a camera read and then processes it to generate the offset values
 *
 */
void tracking::process_loop(){
    double solar_pos_x, solar_pos_y = 0;
    int limit = settings.getInt("tracking/limit");
    this->capture->set(cv::CAP_PROP_EXPOSURE, settings.getInt("tracking/exposure"));
    if(!settings.getBool("tracking/enabled")) return;

    capture->read(frame); // read from the camera object
    frameCount++; // iterate frame count
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

    //collect moments
    moments = cv::moments(grey3, true);
    // Points
    cv::Point2d center_moment = {moments.m10 / moments.m00, moments.m01 / moments.m00};
    // draw a circle
    cv::circle(frame, center_moment, limit, cv::Scalar(255, 0, 0), 3);
    // find countors
    cv::findContours(grey3, contours, cv::RETR_LIST, CHAIN_APPROX_NONE);
   // check if empty
    if(contours.empty()){
        noSunCount++;
    }

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
              solar_pos_x = (center_moment.x + c.x)/2 - x_res/2; solar_pos_y = (center_moment.y + c.y)/2 - y_res/2;
              rtn = "Possible sun found: \nX: " + std::to_string(solar_pos_x)  + "  Y:  " + std::to_string(solar_pos_y);

              strack_x = 0; strack_y =0; // reset sun search so it is able to restart from origin
              firstEntrySolarTracking = false;
              noSunCount = 0;
              break;
          }
          else{
            noSunCount++;
          }
    }

    if( settings.getBool("tracking/sunScan") && (noSunCount > noneBeforeSearch)){
        std::cout << "SunSearch Method Enabled" << std::endl;
        // disable capturing
        settings.set("tracking/sun", Value(std::make_shared<bool>(false), Bool));
        sunScan();
    }

    //cv::resize(frame, smallframe, Size(), 1, 1);
//    if(this->debug){
//        cv::imshow("Grey3", grey3);
//        cv::imshow("TrackingDebug", frame);
//        cv::pollKey();
//    }
    if(noSunCount == 0){
        // if limits are small then enable the sci camera
        if(solar_pos_x < settings.getInt("tracking/solarLimit") && solar_pos_y < settings.getInt("tracking/solarLimit")){
            settings.set("tracking/sun", Value(std::make_shared<bool>(true), Bool));
        }
        else{ // TODO: Not sure about this one
            settings.set("tracking/sun", Value(std::make_shared<bool>(false), Bool));
        }

            moveToPosition(solar_pos_x, solar_pos_y);

    }

    if(!rtn.empty()) std::cout << "Response: " << rtn << std::endl;

    // use the end of the tree to append the images.
    messageList.set("tracking/grey", grey3);
    messageList.set("tracking/image", frame);
    FileStorage fs(std::string("trackingimg_")+std::to_string(std::time(0))+std::string(".json"), FileStorage::WRITE);
    fs.write("_file"+std::to_string(std::time(0)), frame);
    fs.release();

}

void tracking::sunScan() {
    // block attempts to move until there has been var searches
    if(onSearchMoveCurrent < settings.getInt("tracking/noMovesFromSearch")){onSearchMoveCurrent++; return;};
    onSearchMoveCurrent = 0;
    // assume the current position is zero, then
    // get information from the Rpi sensors about rotation and other information
    // move to the limits of the motor and then all the way back around
    if(!firstEntrySolarTracking){
        stepToAbsolutePosition(0,0);
        firstEntrySolarTracking = true;
    }
    strack_x++;
    if(strack_x > subDivide_x){ strack_x= 0; strack_y++;}
    if(strack_y > subDivide_y){messageList.set("tracking/onTarget", "Unable to find in one pass"); strack_y = 0;}
    std::cout << "Current Track: X : " << (strack_x/subDivide_x)*x_stop << " Current Track Y: " << (strack_y/subDivide_y)*y_stop << std::endl;
    stepToAbsolutePosition((strack_x/subDivide_x)*x_stop, (strack_y/subDivide_y)*y_stop);


    // step to starting position

    // space divided up into 6 segments on the x axis and 3 on the y, the program should return to the tracking core to determine the solar position

}
// start to move to position and get as close as possible in the 5ms then refresh
// assume taken from 0,0
void tracking::moveToPosition(double x_coord, double y_coord){
    // convert to real space from pixels
    double real_x, real_y = 0;
    real_x = (x_coord/x_res)*sensor_x;
    real_y = (y_coord/y_res)*sensor_y;
    double angle_x, angle_y = 0;

    std::cout << "R_X: " << real_x << "\nR_Y: " << real_y << std::endl;


    angle_x = 2*atan(real_x/(2*focal_length));
    angle_y = 2*atan(real_y/(2*focal_length));

    std::cout << "A_X: " << angle_x << "\nA_Y: " << angle_y << std::endl;


    // calculate steps to position.
    int steps_x = steps_per_rev*(angle_x/const_2pi);
    int steps_y = steps_per_rev*(angle_y/const_2pi);

    std::cout << "Steps to target X : " << steps_x << " Steps to target Y: " << steps_y << std::endl;
    if(settings.getBool("tracking/enableMotors")) {
        stepToRelativePosition(steps_x, steps_y);
    }
}

void tracking::stepToAbsolutePosition(int stepx, int stepy){
    int stx = stepx - total_x_steps;
    int sty = stepy - total_y_steps;
    tracking::stepToRelativePosition(stx, sty);

}


void tracking::stepToRelativePosition(int stepx, int stepy){

    // X AND Y NEED TO BE SWAPPED FOR CUrreNT MOTOR CONF
    int t = stepx;
    stepx = stepy;
    stepy = t;

    std::cout << "Relative position CMD" << std::endl;
    std::cout << "X: " << stepx << " Y: " << stepy << std::endl;
    if(stepx == 0 && stepy == 0) return;
    if(total_x_steps+stepx > x_stop) stepx = x_stop - total_x_steps;
    if(total_y_steps+stepx > y_stop) stepx = y_stop - total_y_steps;
    // construct the serial message to the ard

    std::string dirx = stepx > 0 ? "1":"0";
    std::string diry = stepy > 0 ? "1":"0";
    std::string zero(std::to_string(0));


    // ensure is abs
    stepx = abs(stepx);
    stepy = abs(stepy);

    // take the int and ensure that any overflow is handled
    bool overflowEnabled = false;
    int overflow_x;
    int overflow_y ;
    if(stepx > 9999){
        overflow_x = abs(stepx) - 9999;
        stepx = 9999;
        overflowEnabled = true;
    }
    if(stepy > 9999){
        overflow_y = abs(stepy) - 9999;
        stepy = 9999;
        overflowEnabled = true;
    }
    while(overflowEnabled){

        int stepxDir = stepx > 0 ? 1:-1;
        int stepyDir = stepy > 0 ? 1:-1;
        stepx = abs(stepx);
        stepy = abs(stepy);
        std::string sstepx = std::to_string(stepx);
        std::string sstepy = std::to_string(stepy);


        while(sstepx.size() < 4){
            sstepx.insert(0, zero);
        }
        while(sstepy.size() < 4){
            sstepy.insert(0, zero);
        }


        std::string motorCMD;
        motorCMD.append(sstepx);
        motorCMD.append(dirx);
        motorCMD.append(sstepy);
        motorCMD.append(diry);

        std::cout << motorCMD << std::endl;


        total_x_steps  = total_x_steps + (stepx*stepxDir);
        total_y_steps = total_y_steps + (stepy*stepyDir);


        if(overflow_x>0){
            stepx = overflow_x > 9999 ? 9999:overflow_x;
            stepx = stepx*stepxDir;
            overflow_x = overflow_x-9999>0 ? overflow_x-9999:0;
        }else{
            stepx = 0;
        }
        if(overflow_y>0){
            stepy = overflow_y > 9999 ? 9999:overflow_y;
            stepy = stepy*stepyDir;
            overflow_y = overflow_y-9999>0 ? overflow_y-9999:0;
        }
        else{
            stepy =0;
        }

        if((overflow_y==0&&stepy==0) && (overflow_x==0&&stepx==0)){
            overflowEnabled = false;
        }
    }
    



/*    // check the constraits
//    if(total_x_steps + stepx >= x_stop || total_x_steps + stepx <= 0) {// will be out of range in movment limit
//        stepx = total_x_steps + stepx > total_x_steps ? x_stop - total_x_steps : total_x_steps;
//        x_limit = true;
//     }
//    if(total_x_steps + stepy >= y_stop || total_y_steps + stepy <= 0 ) {// will be out of range in movment limit
//        stepy = total_y_steps + stepy > total_y_steps ? y_stop - total_y_steps : total_y_steps;
//        y_limit = true;
//     }
//
//
//            // unsure of the drivers but these should work well.
//    int larger = abs(stepx) >= abs(stepy) ? stepx:stepy;
//    int smaller = abs(stepy) > abs(stepx) ? stepy:stepx;
//
//    gpioDrivers *largerDriver = abs(stepx) >= abs(stepy) ? m1.get():m2.get();
//    gpioDrivers *smallerDriver = abs(stepy) > abs(stepx) ? m1.get():m2.get();
//
//    if(larger > 0) {  largerDriver->setDirection(false);
//    }
//    else { largerDriver->setDirection(true);
//    }
//    if(smaller > 0) { smallerDriver->setDirection(false);
//    }
//    else { smallerDriver->setDirection(true);
//    } // TODO: CONFIRM THIS
//
//    int run = stepx;
//    if(stepy != 0){
//        run = round(stepx/stepy);
//    }
//
//
//    int localLarger = 0;
//    int localSmaller = 0;
//
//    // for diagonal movement
//    for(;localLarger < larger; localLarger++){
//        if(localLarger%run == 0){
//            smallerDriver->step(); //TODO: Uncomment
//            localSmaller++;
//        }
//        largerDriver->step(); // TODO: Uncomment
    }
 */

//    // total x steps away from origin
//    // larger
//    if(abs(stepx) >= abs(stepy)) {
//        // step x
//        total_x_steps = total_x_steps + larger;
//    }
//    else{
//        // step y
//        total_y_steps = total_y_steps + larger;
//    }
//
//    // smaller
//    if(abs(stepy) > abs(stepx)){
//        // step y
//        total_y_steps = total_y_steps + smaller;
//    }
//    else{
//        // step x
//        total_x_steps = total_x_steps + smaller;
//    }


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

