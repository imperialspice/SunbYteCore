//
// Created by middleton on 9/3/22.
//

#ifndef SUNBYTE_MOTORDRIVE_UDP_H
#define SUNBYTE_MOTORDRIVE_UDP_H

#include "simpleNET.h"
#include <iostream>

class motorDrive_UDP {
    UDPClient *client;
    bool enabled = false;
public:
    motorDrive_UDP(){
        client = new UDPClient(42069, "172.16.18.152");
    }

    void send(std::string msg){
        client->send_message(msg);
    }

    ~motorDrive_UDP(){
        free(client);
    }
};


#endif //SUNBYTE_MOTORDRIVE_UDP_H
