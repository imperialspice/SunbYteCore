//
// Created by middleton on 8/26/22.
//

#ifndef SUNBYTE_MESSAGES_H
#define SUNBYTE_MESSAGES_H

#include <generics.h>
#include <memory>
#include <string>
#include <vector>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <map>
// external message class, can request information though message command


struct message{
    std::string type;
    int id;
    std::vector<char> data;
};


class Messages {

    std::map<std::string, message> data_list;
public:
    Messages(){

    }

    message msg_string(std::string t){
        message msg;        // vector + vector copy quite fast
        std::vector<char> data_overlay(&t[0],&t[t.size()]);
        msg.data.insert(msg.data.end(), data_overlay.begin(), data_overlay.end());
        return msg;
    }

    message msg_cv(cv::Mat mat){
        message msg;
        std::stringstream osout(std::ios::binary|std::ios::in|std::ios::out);
        osout << mat;
        char tmp[1000];
        int count = 0;

        while(osout.good()){
            osout.read(tmp, 1000);
            count = osout.gcount() == 1000 ? 1000 : osout.gcount();
            for(int i = 0; i < count; i++){
                msg.data.push_back(tmp[i]);
            }
        }
        return msg;
    }


    message get(std::string id){
        auto it = data_list.find(id);
        if(it == data_list.end()) return message();
        return it->second;
    }

    void set(std::string id, message msg){
        data_list.insert_or_assign(id, msg);
    }

    void set(std::string id, std::string msg){
        auto i = msg_string(msg);
        i.type = "string";
        data_list.insert_or_assign(id, i);
    }

    void set(std::string id, cv::Mat mat){
        auto i = msg_cv(mat);
        i.type = "opecvmat";
        data_list.insert_or_assign(id, i);
    }

    static cv::Mat returnType(std::vector<char> data){
        cv::Mat rtn;
        std::stringstream str;
        str.write(data.data(), data.size());

    }

};

Messages messageList = {};

class msg_Messages : public generic{
    std::string name;

    std::vector<char> data;


public:

    msg_Messages(){
      id = "message";
    }
    explicit msg_Messages(std::string namel){
        name = namel;
    }

    void run() override{
        if(name.empty()) {msg = ""; return;}
        auto mess = messageList.get(name);
        data.insert(data.end(), mess.data.begin(), mess.data.end());
        msg = "gotten returned data: ";
        msg = msg + std::string(data.begin(), data.end());

    }


    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, name, data);
    }


};

CEREAL_REGISTER_TYPE(msg_Messages);

CEREAL_REGISTER_POLYMORPHIC_RELATION(generic, msg_Messages);

#endif //SUNBYTE_MESSAGES_H
