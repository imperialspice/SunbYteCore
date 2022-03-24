#include <iostream>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"


using namespace cv;

int main() {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
//    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 500);
//    //camera.set(cv::CAP_PROP_FRAME_WIDTH, 500);
    camera.set(cv::CAP_PROP_FPS, 30);



    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }




    // create a window to display the images from the webcam
    cv::namedWindow("Webcam", WINDOW_NORMAL| WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);

    // this will contain the image from the webcam
    cv::Mat frame;
    cv::Mat greyframe;
    struct location{
        double minX;
        double minY;
        Point min1;
        Point max1;
    };
    auto* Location = new location();
    std::vector<std::vector<cv::Point>> circles;
    std::vector<cv::RotatedRect> ellipses;
    cv::Mat smallframe;
    // capture the next frame from the webcam


    // display the frame until you press a key
    while (1) {
        camera >> frame;
        // show the image on the window
        ellipses.clear();
        cv::cvtColor(frame, greyframe, cv::COLOR_BGR2GRAY);
//        // convert to grey
//        cv::minMaxLoc(greyframe, &Location->minX, &Location->minY, &Location->min1, &Location->max1);
//        cv::circle(frame, Location->max1, 10, cv::Scalar(255, 0, 0), 4);
        cv::threshold(greyframe, greyframe, 200, 255, cv::THRESH_BINARY);
        cv::findContours(greyframe, circles, cv::RETR_LIST, CHAIN_APPROX_NONE);

        for(int idx = 0; idx < circles.size(); idx++){
            std::vector<cv::Point> _smallC = circles.at(idx);
            if(_smallC.size() > 100){
                ellipses.push_back(cv::fitEllipse(_smallC));
            }

        }


        for(int idx = 0; idx < circles.size(); idx++){
            cv::drawContours(frame, circles, idx, Scalar(200, 200, 1), 1);
        }
        for(auto& rect : ellipses){
            cv::ellipse(frame, rect, Scalar(0, 255, 0), 5);
        }

        cv::resize(frame, smallframe, Size(), 1, 1);
        cv::imshow("Webcam", smallframe);
        // wait (10ms) for a key to be pressed

        if (cv::pollKey() != -1) break;
    }
    return 0;

}
