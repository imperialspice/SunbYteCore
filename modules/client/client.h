//
// Created by middleton on 4/26/22.
//

#ifndef SUNBYTE_CLIENT_H


#define SUNBYTE_CLIENT_H

#endif //SUNBYTE_CLIENT_H

#include <iostream>
#include <string>
#include <external.h>
#include <internal.h>



class Terminal {

    std::string cmd;
    TCPStream* client;
    int client_fd;

public:
    void establish(std::string address, const char * port);
    std::string transceive(std::string cmd);

    _ptr<generic> serial_transceive(std::string cmd);

    ~Terminal(){
        free(client);
    }


};

