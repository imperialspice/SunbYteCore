//
// Created by middleton on 5/27/22.
//

#ifndef SUNBYTE_GENERICS_H
#define SUNBYTE_GENERICS_H


#include <cstring>


#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/portable_binary.hpp>




class generic{
private:


public:
    std::string msg;
    std::string id;

    template<class Archive>
    void serialize(Archive & archive){};

    virtual void run(){};

    virtual void create(){};


    virtual ~generic() = default;

};

class SendI2C : public generic{




};

class SendTCP : public generic{



};

class Message : public generic{
public:
    Message(){
        id = "Message";
    }

    void run() override{}

    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, msg);
    }

};

class EXE : public generic{
public:
    EXE(){
        id = "exe";
    }

    void run() override{
        std::cout << "Execute" << std::endl;
        msg.append("executed code in server side from message");
    }

    template<class Archive>
    void serialize(Archive &archive){
        archive(id, msg);
    }


};

class Error : public generic{
public:
    Error(){
        id = "error";
    }
    Error(std::string error){
        id = "error";
        msg = std::move(error);
    }
    using generic::run;
    void run(std::string a){
        msg = std::move(a);

    }


    template<class Archive>
    void serialize(Archive &archive){
        archive(id, msg);
    }

};



//CEREAL_REGISTER_TYPE(generic);
CEREAL_REGISTER_TYPE(Message);
CEREAL_REGISTER_TYPE(EXE);
CEREAL_REGISTER_TYPE(Error);


CEREAL_REGISTER_POLYMORPHIC_RELATION(generic, Message);
CEREAL_REGISTER_POLYMORPHIC_RELATION(generic, EXE);
CEREAL_REGISTER_POLYMORPHIC_RELATION(generic, Error);

#endif //SUNBYTE_GENERICS_H