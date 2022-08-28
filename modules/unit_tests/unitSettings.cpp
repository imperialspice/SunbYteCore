//
// Created by middleton on 8/23/22.
//

#include "settings.h"
#include <iostream>


int main() {

    Settings tmp;
    tmp.registerSetting("hello/world", "booyaa", types::String);
    std::cout << tmp.getType("hello/world") << std::endl;
    std::cout << tmp.getString("hello/world") << std::endl;
//    std::cout << tmp.getInt("hello/world") << std::endl;
    auto pt4 = tmp.getPointer("hello/world");
    auto pt1 = tmp.getPointer("hello/world").get();
    std::cout << pt1 << std::endl;
    Value val(std::shared_ptr<void>(std::make_shared<int>(100)), Int);
    tmp.set("hello/world", val);
    auto pt3 = tmp.getPointer("hello/world");
    auto pt2 = tmp.getPointer("hello/world").get();
    std::cout << pt1 << " " << pt2 << std::endl;



}