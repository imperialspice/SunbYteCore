//
// Created by middleton on 4/26/22.
//

#include "client.h"




void Terminal::establish(std::string address, const char * port) {
    this->client = new TCPStream();
    this->client->establish(address.c_str(), port);
    this->client->remote(this->client->socketID, this->client->res);
    this->client_fd = this->client->socketID;
    fprintf(stdout, "Connection Established\n");
}

std::string Terminal::transceive(std::string cmd) {

    std::string rtn;


    this->client->send(this->client_fd, cmd.data(), cmd.size(), true);
    // should be looped to collect all data if smaller than size of buffer.


    return this->client->receive(this->client_fd);
}

ptr<generic> Terminal::serial_transceive(std::string cmd){
    if(cmd.find("exe") > 0){
        std::unique_ptr<generic> msg = std::make_unique<EXE>();
        msg->msg = std::move(cmd);
        this->client->sendSerial(this->client_fd, msg);
        return this->client->receiveSerial(this->client_fd);
    }
    else{
        std::unique_ptr<generic> msg = std::make_unique<Message>();
        msg->msg = std::move(cmd);
        this->client->sendSerial(this->client_fd, msg);
        return this->client->receiveSerial(this->client_fd);
    }
    }

