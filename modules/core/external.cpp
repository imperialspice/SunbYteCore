//
// Created by midd1eton on 10/04/2022.
//
#include <iostream>
#include <cstring>
#include <streambuf>
#include "external.h"
#include "algorithm"
#include <future>

int localError = 0;


/*
 *
 * addr - (const char *) Socket Address
 * port - (int) for port | NULL for AF_UNIX socket
 */
int TCPStream::establish(const char * addr, const char *port) {
    std::cout << "Establishing Connection" << std::endl;

    int addr_status;

    memset(&hints, 0, sizeof hints);


    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    // Unix mode
    if(port == NULL){

        // create new res
        res = new addrinfo;


        if(strlen(addr) > sizeof(address.sun_path)){
            fprintf(stderr, "Error: %s\n", "Socket Path too long");
        }

        res->ai_family = AF_UNIX;
        res->ai_socktype = SOCK_STREAM;
        res->ai_protocol = 0;
        address.sun_family = AF_UNIX;
        strncpy(address.sun_path, addr, sizeof(address.sun_path));
        res->ai_addr = (struct sockaddr*) (&address);
        res->ai_addrlen = strlen(address.sun_path) + sizeof(address.sun_family);

    }
    // INET mode
    else if((addr_status = getaddrinfo(addr, port, &hints, &res)) != 0){
        fprintf(stderr, "Error: %s\n", gai_strerror(addr_status));
        exit(1);
    }

    auto a = TCPStream::getIP(res);

    this->socketID = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(this->socketID == -1){
        std::cerr << "Socket Error: " << errno <<  " Error Code: " << gai_strerror(errno) << std::endl;
    }

//    bind(this->socketID, res->ai_addr, res->ai_addrlen);


    return 0;
}

TCPStream::~TCPStream() {
    freeaddrinfo(res);
}

int TCPStream::remote(int socketID, addrinfo* res) {
    // let kernel choose local socket for outward connections
    int e;
    for(auto p = res; (p != nullptr && p != (void*)0x40); p = p->ai_next){
        e = connect(socketID, res->ai_addr, res->ai_addrlen);
        if(e == -1){
            if(p->ai_next != NULL){
                continue;
            }
            else{
                std::cout << "Socket Remote Error: " << errno <<  " Error Code: " << strerror(errno) << std::endl;
                exit(errno);
            }
        }
        else{
            break;
        }
    }

    return e;
}

int TCPStream::local(int socketID, addrinfo* res) {
    int yes = 1; // reuse addr setting
    // fist bind to a local socket
    if(setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))){
        std::cerr << "Set Socket Options Local Error: " << errno <<  " Error Code: " << strerror(errno) << std::endl;
        exit(errno);
    }

    int s = remove(((struct sockaddr_un*)res->ai_addr)->sun_path);
    if(s != 0){
        fprintf(stderr, "Socket could not be created\nExisting file could not be removed or replaced.\n");
    }

    int e = bind(socketID, res->ai_addr, res->ai_addrlen);
    if(e == -1){
        std::cerr << "Socket Local Error: " << errno <<  " Error Code: " << strerror(errno) << std::endl;
        exit(errno);
    }
    listen(socketID, 20);
    return e;
}

int TCPStream::accept(std::function<void(int, TCPStream *self)> message_loop){
    int connections = 0;
    for(;;){
        // seperate out as multithreaded means this object is shared across all threads, stack varables only.
        int socketFD = ::accept(socketID, (struct sockaddr *)&local_addr, &l_addr_s);
        connections++;
        fprintf(stdout, "Accepting Connection : %i\n", connections);
        if(socketFD == -1){
            std::cerr << "Local Accept Error: " << errno <<  " Error Code: " << gai_strerror(errno) << std::endl;
            continue;
        }
        // create new threads for each connection
//        int pid = getpid();
// this is passed only for access to thread_state, it might be easier to pass this directly. operations should be fine non-atomic
        std::thread _thread(message_loop, socketFD, this);
        createThreadLocks(std::move(_thread)); // pass to create thread locks
        std::cout << "thead created" << std::endl;

    }
    return 0;
}

