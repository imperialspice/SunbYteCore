//
// Created by middleton on 8/11/22.
//

#ifndef SUNBYTE_MAIN_HPP
#define SUNBYTE_MAIN_HPP

#include "generics.h"
#include <iostream>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "settings.h"
#include "messages.h"
#include "motorDrive_UDP.h"
#include <cmath>

extern Settings settings;
extern Messages messageList;

class tracking{
private:
    cv::VideoCapture* capture;
    int cameraID = -1;
    std::string cameraPath = "";
    bool debug = false;
    // return string.
    struct _location{
        double minX;
        double minY;
        cv::Point min1;
        cv::Point max1;
    };
    struct _circle{
        cv::Point2f center;
        float radius;
    };
//    std::shared_ptr<void> limit;
    int noSunCount = 0; // update position every X frames
    int noneBeforeSearch = 10;
    int onSearchMoveCurrent = 0;
//    int update_int = 0;
    int frameCount =0;
//    _location *Location;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<_circle> circles;
    std::vector<cv::RotatedRect> ellipses;

    cv::Mat frame = {};
    cv::Mat greyframe = {};
    cv::Mat grey = {};
    cv::Mat grey2 = {};
    cv::Mat grey3 = {};
    cv::Mat grey4 = {};

    cv::Moments moments;
    std::vector<cv::Vec3f> h_circles;
    cv::Point2i frame_center;
    std::vector<cv::Point2d> centers;

    int y_stop = 8000;
    int x_stop = 33000;
    bool x_limit = false;
    bool y_limit = false;

    double x_res = 0, y_res = 0;
    int total_x_steps = 0, total_y_steps = 0;
    double sensor_x = 2.589;
    double sensor_y = 1.457;
    double focal_length = 3.67;
    int steps_per_rev = 32000;
    static constexpr double const_2pi = 2*M_PI;


    // solar tracking
    float strack_x = 0;
    float strack_y = 0;
    float subDivide_x = 6;
    float subDivide_y = 3;
    bool firstEntrySolarTracking = false;

public:

    // movement and tracking code


    motorDrive_UDP motorComms;

    float current_pitch, current_yaw = 0;


    tracking(int cameraIndex, bool debug){
        this->debug = debug; // set debug mode, ie show the window
        cameraID = cameraIndex;

        setup();

    }
    tracking(std::string cameraPath, bool debug){
        this->debug = debug;
        this->cameraPath = cameraPath;
        setup();
    }
    void setup();
    void debug_file(std::string path);

    void task_loop();
    void process_loop();
    void sunScan();




protected:
    ~tracking(){
        delete capture;
    }


// frames etc.
std::string rtn = "";

    void moveToPosition(double, double);

    void stepToAbsolutePosition(int stepx, int stepy);

    void stepToRelativePosition(int stepx, int stepy);
};


class tracking_module : public generic, tracking{
public:

    tracking_module(int cameraIndex, bool debug) : tracking(cameraIndex, debug) {
        id = "tracking";
    }
    tracking_module(std::string cameraString, bool debug) : tracking(cameraString, debug){
        id = "tracking";
    }

    void run() override{
        process_loop();
        msg = rtn;
    }


};

[[maybe_unused]] std::unique_ptr<generic> tracking_factory();


#endif //SUNBYTE_MAIN_HPP


