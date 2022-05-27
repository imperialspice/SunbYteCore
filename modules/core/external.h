//
// Created by midd1eton on 10/04/2022.
//

#ifndef SUNBYTE_EXTERNAL_H
#define SUNBYTE_EXTERNAL_H

#include <pigpio.h>
#include "i2c/smbus.h"
#include "linux/i2c-dev.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <functional>
#include <cereal/archives/portable_binary.hpp>
#include <memory>

//#define 	BSC_FIFO_SIZE   16

#define ptr std::unique_ptr


class external {


};



class TCPStream{
    struct IPList {
        std::string IPv4;
        std::string IPv6;
        in_addr* ipv4;
        in6_addr* ipv6;
    };



private:
    IPList getIP(addrinfo* book);
    bool validateIP(addrinfo* book);

    int sockFD;
    addrinfo hints;
    sockaddr_un address;

    struct sockaddr_storage local_addr;
    socklen_t l_addr_s = sizeof(local_addr);


    // Stack variables for message divisions etc.
    char _m_div[3] = {0x0,0x3f,0x3f};


public:
    int socketID;
    addrinfo* res;
    sockaddr_un localaddr;

    int establish(const char * addr, const char *port);

    int remote(int socketID, addrinfo* res);


    void send(int socketfd, char *buffer, size_t buffer_len);
    std::string receive(int socketfd);






    int local(int socketID, addrinfo* res);
    int accept(std::function<int(int, TCPStream*)> message_loop);
    ~TCPStream();

    template<class Generic>
    void sendSerial(int socketfd, std::unique_ptr<Generic> generic);

    template<class Generic>
    std::unique_ptr<Generic> recieveSerial(int sockedfd);
};

class I2CStream{
    u_int8_t I2CBus;
    bool Controller; // IE. Master Mode
    char TxD[BSC_FIFO_SIZE];
    char RxD[BSC_FIFO_SIZE];
    unsigned int address;
    bsc_xfer_t slave;
    int control;
    int flags[14];
    void controlEdit(int address, int *flags);

    void open(int address);

    void read(unsigned int reg);
    void write(unsigned int reg);

    I2CStream(bool isController, u_int8_t I2CBus);
    ~I2CStream();


};

#endif //SUNBYTE_EXTERNAL_H
