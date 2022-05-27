//
// Created by midd1eton on 10/04/2022.
//
#include <iostream>
#include <cstring>
#include <streambuf>
#include "external.h"
#include "algorithm"


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
    for(auto p = res; p != NULL; p = p->ai_next){
        e = connect(socketID, res->ai_addr, res->ai_addrlen);
        if(e == -1){
            if(p->ai_next != NULL){
                continue;
            }
            else{
                std::cout << "Socket Remote Error: " << errno <<  " Error Code: " << gai_strerror(errno) << std::endl;
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
        fprintf(stderr, "Socket could not be created\nExisting file could not be removed or replaced.");
    }
    int e = bind(socketID, res->ai_addr, res->ai_addrlen);
    if(e == -1){
        std::cerr << "Socket Local Error: " << errno <<  " Error Code: " << strerror(errno) << std::endl;
        exit(errno);
    }
    listen(socketID, 20);
    return e;
}

int TCPStream::accept(std::function<int(int, TCPStream*)> message_loop){
    for(;;){
        sockFD = ::accept(socketID, (struct sockaddr *)&local_addr, &l_addr_s);
        fprintf(stdout, "Accepting Connection : \n");
        if(sockFD == -1){
            std::cerr << "Local Accept Error: " << errno <<  " Error Code: " << gai_strerror(errno) << std::endl;
            continue;
        }
        int pid = 0;
//        fork();
        if(pid == 0){
            if(message_loop(sockFD, this) == -1) {
                close(sockFD);
            break; }
        }

       close(sockFD);

    }
    return 0;
}

void TCPStream::send(int socketfd, char *buffer, size_t buffer_len){
    long sentBytes = 0;
    size_t remainingBytes = buffer_len;
    int failedCount = 0;
    while(sentBytes != buffer_len){
        //int sent = sendto(socketfd, buffer+sentBytes, remainingBytes,0, res->ai_addr, res->ai_addrlen);
        int sent = ::send(socketfd, buffer+sentBytes, remainingBytes,0);
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
    ::send(socketfd, this->_m_div, sizeof this->_m_div, 0);

}

std::string TCPStream::receive(int socketfd) {
    // are we doing serialisation? if we are it could save us time and effort.
    char buf[1000];
    std::string a;
    int failedCount = 0;
    int fullCount = 0;
    for(;;){

        //int sizeRead = recvfrom(socketfd, buf, sizeof buf - 1, 0, NULL,NULL);
        int sizeRead = recv(socketfd, buf, sizeof buf - 1, 0);
        std::cout << "read data " << sizeRead << " " << buf << std::endl;
        if(sizeRead == -1 || sizeRead == 0){
            failedCount++;
            if(failedCount == 5){
                fprintf(stderr, "Read Error: %s", gai_strerror(errno));
                break;
            }
            continue;
        }
        int smaller = std::min((int) sizeof this->_m_div, sizeRead);
        if((memcmp(buf+sizeRead-smaller, this->_m_div, smaller) == 0)){
            a.append(buf, sizeRead-smaller);

            break;
        }

        a.append(buf);
        fullCount += sizeRead;
    }

    return a;

}

bool TCPStream::validateIP(addrinfo *book) {


}

TCPStream::IPList TCPStream::getIP(addrinfo *book) {

    IPList ip = IPList();
    char ipstr[INET6_ADDRSTRLEN];
    addrinfo* p;
    for(p = book; p != NULL; p = p->ai_next){

        char* ipver;

        if(p->ai_family == AF_INET){
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            in_addr *addr = &(ipv4->sin_addr);
            ipver = "v4";
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            ip.ipv4 = addr;
            ip.IPv4 = ipstr;

        }
        else{
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            in6_addr *addr = &(ipv6->sin6_addr);
            ipver = "v6";
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            ip.ipv6 = addr;
            ip.IPv6 = ipstr;
        }

        printf(" %s: %s\n", ipver, ipstr);

    }

    return ip;
}

template<class Generic>
void TCPStream::sendSerial(int socketfd, std::unique_ptr<Generic> generic) {
    char *tmp;
    int count;
    // create new serialisation box
    std::stringstream serial;
    std::unique_ptr<cereal::PortableBinaryOutputArchive> archive(serial);
    archive(generic);
    archive.reset(nullptr);

    while(!serial.eof() || !serial.bad()){
        serial.read(tmp, 1024);
        count = serial.gcount() == 1024? 1024 : serial.gcount();
        send(socketfd, tmp, count);
    }




}

template<class Generic>
ptr<Generic> TCPStream::recieveSerial(int sockedfd) {



}

I2CStream::I2CStream(bool isController, u_int8_t I2CBus){
    int state;
    this->Controller = isController;
    this->I2CBus = I2CBus;
    gpioInitialise();

    state = gpioSetPullUpDown(0, PI_PUD_UP);
    if(state == PI_BAD_GPIO || state == PI_BAD_PUD) std::cout << "Pull Up Failed" << std::endl;
    state = gpioSetPullUpDown(1, PI_PUD_UP);
    if(state == PI_BAD_GPIO || state == PI_BAD_PUD) std::cout << "Pull Up Failed" << std::endl;
}

I2CStream::~I2CStream(){
    gpioTerminate();
};

void I2CStream::open(int address) {
    this->address = address;
}

void I2CStream::read(unsigned int reg) {
    int status;
    if(this->Controller){
        int _h = i2cOpen(this->I2CBus, this->address, 0);
        // test for the moment

        status = i2cReadWordData(_h, reg);
        if(status == PI_BAD_HANDLE || status == PI_BAD_PARAM || status == PI_I2C_READ_FAILED) {
            std::cout << "Read Failed" << std::endl;
        }

    }
    else{
        this->slave.control = this->control;
        int status = bscXfer(&this->slave);
        if(status >= 0){

        }

    }

    std::cout << status << std::endl;
}

void I2CStream::write(unsigned int reg) {
    int status;
    if(this->Controller) {
        int _h = i2cOpen(this->I2CBus, this->address, 0);
        // test f m
        status = i2cWriteWordData(_h, reg, 0xFFFF);
        if (status == PI_BAD_HANDLE || status == PI_BAD_PARAM || status == PI_I2C_WRITE_FAILED) {
            std::cout << "Write Failed" << std::endl;
        }
    }
    else{
        // TEST NOTES
        this->slave.control = this->control;
        memccpy(this->slave.txBuf, "ABCD", NULL, 4);
        this->slave.txCnt = 4;
        int status = bscXfer(&this->slave);
        if(status >= 0){

        }

    }

}

void I2CStream::controlEdit(int address, int *flags) {
/*
    Excerpt from http://abyz.me.uk/rpi/pigpio/cif.html#bscXfer regarding the control bits:

    22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    a  a  a  a  a  a  a  -  -  IT HC TF IR RE TE BK EC ES PL PH I2 SP EN

    Bits 0-13 are copied unchanged to the BSC CR register. See pages 163-165 of the Broadcom
    peripherals document for full details.

            aaaaaaa defines the I2C slave address (only relevant in I2C mode)
    IT  invert transmit status flags
    HC  enable host control
    TF  enable test FIFO
    IR  invert receive status flags
    RE  enable receive
    TE  enable transmit
    BK  abort operation and clear FIFOs
    EC  send control register as first I2C byte
    ES  send status register as first I2C byte
    PL  set SPI polarity high
    PH  set SPI phase high
    I2  enable I2C mode
    SP  enable SPI mode
    EN  enable BSC peripheral
*/
 // Flags like this: 0b/*IT:*/0/*HC:*/0/*TF:*/0/*IR:*/0/*RE:*/0/*TE:*/0/*BK:*/0/*EC:*/0/*ES:*/0/*PL:*/0/*PH:*/0/*I2:*/0/*SP:*/0/*EN:*/0;

    int f =0; int *flag_c;
    flag_c = flags;
    for(int offset = 17; offset > 0; offset++ ){
        f = (*(flag_c + (17 - offset)) << offset);
    }

    this->control = (address << 16) | f;


}