void TCPStream::createThreadLocks(std::thread _thread){
    std::unique_ptr<std::condition_variable> _lock = std::make_unique<std::condition_variable>();
    std::unique_ptr<std::condition_variable> _lock2 = std::make_unique<std::condition_variable>();
    thread_lock _tmp = {std::move(_thread), std::move(_lock), std::move(_lock2)}; // move both into position
    auto id = _tmp._thread.get_id();
    thread_state.emplace(std::make_pair(id, false));
    threads.emplace(std::make_pair(id, std::move(_tmp)));
}

void TCPStream::createThreadLocks(std::thread::id thread_id){ // this one ignores the unique_ptr, since it isn't required for the different classes threads.
    std::thread _donotuse;
    std::unique_ptr<std::condition_variable> _lock = std::make_unique<std::condition_variable>();
    std::unique_ptr<std::condition_variable> _lock2 = std::make_unique<std::condition_variable>();
    thread_lock _tmp = {std::move(_donotuse), std::move(_lock), std::move(_lock2)}; // move both into position
    thread_state.emplace(std::make_pair(thread_id, false));
    threads.emplace(std::make_pair(thread_id, std::move(_tmp)));

}

void TCPStream::changeState(std::thread::id _id) {
    auto res = thread_state.find(_id);
    if(res != thread_state.end()) { // ensure not end
        // then change object state.
        res->second = true;
    }
}

void TCPStream::send(int socketfd, char *buffer, size_t buffer_len, bool flush = true){
    unsigned long sentBytes = 0;
    size_t remainingBytes = buffer_len;
    int failedCount = 0;
    while(sentBytes != buffer_len){
        //int sent = sendto(socketfd, buffer+sentBytes, remainingBytes,0, res->ai_addr, res->ai_addrlen);
        int sent;
        try {
            sent = ::send(socketfd, buffer + sentBytes, remainingBytes, 0);
        }
        catch (const char* mes){
            std::cout << mes << std::endl;
        }
        if(sent == -1){
            failedCount++;
            if(failedCount == 5){
                break;
            }
            continue;
        }
        sentBytes += sent;
        remainingBytes -= sent;
    }
    if(flush){
        ::send(socketfd, _m_div, sizeof _m_div, 0);
    }


}

std::string TCPStream::receive(int socketfd) {
    // are we doing serialisation? if we are it could save us time and effort.
    char buf[1024]; // if half of the flush bytes are sent after the rest of the message the buffer will never flush.
    std::string a;
    int failedCount = 0;
    [[maybe_unused]] int fullCount = 0;
    for(;;){

        //int sizeRead = recvfrom(socketfd, buf, sizeof buf - 1, 0, NULL,NULL);
        int sizeRead = recv(socketfd, buf, sizeof buf - 1, 0);
        fprintf(stdout, "Message Rec: %i bytes\n", sizeRead);
        if(sizeRead == -1 || sizeRead == 0){
            failedCount++;
            if(failedCount == 5){
                a = "Read Error: ";
                a.append(strerror(errno));
                localError = 1;
                std::cout << "Local Error Set" << std::endl;
                break;
            }
            continue;
        }
        int smaller = std::min((int) sizeof _m_div, sizeRead);
        if((memcmp(buf+sizeRead-smaller, _m_div, smaller) == 0)){
            for(int i = 0; i < (sizeRead-smaller); i++){
                a.push_back(*(buf+i));
            }

            break;
        }

        for(int i = 0; i < sizeRead; i++){
            a.push_back(*(buf+i));
        }
        fullCount += sizeRead;
    }

    return a;

}

//bool TCPStream::validateIP(addrinfo *book) {
//
//
//}

