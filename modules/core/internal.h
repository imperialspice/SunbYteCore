//
// Created by midd1eton on 10/04/2022.
//

#ifndef SUNBYTE_INTERNAL_H
#define SUNBYTE_INTERNAL_H

#include "iostream"


// uses IPC sockets for communication
// creating var spools

class internal {

};


class Generic{
public:

    virtual void run(){
        std::cout << "Should never be seen" << std::endl;
    }

};

class SendI2C : public Generic{




};

class SendTCP : public Generic{



};

#endif //SUNBYTE_INTERNAL_H
