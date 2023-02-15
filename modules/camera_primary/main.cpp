//
// Created by middleton on 8/31/22.
//

#include "main.h"


#include <date.h>
#include <fstream>
#include "filesystem"
#include "messages.h"

#include <ASICamera2.h>

#define CAM_NUM 0
extern Message msg_Messages;

/*
 *
 *      this->capture = new cv::VideoCapture(this->cameraID, cv::CAP_V4L2);
        this->capture->set(cv::CAP_PROP_AUTO_EXPOSURE, 3);
        this->capture->set(cv::CAP_PROP_AUTO_EXPOSURE, 1);
        this->capture->set(cv::CAP_PROP_EXPOSURE, settings.getInt("tracking/exposure"));
        x_res = this->capture->get(cv::CAP_PROP_FRAME_WIDTH);
        y_res = this->capture->get(cv::CAP_PROP_FRAME_HEIGHT);
 *
 */

inline void getTemp(){
    long ltemp = 0;
    ASI_BOOL bAUTO = ASI_FALSE;
    ASIGetControlValue(CAM_NUM, ASI_TEMPERATURE, &ltemp, &bAUTO);

}
science_camera::science_camera(int cameraID, std::string imageSavePath) {
    std::cout << "Init Science 1" << std::endl;

        settings.registerSettings("science/binary", true, Bool);
        image_path_template = imageSavePath;

    std::cout << "Init Science 2" << std::endl;
//        s_cam = new cv::VideoCapture(internal_camera_id, cv::CAP_V4L2);
//        s_cam->set(cv::CAP_PROP_HW_ACCELERATION, cv::VIDEO_ACCELERATION_ANY);
        image_path_template = image_path_template.find("/", image_path_template.size()-1)==-1 ? image_path_template.append("/"):image_path_template;

        if(std::filesystem::exists(image_path_template))
            std::filesystem::create_directories(image_path_template);
    std::cout << "Init Science 3" << std::endl;
        // fine the ASI Camera
        int numberASI = ASIGetNumOfConnectedCameras();
        if(numberASI <= 0) {std::cout << "No science camera attached" << std::endl; ::exit(-1);}
        // get the settings of the ASI Camera
    std::cout << "Init Science 4" << std::endl;
        ASIGetCameraProperty(&cameraInfo, CAM_NUM);
        ASI_MAX_Y_RES = cameraInfo.MaxHeight;
        ASI_MAX_X_RES = cameraInfo.MaxWidth;

        // get the data from ASI Camera 1
        auto status = ASIOpenCamera(CAM_NUM);
        if(status != ASI_SUCCESS) {std::cout << "Unable to open Camera" << std::endl; ::exit(-2);}
    std::cout << "Init Science 5" << std::endl;
        auto i = ASIInitCamera(CAM_NUM);
        if(i != ASI_SUCCESS){
            std::cout << "Unable to INIT Camera" << std::endl;
        }
    std::cout << "Init Science 5a" << std::endl;
        auto err = ASIEnableDarkSubtract(CAM_NUM, "dark.bmp");
        if(err != ASI_SUCCESS){
            std::cout << "Unable to open darkfield" << std::endl;
        }
        getTemp();
    std::cout << "Init Science 6" << std::endl;
        settings.registerSettings("science/width", ASI_MAX_X_RES, Int);
        settings.registerSettings("science/height", ASI_MAX_Y_RES, Int);
        settings.registerSettings("science/bin", 1, Int);
        settings.registerSettings("science/exposure", 100, Int);
        ASISetROIFormat(CAM_NUM, settings.getInt("science/width"), settings.getInt("science/height"), settings.getInt("science/bin"), ASI_IMG_TYPE::ASI_IMG_RAW16);
    std::cout << "Init Science 7" << std::endl;
//        processingThread = new std::thread(process_image, this);

        // ensure that the mat is continous


}

void science_camera::exit(){
//    exitFLAG = true;
//    condition.notify_one();
//    processingThread->join();
}

