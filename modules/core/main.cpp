//
// Created by midd1eton on 10/04/2022.
//
#include <iostream>
#include <cstring>

#include "external.h"
#include <fstream>




int accept_loop (int sockfd, TCPStream *_this){
    int len = 0;
    unsigned int flags =0;
    struct sockaddr *from;
    socklen_t *fromlen;
    char hold[20] = "not implemented\0";
    std::cout << "Setting up message loop" << std::endl;
    while(1) {

        std::string msg = _this->receive(sockfd);
        fprintf(stdout, "Server Copy: %s\n", msg.c_str());

        if (msg.compare("quit") == 0) {
            std::cout << "Closing Server" << std::endl;
            break;
        }
        _this->send(sockfd, hold, sizeof hold);

    }
    return 0;
}


int main(int argc, char *argv[]){


//    auto a= new TCPStream();
//    a->establish(NULL, "2223");
//    a->local(a->socketID, a->res);
//    a->accept(accept_loop);


    auto b = new TCPStream();
    b->establish("/home/middleton/test123", NULL);
    b->local(b->socketID, b->res);
    b->accept(accept_loop);

}