//
// Created by middleton on 6/8/22.
//

#include "server.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <csignal>

// OLD Code
//std::shared_ptr<internal> global::_global_internal = std::make_shared<internal>();


//
//int accept_loop (int sockfd, TCPStream *_this){
//    int len = 0;
//    unsigned int flags =0;
//    struct sockaddr *from;
//    socklen_t *fromlen;
//    char hold[20] = "not implemented\0";
//    std::cout << "Setting up message loop" << std::endl;
//    while(1) {
//
//        std::string msg = _this->receive(sockfd);
//        fprintf(stdout, "Server Copy: %s\n", msg.c_str());
//
//        if (msg.compare("quit") == 0) {
//            std::cout << "Closing Server" << std::endl;
//            break;
//        }
//        _this->send(sockfd, hold, sizeof hold, true);
//
//    }
//    return 0;
//}
//
//
//int serial_accept_loop(int sockfd, TCPStream *_this){
//    int len = 0;
//    unsigned int flags =0;
//    struct sockaddr *from;
//    socklen_t *fromlen;
//    ptr<generic> rtn(new Message);
//    rtn->msg = "not implemented";
//
//    std::cout << "Setting up message loop" << std::endl;
//    while(true) {
//
//        ptr<generic> msg = _this->receiveSerial(sockfd);
//        if(msg->id == "error"){ break; }
//        // caller handles thread wait and locking so that message can be processed with little cpu time.
//        std::shared_ptr<generic_handle> caller = global::_global_internal->append(std::move(msg));
//        std::cout << getpid() << " : " << global::_global_internal->size() << std::endl;
//        // acquire a wait
//        std::unique_lock<std::mutex> lock(*caller->lock);
//        caller->flag->wait(lock);
//        // wait until the flag is notified
//
//
////        fprintf(stdout, "Server Copy: %s\n", msg->msg.c_str());
////        std::cout << msg->id << std::endl;
////        msg->run();
////        if (msg->msg == "quit") {
////            std::cout << "Closing Server" << std::endl;
////            break;
////        }
////        fprintf(stdout, "Sending data\nid: %s\nmsg: %s", msg->id.c_str(), msg->msg.c_str());
//        _this->sendSerial(sockfd, caller->msg);
//        // destroy caller message by poping it from the list.
//        global::_global_internal->consume();
//        caller.reset();
//
//    }
//    return 0;
//}

//
//int test_archive(){
//
//}
//
//int test_unarchive(){
//
//}

//void quickProcess(){
//    while(true){
//        if (global::_global_internal->size() != 0) {
//            auto a = global::_global_internal->getShared();
//            a->msg->run();
//            a->flag->notify_one();
//            std::cout << a->id << std::endl;
//        }
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(20));
//
//    }
//}



//void establishConnectionThread(){
//    auto b = new TCPStream();
//    b->establish("/home/middleton/test123", NULL);
//    b->local(b->socketID, b->res);
//    //b->accept(accept_loop);
//    b->accept(serial_accept_loop);
//}

// static redec

extern Settings settings;


long core::_loopCount = 0;
bool core::isDebug = true;
int core::intervalTime = 20;
std::shared_ptr<internal> core::_int;
TCPStream* core::tcp;
std::chrono::steady_clock::time_point core::exe_start;
std::chrono::steady_clock::time_point core::exe_finish;
bool core::exit = false;

void core::clockStart(){
    exe_start = std::chrono::steady_clock::now();
}
void core::clockFinish() {
    exe_finish = std::chrono::steady_clock::now();
}
long core::getTime() {
    clockFinish();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(exe_finish - exe_start);
    return time.count();
}


void core::loop(int sockfd, TCPStream *_handle) {

    _ptr<generic> rtn(new Message);
    rtn->msg = "not implemented";
    if(isDebug) std::cout << "Setting up message loop" << std::endl;
    while(true) {
        _ptr<generic> msg = TCPStream::receiveSerial(sockfd);
        if (msg->id == "error") {
            if (isDebug) std::cout << "Error Flag is set, exiting message loop." << std::endl;
            break;
        }
            // move the task to the execution list
            std::shared_ptr<generic_handle> caller = _int->append(msg);
            // print the size of the task list.
            if (isDebug) std::cout << "Task list is now: " << _int->size() << std::endl;
            // wait until the execution has finished.
            std::unique_lock<std::mutex> lock(*(caller->lock));
            caller->flag->wait(lock);
            TCPStream::sendSerial(sockfd, caller->msg);

    }
    std::thread::id thread_id = std::this_thread::get_id();
    _handle->changeState(thread_id); // ready for exit // signal ready for join
    std::cout << "thread closing" << std::endl;


}


// call process after setup complete, will sit in this loop until an exit event
// AUG 22 - Redesign to account for task adds, clears the list and then adds new tasks

void core::process() {

    while(!exit){
        // only process if there are jobs available
        _loopCount = 0;
        if(_int->size() == 0){
           addTasks();
        }
        clockStart();
        if(_int->size() > 0){
            // get a copy of the jobs shared pointer
            auto job = _int->getShared();
            // execute the job
            job->msg->run();
            // notify the flag within the job unit to unlock the waiting thread
            job->flag->notify_one();
        }
        long waitMS = (intervalTime - getTime());
        waitMS = waitMS > 0 ? waitMS: 0;
        //std::cout << "time wait " << waitMS << " getTime() " << getTime() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(waitMS));
    }
}

void core::connect(std::string addr, std::string port) {
            tcp = new TCPStream();

            const char * c_port = port.empty() ? NULL:port.c_str(); // conversion for connection
            const char * c_addr = addr.empty() ? NULL:addr.c_str();
            tcp->establish(c_addr, c_port);
            tcp->local(tcp->socketID, tcp->res);
            tcp->accept(loop); // needs static memeber, has this passed as parameter
}

int core::mt_connect(std::string addr, std::string port) {
    thread_connect = std::make_shared<std::thread>(this->connect, addr, port);

    return 0;
}

void core::mt_process() {
    thread_process = std::make_shared<std::thread>(this->process);
}

// schedule a task for execution within the task loop
// every iteration needs to be added upon loop completion

void core::add(std::string id, std::shared_ptr<generic> factory, int execution_time){

    task_handle _th = {std::move(factory), execution_time};
    mainTaskList.emplace( std::make_pair(id, std::move(_th)));
}

void core::addTasks(){
    for(auto &a : mainTaskList){
        if(_loopCount % a.second.task_occurance == 0){ // on division add to loop
            // add pointer to list
            _int->append((a.second._generic_handle)); // the handler is ignored since the task is headless, ie no output
        }

    }
}



core::~core() {
    thread_process->join();
    thread_connect->join();
    delete tcp;
}