std::string science_camera::take_image() {
    if (!settings.getBool("tracking/sun")) {
        ASIStopVideoCapture(CAM_NUM);
        captureIsStarted = false;
        return "unable to capture image, no sun in tracking image";
    }
    int width, height, bin;
    ASI_IMG_TYPE type = ASI_IMG_RAW16;
    ASIGetROIFormat(CAM_NUM, &width, &height, &bin, & type);
    // capture image from video capture
    if(!captureIsStarted){
        auto err = ASIStartVideoCapture(CAM_NUM);
        if(err != ASI_SUCCESS) {std::cout << "Unable to start video capture " << std::endl;}
        captureIsStarted = true;
    }
    // place image into deque for processing by seperate thread
    long size = width*height*2;
    auto * data = new unsigned char[size];

    auto st = ASIGetVideoData(CAM_NUM, data, size, 1000);
    if(st != ASI_SUCCESS){std::cout << "Something when wrong when reading the frame" << std::endl;}
    raw_image_data rw = {std::vector<unsigned char>(data, data+(size)), static_cast<size_t>(size), height, width};
    appendList(rw);
    delete[] data;
    return "Captured Image";
}

// run on a seperate thread

void science_camera::process_image(science_camera *_this){
//    while(!_this->exitFLAG){
//        auto data = _this->popList();
//        std::string file_name_no_extension = _this->image_path_template + "_" + std::to_string(_this->frameCount) + "_" +
//                                             date::format("%F %T", std::chrono::system_clock::now());
//        // write the raw image data to disk
//        std::ofstream writeRaw(file_name_no_extension+".raw16", std::ofstream::binary);
//        writeRaw.write(reinterpret_cast<const char *>(data.data.data()), data.data.size());
////    }
//    while(!_this->exitFLAG) {
//        std::cout << "thread" << std::endl;
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }
}



//    bool binary_or_yaml = settings.getBool("science/binary");
//    s_cam->read(currentImage);
//    frameCount++;
//    std::string rtn = "";
//    std::string file_name_no_extension = image_path_template + "_" + std::to_string(frameCount) + "_" +
//                                         date::format("%F %T", std::chrono::system_clock::now());
//    if (binary_or_yaml) {
//        // write the image out to the template path and inc the iterator
//        std::ofstream writeImage(file_name_no_extension + ".blob",
//                                 std::ofstream::binary | std::ios_base::app);
//
//        auto tmp_s = currentImage.reshape(1, currentImage.total() * currentImage.channels());
//        std::vector<char> save = tmp_s.isContinuous() ? tmp_s : tmp_s.clone();
//        std::string height = std::to_string(currentImage.size().height);
//        std::string width = std::to_string(currentImage.size().width);
//        std::string type = std::to_string(currentImage.type());
//
//        std::vector<char> header;
//        // spacer
//        header.insert(header.end(), spacer.begin(), spacer.end());
//        // height
//        header.insert(header.end(), height.begin(), height.end());
//        //spacer
//        header.insert(header.end(), spacer.begin(), spacer.end());
//        //width
//        header.insert(header.end(), width.begin(), width.end());
//        //s
//        header.insert(header.end(), spacer.begin(), spacer.end());
//        //type
//        header.insert(header.end(), type.begin(), type.end());
//        //s
//        header.insert(header.end(), spacer.begin(), spacer.end());
//        //image data
//        header.insert(header.end(), save.begin(), save.end());
//
//        writeImage.flush();
//        writeImage.close();
//        rtn = "Image Saved to Disk - Binary - " + file_name_no_extension + ".blob";
//    } else {
//        cv::FileStorage writeFile(file_name_no_extension + ".yml", cv::FileStorage::WRITE);
//        writeFile << "science_image" << currentImage;
//        rtn = "Image Saved to Disk - YML - " + file_name_no_extension + ".yml";
//    }
//
//    if(debug){
//        cv::imshow("ScienceDebug", currentImage);
//        cv::pollKey();
//    }

science_camera::~science_camera(){

        ASICloseCamera(CAM_NUM);
}



//
//int main(){
//    std::cout << "running sci cam test" << std::endl;
//    science_camera sci(0, "/home/middleton/sciImages");
//    for(;;){
//        std::string rtn = sci.take_image();
//        std::cout << rtn << std::endl;
//    }


//
//}

