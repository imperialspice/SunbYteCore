//
// Created by middleton on 8/26/22.
//

#include "messages.h"


Messages messageList = {};

void msg_Messages::run(){
    if(name.empty()) {msg = "Empty Name Error"; return;}
    auto mess = messageList.get(name);
    data.insert(data.end(), mess.data.begin(), mess.data.end());
    msg = "gotten returned data: ";
    msg = msg + std::string(data.begin(), data.end());
}