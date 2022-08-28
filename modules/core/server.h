//
// Created by middleton on 6/8/22.
//



#include "external.h"
#include "internal.h"
#include "global.h"
#include <chrono>
#include <cmath>
#include "settings.h"
#include "messages.h"


class core{
private:

        static long _loopCount; // loop count is a control for the mainTaskList
        static bool isDebug; // adds extra verbosity
        // Interval time in ms
        static int intervalTime;
        static std::chrono::steady_clock::time_point exe_start; // timer start
        static std::chrono::steady_clock::time_point exe_finish; // timer finish
        static bool exit; // exit flag for server, breaks processing loop
        static TCPStream* tcp; // NOT THREAD SAFE
        std::shared_ptr<std::thread> thread_connect; // connection thread handle
        std::shared_ptr<std::thread> thread_process; // process thread handle

        inline static void clockStart();
        inline static void clockFinish();
        inline static long getTime(); // return inline time function for SPEED
        inline static std::map<std::string, task_handle> mainTaskList;

        // settings


public:
    static Settings settings;
    static std::shared_ptr<internal> _int; // internal list of tasks
    core(){ // initaliser
    _int = std::make_shared<internal>(); // create global internal
    isDebug = true;
    }

    static void loop(int, TCPStream*);

    static void process();
    void mt_process();
    /*
     * Underlying function shared with both server and client,
     * Depending on settings, will bind, or connect, to an address
     * @addr - Location of bound port
     * @port - Port Number, or NULL if unix Socket.
     */
    static void connect(std::string addr, std::string port);
    int mt_connect(std::string addr, std::string port);
    ~core();

//    static void add(std::string id, std::function<std::unique_ptr<generic>()> factory, int execution_time);

    static void addTasks();

    void add(std::string id, std::shared_ptr<generic> factory, int execution_time);
};

#ifndef SUNBYTE_SERVER_H
#define SUNBYTE_SERVER_H

#endif //SUNBYTE_SERVER_H
