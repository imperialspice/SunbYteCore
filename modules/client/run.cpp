//
// Created by middleton on 6/8/22.
//

#include <memory>
#include <client.h>
#include <string>

std::string cmd;
int main(){
// Create an inital command loop for communicating with the remote clients;
    auto *term = new Terminal();

    term->establish("/home/middleton/test123", NULL);


    for(;;){
        std::cout << ">> " << std::flush;
        std::getline(std::cin, cmd);
        if(cmd.size() == 0) continue; // dont bother sending anything if it's empty.
        ptr<generic> rtn = term->serial_transceive(cmd);
        std::cout << rtn->id << "  :  " << rtn->msg << std::endl;
        if(strncmp(cmd.c_str(), "quit", 4) == 0){
            break;
        }

    }

    delete(term);

};
