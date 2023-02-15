//
// Created by middleton on 8/29/22.
//


// This file will control automatic downlink systems, eg downloading temp and other status keeping data from the probe.
#define NOMESSAGELIST

#include "client.h"
#include "messages.h"
#include <filesystem> // I think this is the only c++17 use
#include <date.h>
#include <cmath>
#include <fstream>


//std::vector<std::string> break_path(const std::string& path) {
//    std::vector<std::string> rtn;
//    bool search = true;
//    int pos = 0;
//    int oldpos = 0;
//    while (search) {
//        pos = path.find(" ", oldpos);
//        if(pos == -1) {
//            rtn.emplace_back(path.substr(oldpos, rtn.size() - oldpos));
//            break;
//        }
//        rtn.emplace_back(path.substr(oldpos, pos-oldpos));
//        oldpos = pos+1;
//    }
//    return rtn;
//}
//
//



int main( int argc, char* argv[]) {
    // downlink core info from data cores

    if(argc < 3){ std::cout << "Please format the command as follows: \nfile | folder ( app | over ) <Command>"; return(-1);}
    bool fileexists = std::filesystem::exists(argv[1]);
    bool isDir = std::filesystem::is_directory(argv[1]); // true - dir | false - file
    bool overWrite = false;
    if(!fileexists) {std::cout << "File or Folder does not exist" << std::endl; return(-1);}
    if(strcmp(argv[2], "over") == 0){
       overWrite = true;
    }
    else if(strcmp(argv[2], "app") == 0){
        overWrite = false;
    }
    else{
        std::cout << "Please format the command as follows: \nfile | folder ( app | over ) <Command>\n"; return(-1);
    }


    // create a series of terminals which ask for data at set intervals
    auto *term = new Terminal();
    term->establish("172.16.18.151", "42069");
    std::string cmd;
    for(int i = 3; i < argc; i++){
        cmd = cmd + std::string(argv[i]) + std::string(" ");
    }
    if(cmd.empty()) {std::cout << "Command line empty."; return(-1);}

    // clock time for file names

    std::string name_template(argv[1]);
    if(isDir){ // enter dir mode, create files with system date and command name
        name_template = name_template.find("/", name_template.size()-1)==-1 ? name_template.append("/"):name_template;
        name_template = name_template+std::string(argv[3]);
        for(;;){
            _ptr<generic> rtn = term->serial_transceive(cmd);
            std::cout << rtn->id << "  :  " << rtn->msg << std::endl;

            std::ofstream outfile(name_template+date::format("%F %T", std::chrono::system_clock::now()),std::ofstream::binary|std::ios_base::app);
            outfile.write(rtn->msg.c_str(), rtn->msg.size());

            if(rtn->id=="message"){
                auto dump = dynamic_cast<msg_Messages*>(rtn.get())->dump_data();
                outfile.write(dump.data(), dump.size());
            }
            outfile.close();
        }
    }
    else{
        for(;;){
            _ptr<generic> rtn = term->serial_transceive(cmd);
            std::cout << rtn->id << "  :  " << rtn->msg << std::endl;
            std::ofstream outfile;
            if(overWrite){
                outfile.open(name_template,std::ofstream::binary);
            }
            else{
                outfile.open(name_template,std::ofstream::binary|std::ios_base::app);
            }

            outfile.write(rtn->msg.c_str(), rtn->msg.size());

            auto dump = dynamic_cast<msg_Messages*>(rtn.get())->dump_data();
            if(rtn->id=="message"){
                outfile.write(dump.data(), dump.size());
            }
            outfile.close();
        }


    }

    delete(term);

//    for(;;){
//        _ptr<generic> rtn = term->serial_transceive(cmd);
//        std::cout << rtn->id << "  :  " << rtn->msg << std::endl;
//
//        time = std::chrono::system_clock::now();
//
//    }
//
//    delete(term);
//
//


}