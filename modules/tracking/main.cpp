#include <iostream>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace cv;

int main() {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // create a window to display the images from the webcam
    cv::namedWindow("Webcam", WINDOW_AUTOSIZE);

    // this will contain the image from the webcam
    cv::Mat frame;

    // capture the next frame from the webcam


    // display the frame until you press a key
    while (1) {
        camera >> frame;
        // show the image on the window
        cv::imshow("Webcam", frame);
        // wait (10ms) for a key to be pressed
        sleep(0.1);
        if (cv::pollKey() != -1) break;
    }
    return 0;

}
