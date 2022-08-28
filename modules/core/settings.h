//
// Created by middleton on 8/23/22.
//

#ifndef SUNBYTE_SETTINGS_H
#define SUNBYTE_SETTINGS_H
//
//#include <map>
//#include <string>
//#include <vector>
//#include <exception>
//
////union tree_values{
////    int val;
////    std::string str;
////};
//
//struct tree_values{
//    std::string str;
//    int ints = 0;
//};
//
//struct tree_settings{
//    std::map<std::string, tree_settings> sub_property_tree;
//    tree_values values;
//
//    explicit tree_settings(int val){
//        sub_property_tree = {};
//        values.ints = val;
//    }
//    explicit tree_settings(std::string val){
//        sub_property_tree = {};
//        values.str = std::move(val);
//    }
//    explicit tree_settings(){
//        sub_property_tree = {};
//    }
//
//};
//
//class settings {
//    // contained within this class are the settings manipulations methods
//    std::map<std::string, tree_settings> property_tree;
//    void settings_reg(std::vector<std::string> path, tree_settings &target, tree_settings &_sub, int level, bool set);
//    void settings_reg(std::vector<std::string> vector1, tree_settings settings, tree_settings settings1, int i);
//
//public:
//    settings(){
//        property_tree = {};
//    }
//
//    void setting_register(std::string, int);
//    void setting_register(std::string, std::string);
//    tree_values get(std::string);
//    void set(std::string, tree_values);
//
//
//
//};
//
//
//// break up the path
//std::vector<std::string> break_path(std::string path){
//    std::vector<std::string> rtn;
//    bool search = true;
//    int pos = 0;
//    while(search){
//        pos = path.find("/", pos+1);
//        if(pos == -1){
//            search = false;
//            pos = path.size()-1;
//        }
//        rtn.push_back(path.substr(0, pos));
//    }
//    return rtn;
//}
//
//
//void settings::setting_register(std::string path, int value) {
//    auto vpath = break_path(path);
//    auto tmp = property_tree[vpath.at(0)];
//    tree_settings insert(value);
//    settings_reg(vpath, insert, tmp, 0);
//}
//
//void settings::setting_register(std::string path, std::string value) {
//    auto vpath = break_path(path);
//    auto tmp = property_tree[vpath.at(0)];
//
//    tree_settings insert(value);
//    settings_reg(vpath, insert, tmp, 0);
//}
//
//
//void settings::settings_reg(std::vector<std::string> path, tree_settings& target, tree_settings &_sub, int level = 0, bool set = true){
//    if(level < path.size()){
//        auto nextLevel = _sub.sub_property_tree[path[level]];
//        level = level + 1;
//        settings_reg(path, target, nextLevel, level, set);
//    }
//    else{
//        if(set){
//            _sub = std::move(target);
//        }
//        else{
//            target = _sub; // copy out
//        }
//    }
//}
//
//tree_values settings::get(std::string path) {
//    auto vpath = break_path(path);
//    auto tmp = property_tree[vpath.at(0)];
//    tree_settings rtn;
//    settings_reg(vpath, rtn, tmp, 1, false);
//    return rtn.values;
//}
//
//void settings::set(std::string path, tree_values val) {
//    auto vpath = break_path(path);
//    auto tmp = property_tree[vpath.at(0)];
//    tree_settings rtn;
//    rtn.values = std::move(val);
//    settings_reg(vpath, rtn, tmp);
//}
//
//void settings::settings_reg(std::vector<std::string> vector1, tree_settings settings, tree_settings settings1, int i) {
//    settings_reg(std::move(vector1), settings, settings1, i, true);
//}

//#include <rapidjson/rapidjson.h>
//#include <rapidjson/document.h>
//#include <rapidjson/allocators.h>
//#include <vector>
//
//class Settings{
//    rapidjson::Document document;
//    rapidjson::Value obj(rapidjson::kArrayType);
//
//
//    Settings() = default;
//    explicit Settings(const std::string& json){
//        document.Parse(json.c_str());
//    }
//
//    std::vector<std::string> break_path(std::string path);
//
//public:
//    void addSetting(std::string path, std::string value);
//    void addSetting(std::string path, int value);
//};
//
//void Settings::addSetting(std::string path, int value) {
//    auto tmp = break_path(path);
//
//    for(auto t : tmp){
//        auto itr = document.FindMember(t.c_str());
//        if(itr == document.MemberEnd()){
//            rapidjson::GenericValue<rapidjson::UTF8> name = t;
//            n
//            document.AddMember(, document.SetObject(), document.GetAllocator())
//
//
//        }
//
//    }
//
//
//}
//
//void Settings::addSetting(std::string path, std::string value) {
//
//
//}
//
//std::vector<std::string> Settings::break_path(std::string path) {
//    std::vector<std::string> rtn;
//    bool search = true;
//    int pos = 0;
//    while(search){
//        pos = path.find("/", pos+1);
//        if(pos == -1){
//            search = false;
//            pos = path.size()-1;
//        }
//        rtn.push_back(path.substr(0, pos));
//    }
//    return rtn;
//}
//

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <cstring>
#include "generics.h"



