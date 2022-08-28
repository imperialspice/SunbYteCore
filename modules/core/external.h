//
// Created by midd1eton on 10/04/2022.
//

#ifndef SUNBYTE_EXTERNAL_H

#define SUNBYTE_EXTERNAL_H


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <functional>
#include <memory>
#include <generics.h>
#include <thread>
#include <condition_variable>


#define _ptr std::shared_ptr


class external {


};



class TCPStream{
    struct IPList {
        std::string IPv4;
        std::string IPv6;
        in_addr* ipv4;
        in6_addr* ipv6;
    };
public:
    struct thread_lock{
        std::thread _thread;
        std::unique_ptr<std::condition_variable> _lock;
        std::unique_ptr<std::condition_variable> _lockReady; // acts as a second lock to ensure that the exit flag is ready for an exit
        std::thread::id closing_thread_id = {};
    };



private:
    IPList getIP(addrinfo* book);
    bool validateIP(addrinfo* book);

    int sockFD;
    addrinfo hints;
    sockaddr_un address;

    struct sockaddr_storage local_addr;
    socklen_t l_addr_s = sizeof(local_addr);



    //thread list
    std::map<std::thread::id, thread_lock> threads;
    std::map<std::thread::id, bool> thread_state;

    // Stack variables for message divisions etc.
    constexpr static char _m_div[1] = {0x04}; // end of transmission.


    // async thread cleanup processor
    void thread_cleanup();

public:



    int socketID;
    addrinfo* res;
    sockaddr_un localaddr;

    int establish(const char * addr, const char *port);

    int remote(int socketID, addrinfo* res);

    void changeState(std::thread::id _id);
    thread_lock& getThread(std::thread::id _id);

    static void send(int socketfd, char *buffer, size_t buffer_len, bool flush);
    static std::string receive(int socketfd);






    int local(int socketID, addrinfo* res);

    int accept(std::function<void(int, TCPStream *self)> message_loop);
    ~TCPStream();

//    template<class Generic>
    static void sendSerial(int socketfd, _ptr<generic> &generic);

//    template<class Generic>
    static _ptr<generic> receiveSerial(int sockedfd);

    void createThreadLocks(std::thread _thread);

    void createThreadLocks(std::thread::id thread_id);
};




#endif //SUNBYTE_EXTERNAL_H
