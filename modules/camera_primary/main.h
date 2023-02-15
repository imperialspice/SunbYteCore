//
// Created by middleton on 8/31/22.
//


#ifndef SUNBYTE_MAIN_H
#define SUNBYTE_MAIN_H

#include <exception>
#include <generics.h>
#include <opencv2/opencv.hpp>
#include <utility>
#include "settings.h"
#include "ASICamera2.h"
#include "deque"
//#include "thread"
#include "condition_variable"
#include "mutex"
#include "atomic"

extern Settings settings;


class science_exception : std::exception{
    std::string msg;

public:
    science_exception(char * message) : msg(message){}
    science_exception(std::string message) : msg(std::move(message)){};

//    const char* what() {
//        return msg.c_str();
//    }


};

struct raw_image_data {
    std::vector<unsigned char> data;
    size_t length;
    int height;
    int width;
};

class science_camera {
    bool debug = true;


    std::string image_path_template;


    int frameCount =0;


    // BIN
    long ASI_MAX_X_RES;
    long ASI_MAX_Y_RES;
    ASI_CAMERA_INFO cameraInfo;

    bool captureIsStarted = false;
    // processing thread
//    std::thread *processingThread;

    // cir buffer

    std::mutex mutex = {};
    std::condition_variable condition = {};
    std::atomic<bool> exitFLAG = false;
public:
    void appendList(raw_image_data rid){
        {
//            std::lock_guard<std::mutex> lock(mutex);
            cir.push_back(std::move(rid));

        }
        condition.notify_one();
    }
    raw_image_data popList(){
//        std::unique_lock<std::mutex> lock{mutex};
        while(cir.empty()){
//            condition.wait(lock);
        }
        auto tmp = std::move(cir.front());
        cir.pop_front();
        return tmp;
    }


    std::deque<raw_image_data> cir;
    science_camera(int cameraID, std::string imageSavePath);
    std::string take_image();
    ~science_camera();


    static void process_image(science_camera *_this);

    void exit();
};


class science_camera_module : public science_camera, public generic{
    std::string rtn;
public:
    science_camera_module(int cameraIndex, std::string save_path) : science_camera(cameraIndex, std::move(save_path)) {
        id = "tracking";
    }
//    science_camera_module(std::string cameraString, std::string save_path) : science_camera(cameraString, save_path){
//        id = "tracking";
//    }

    void run() override{
        msg = take_image();
    }

    template<class Archive>
    void serialize(Archive &archive){
        archive(id, msg);
    }




};

#endif //SUNBYTE_MAIN_H
