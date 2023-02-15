//
// Created by middleton on 02/09/22.
//

#include "external.h"
#include "iostream"
#include "server.h"


// TODO: Intergrate the switch system
// TODO: Build the data collection
// TODO: Implment the headers and temp sensors

int main(){
    std::cout << "Starting RPI Server" << std::endl; // primary data logging etc.

    std::string a;

//    auto a= new TCPStream();
//    a->establish(NULL, "2223");
//    a->local(a->socketID, a->res);
//    a->accept(accept_loop);
//    std::thread tcp = std::thread(establishConnectionThread);
//    std::thread process = std::thread(quickProcess);

    core test = core();

    test.mt_connect(NULL, "42069");
    test.mt_process();
    //test.thread_cleanup(); // run cleanup here? I cant think of anywhere else to put it really.
    std::getline(std::cin, a);
    exit(0);




}


/*
 *
 * {

    std::string a;

//    auto a= new TCPStream();
//    a->establish(NULL, "2223");
//    a->local(a->socketID, a->res);
//    a->accept(accept_loop);
//    std::thread tcp = std::thread(establishConnectionThread);
//    std::thread process = std::thread(quickProcess);

    core test = core();
    std::shared_ptr<generic> tracking = std::make_shared<tracking_module>(0, true);
//    std::shared_ptr<generic> science = std::make_shared<science_camera_module>(0, "/home/odroid/science");
    test.add("tracking", tracking, 10);
//    test.add("drivers_m", science, 10);
    test.mt_connect("", "42069");
    test.mt_process();
    //test.thread_cleanup(); // run cleanup here? I cant think of anywhere else to put it really.
    std::getline(std::cin, a);
    exit(0);

 *
 */