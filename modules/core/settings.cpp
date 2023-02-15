//
// Created by middleton on 8/23/22.
//

#include "settings.h"

Settings settings = {};

msg_Settings::msg_Settings(std::string lpath, std::string ltype, std::string ldatas = ""){
    id = "settings";
    path = std::move(lpath);
    data = std::move(ldatas);
    type = std::move(ltype);
    msg = "";

}
msg_Settings::msg_Settings() {
    id = "settings";
}

void msg_Settings::run() {
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
        else if(type == "Bool"){
            bool m;
            if(data == "True") {m = true;
                tmp = std::make_shared<bool>(m);
                t1 = Bool;
            }
            else if(data == "False") {m = false;
                tmp = std::make_shared<bool>(m);
                t1 = Bool;
            }
            else t1 = Null;
        }
        else{
            t1 = Null;
        }
        Value val(tmp, t1);
        msg = settings.set(path, val);
};


CEREAL_REGISTER_TYPE(msg_Settings);

CEREAL_REGISTER_POLYMORPHIC_RELATION(generic, msg_Settings);
