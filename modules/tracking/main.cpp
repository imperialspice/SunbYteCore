#include <iostream>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"


using namespace cv;

int main() {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
    //cv::VideoCapture camera("/home/midd1eton/Projects/sunbytecore/modules/tracking/res/sun_no_clouds.mp4");
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
    struct _location{
        double minX;
        double minY;
        Point min1;
        Point max1;
    };
    struct _circle{
        Point2f center;
        float radius;
    };

    double limit = 50;
    int update_frequency = 5; // update position every X frames
    int update_int = 0;
    int frameCount =0;
    auto* Location = new _location();
    std::vector<std::vector<cv::Point>> contours;
    std::vector<_circle> circles;
    std::vector<cv::RotatedRect> ellipses;
    cv::Mat smallframe;
    cv::Mat greyroot;
    cv::Mat greyblur;
    cv::Moments m1;
    std::vector<Vec3f> h_circles;
    Point2i frame_center;
    std::vector<Point2d> centers;
    // capture the next frame from the webcam


    // display the frame until you press a key
    while (1) {
        camera.read(frame);
        frameCount++;

        if(update_int != update_frequency){
            update_int++;
            continue;

        }
        update_int = 0;
        frame_center = {frame.cols/2, frame.rows/2};


        // show the image on the window
        ellipses.clear();
        circles.clear();
        // convert to grey
        cv::cvtColor(frame, greyroot, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(greyroot, greyblur, cv::Size(51, 51), 0, 0);
        //cv::imshow("blur", greyblur);
        // cvt to binary image
        cv::threshold(greyblur, greyframe, 200, 200, cv::THRESH_BINARY);
        //cv::imshow("binary", greyframe);
        //Moments
        m1 = cv::moments(greyframe, true);
        Point2d center_moment = {m1.m10 / m1.m00, m1.m01 / m1.m00};
        circle(frame, center_moment, 10, Scalar(255, 0, 0), 3);

        cv::findContours(greyframe, contours, cv::RETR_LIST, CHAIN_APPROX_NONE);


        for(auto& contour : contours){
            Point2f c; float r;
            minEnclosingCircle(contour, c, r);
            circles.push_back((_circle){c, r});
        }

        for(auto&  ci : circles){
            circle(frame, ci.center, ci.radius, Scalar(0, 255, 0), 1);
            circle(frame, ci.center, 1, Scalar(0, 255, 0), 1);

            double distance = sqrt((center_moment.x-ci.center.x)*(center_moment.x-ci.center.x) + (center_moment.y-ci.center.y)*(center_moment.y-ci.center.y));
            if(distance < limit){
                centers.push_back(ci.center);
            }
        }



        for(auto& c : centers){
            std::cout << "Center Point: x :: " << c.x - frame_center.x << " y :: " << c.y - frame_center.y << std::endl;
        }


        //cv::resize(frame, smallframe, Size(), 1, 1);
        cv::imshow("Webcam", frame);
        // wait (10ms) for a key to be pressed

        if (cv::pollKey() != -1) break;
    }
    return 0;

}