TCPStream::IPList TCPStream::getIP(addrinfo *book) {

    IPList ip = IPList();
    char ipstr[INET6_ADDRSTRLEN];
    addrinfo* p;
    for(p = book; (p != nullptr && p != (void*)0x40); p = p->ai_next){

        char* ipver;

        if(p->ai_family == AF_INET){
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            in_addr *addr = &(ipv4->sin_addr);
            ipver = (char*)"v4";
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            ip.ipv4 = addr;
            ip.IPv4 = ipstr;

        }
        else{
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            in6_addr *addr = &(ipv6->sin6_addr);
            ipver = (char*)"v6";
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            ip.ipv6 = addr;
            ip.IPv6 = ipstr;
        }

        printf(" %s: %s\n", ipver, ipstr);

    }

    return ip;
}

template<class Generic>
Generic rtnObj(_ptr<Generic> &generic){
    return *generic.get();
}

//template<class Generic>
void TCPStream::sendSerial(int socketfd, _ptr<generic> &generic) {
    //std::unique_ptr<char> tmp(new char[1024]);
    //char tmp[1024];
    char tmp[20];

    int count;
    // create a new stream for the data
    std::stringstream serial(std::ios::in|std::ios::out|std::ios::binary);
    // create new serialisation box

    {
        cereal::PortableBinaryOutputArchive archive = cereal::PortableBinaryOutputArchive(serial);

        //cereal::PortableBinaryOutputArchive arc(serial);
        archive(generic);

    }

    // std::cout << serial.rdbuf()->str() << "  :  "  << serial.rdbuf()->str().size() << std::endl;
    while(serial.good()){
        serial.read(tmp, 20);
        count = serial.gcount() == 20 ? 20 : serial.gcount();
        send(socketfd, tmp, count, false);
    }
    send(socketfd, nullptr, 0);

}

_ptr<generic>  TCPStream::receiveSerial(int socketfd) {
    std::string a = receive(socketfd);
    _ptr<generic> rec;
    if (localError != 0) {
        std::cout << "Local Error Unset" << std::endl;
        localError = 0;
        // likely no data sent, just return now
        rec = std::make_unique<Error>(a);
        return rec;
    }
    std::istringstream stream(a);

    { // ensure it is flushed to stream,
    cereal::PortableBinaryInputArchive archive = cereal::PortableBinaryInputArchive(stream);
//    Generic rec;
    archive(rec);

    }

    if(rec){
        return rec;
    }
    else{
        return std::make_unique<Error>("Invalid Message Received.");
    }
//    std::cout << "Message Type: " << rec->id << std::endl;

}

void TCPStream::thread_cleanup() {
    // iterate though threads and ready to join any which are exiting.
    start:
    std::cout << "1" << std::endl;
    auto thread_iterator = thread_state.begin();
    while(thread_iterator != thread_state.end()){
        std::cout << "2" << std::endl;
        if(thread_iterator->second){
            // thread is ready to exit.
            // fire the condition variable and join.
            auto& tmp = getThread(thread_iterator->first);
            std::cout << "3" << std::endl;
            std::mutex mtx;
            std::unique_lock<std::mutex> ul2(mtx); // create a mutex here to ensure that the exit is ready
            tmp.closing_thread_id = std::this_thread::get_id(); // give this threads id to the thread being closing so it can wait
//            tmp._lockReady->wait(ul2);
            //tmp._lock->notify_one();

            std::cout << "attempting to join" << std::endl;
            tmp._thread.join();
            std::cout << "thread joined" << std::endl;
            tmp._lock.reset();

            // erase the data from the maps
            threads.erase(thread_iterator->first);
            thread_state.erase(thread_iterator->first);
            // this invalidated the array iterators, and so they need to be reset.
            goto start;
        }
        thread_iterator = std::next(thread_iterator);
    }

}

TCPStream::thread_lock &TCPStream::getThread(std::thread::id _id) {
    thread_lock& tmp = threads.at(_id);
    return tmp;
}



// enable the templates
//template void TCPStream::sendSerial(int, std::unique_ptr<Message>&);
//template std::unique_ptr<Message> TCPStream::receiveSerial<Message>(int);
//template ptr<generic> TCPStream::receiveSerial(int);
//template void TCPStream::sendSerial(int, std::unique_ptr<generic>&);
//template void TCPStream::sendSerial(int, std::unique_ptr<EXE>&);
