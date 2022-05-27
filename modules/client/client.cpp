//
// Created by middleton on 4/26/22.
//

#include "client.h"
#include <cstring>

std::string cmd;


void Terminal::establish(std::string address, const char * port) {
    this->client = new TCPStream();
    this->client->establish(address.c_str(), port);
    this->client->remote(this->client->socketID, this->client->res);
    this->client_fd = this->client->socketID;
    fprintf(stdout, "Connection Established\n");
}

std::string Terminal::transceive(std::string cmd) {

    std::string rtn;


    this->client->send(this->client_fd, cmd.data(), cmd.size());
    // should be looped to collect all data if smaller than size of buffer.


    return this->client->receive(this->client_fd);
}

int main(){
// Create an inital command loop for communicating with the remote clients;
    auto term = new Terminal();

    term->establish("/home/middleton/test123", NULL);


    for(;;){
        std::cout << ">> " << std::flush;
        std::getline(std::cin, cmd);

        std::string rtn = term->transceive(cmd);
        std::cout <<  rtn << std::endl;
        if(strncmp(cmd.c_str(), "quit", 4) == 0){
            break;
        }

    }

    free(term);

};