class settings_exception : std::exception{
    std::string msg;

public:
    settings_exception(char * message) : msg(message){}
    settings_exception(std::string message) : msg(std::move(message)){};

//    const char* what() {
//        return msg.c_str();
//    }


};

enum types{
    String,
    Int,
    Null

};

struct Value{
    std::shared_ptr<void> store;
    types _type;

    Value(std::shared_ptr<void> st1, types ty1){
       set(st1, ty1);
    }

    void set(std::shared_ptr<void> st1, types ty1){
        store = std::move(st1);
        _type = ty1;
    }

    std::string* getString(){
        if(_type != types::String) throw settings_exception("This setting is not a String.");
        return static_cast<std::string*>(store.get());
    }

    int* getInt(){
        if(_type != types::Int) throw settings_exception("This setting is not a Int.");
        return static_cast<int*>(store.get());
    }


};

// honestly why borther with a virtual model, just use map with keys and pretend
class Settings{
    std::map<std::string, Value> store;

public:

    Settings(){
        store = {};
    }
    const void registerSetting(std::string path, std::string value, types type){
        std::shared_ptr<void> tmp = std::make_shared<std::string>(value);
        Value val(tmp, (type));
        store.insert_or_assign(path, std::move(val));
    }

    void registerSetting(std::string path, int value, types type){
        std::shared_ptr<void> tmp = std::make_shared<int>(value);
        Value val(tmp, (type));
        store.insert_or_assign(path, std::move(val));
    }

    std::shared_ptr<void> getPointer(std::string path){
        auto it = store.find(path);
        if(it == store.end()) return std::make_shared<int>(0);
        return it->second.store;
    }

    std::string getString(std::string path){
        auto it = store.find(path);
        if(it == store.end()) return "";
        return *it->second.getString();
    }

    int getInt(std::string path){
        auto it = store.find(path);
        if(it == store.end()) return 0;
        return *it->second.getInt();
    }

    void* get(std::string path){
        auto it = store.find(path);
        if(it == store.end()) return nullptr;
        return it->second.store.get();

    }

    types getType(std::string path){
        auto it = store.find(path);
        if(it == store.end()) return Null;
        return it->second._type;
    }

    std::string set(std::string path,Value value){
        auto i = store.find(path);
        if(i == store.end()){
            store.emplace(std::pair(path, value));
            return "Inserted new value.";
        }
        else{
            if(i->second._type == value._type) // allow reassign
            {
                store.insert_or_assign(path, value);
                return "Replaced existing pair.";
            }
            else{
                return "Type Mismatch on setting.";
            }

        }
    }

};

Settings settings = {};

class msg_Settings : public generic{

    std::string path;
    std::string data;
    std::string type;



public:
    msg_Settings() {
            id = "settings";
    }

    msg_Settings(std::string lpath, std::string ltype, std::string ldatas = ""){
        id = "settings";
        path = std::move(lpath);
        data = std::move(ldatas);
        type = std::move(ltype);
        msg = "";

    }
    void run() override{
        std::shared_ptr<void> tmp;
        types t1;
        if(type == "Int"){
            tmp = std::make_shared<int>(std::stoi(data));
            t1 = Int;
        }
        else if(type == "String"){
            tmp = std::make_shared<std::string>(data);
            t1 = String;
        }
        else{
            t1 = Null;
        }
        Value val(tmp, t1);
        msg = settings.set(path, val);
    }

    template<class Archive>
    void serialize(Archive &archive){
        archive(id, path, data, type, msg);
    }
};



CEREAL_REGISTER_TYPE(msg_Settings);

CEREAL_REGISTER_POLYMORPHIC_RELATION(generic, msg_Settings);



#endif //SUNBYTE_SETTINGS_H
