//
// Created by middleton on 4/26/22.
//

#include "client.h"
#include "settings.h"
#include "messages.h"



std::vector<std::string> break_path(const std::string& path) {
    std::vector<std::string> rtn;
    bool search = true;
    int pos = 0;
    int oldpos = 0;
    while (search) {
        pos = path.find(" ", oldpos);
        if(pos == -1) {
            rtn.emplace_back(path.substr(oldpos, rtn.size() - oldpos));
            break;
        }
        rtn.emplace_back(path.substr(oldpos, pos-oldpos));
        oldpos = pos+1;
    }
    return rtn;
}



void Terminal::establish(std::string address, const char * port) {
    this->client = new TCPStream();
    this->client->establish(address.c_str(), port);
    this->client->remote(this->client->socketID, this->client->res);
    this->client_fd = this->client->socketID;
    fprintf(stdout, "Connection Established\n");
}

std::string Terminal::transceive(std::string cmd) {

    std::string rtn;


    TCPStream::send(this->client_fd, cmd.data(), cmd.size(), true);
    // should be looped to collect all data if smaller than size of buffer.


    return TCPStream::receive(this->client_fd);
}

_ptr<generic> Terminal::serial_transceive(std::string cmd){
    if(cmd.find("exe") != -1){
        _ptr<generic> msg = std::make_unique<EXE>();
        msg->msg = std::move(cmd);
        TCPStream::sendSerial(this->client_fd, msg);
        return TCPStream::receiveSerial(this->client_fd);
    }
    else if(cmd.find("settings") != -1){
        // subdivide msg
        auto tmp = break_path(cmd);
        // command should look like
        // settings path value
        if(tmp.size() == 4){
            _ptr<generic> msg = std::make_unique<msg_Settings>(tmp[1], tmp[2], tmp[3]);
            TCPStream::sendSerial(this->client_fd, msg);
            return TCPStream::receiveSerial(this->client_fd);
        }
    }
    else if(cmd.find("message") != -1){
        auto tmp = break_path(cmd);
        _ptr<generic> msg(std::make_shared<msg_Messages>(tmp[1]));
        TCPStream::sendSerial(this->client_fd, msg);
        return TCPStream::receiveSerial(this->client_fd);



    }
    else{
        std::cout << "Please format the command as:\n settings path type value." << std::endl;
        return _ptr<generic>(std::make_shared<Error>("Please format the command as:\n settings path type value."));
    }

//    else{
//        _ptr<generic> msg = std::make_unique<Message>();
//        msg->msg = std::move(cmd);
//        TCPStream::sendSerial(this->client_fd, msg);
//        return TCPStream::receiveSerial(this->client_fd);
//    }
////
    }

