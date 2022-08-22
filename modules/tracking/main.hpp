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
    double limit = 5;
    int update_frequency = 5; // update position every X frames
    int update_int = 0;
    int frameCount =0;
    _location *Location;
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


public:
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


protected:
    ~tracking(){
        delete capture;
    }


// frames etc.
std::string rtn = "";
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

    template<class Archive>
    void serialize(Archive &archive){
        archive(id, msg);
    }


};


[[maybe_unused]] std::unique_ptr<generic> tracking_factory();


#endif //SUNBYTE_MAIN_HPP


