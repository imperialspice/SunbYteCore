//
// Created by middleton on 6/8/22.
//

#include <external.h>
#include <client.h>

int messageLimt = 10;

std::string genRandom(){
    std::string result;
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    int length = rand() % 1024;
    result.reserve(length);
    std::cout << "Size: " << length << " A Size: " << result.length() << std::endl;
    for(int i=0; i< length; i++){
        result.push_back(charset.at(rand() % charset.size()));
    }
    return result;
}

void create(){
    auto term = new Terminal();
    term->establish("/home/middleton/test123", NULL);
    for(int i = 0; i < messageLimt;i++){
        std::string cmd;
        cmd = genRandom();
        if(cmd.size() == 0) continue; // dont bother sending anything if it's empty.
        ptr<generic> rtn = term->serial_transceive(cmd);
        std::cout << rtn->msg << std::endl;
        if(strncmp(cmd.c_str(), "quit", 4) == 0){
            break;
        }
    }
    free(term);
}



int main(int argc, char* argv[]){
    int number = 100;

    std::vector<std::unique_ptr<std::thread>> l;
    srand(time(nullptr));
    for(int i=0; i< number; i++){
        std::unique_ptr<std::thread> a = std::make_unique<std::thread>(create);
        l.push_back(std::move(a));
    }
    while(true){
        std::string a;
        std::getline(std::cin, a);
        if(a.empty()) {
            break;
        }
    }
}