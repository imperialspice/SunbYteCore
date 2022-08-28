//
// Created by midd1eton on 10/04/2022.
//

#ifndef SUNBYTE_INTERNAL_H
#define SUNBYTE_INTERNAL_H

#include "iostream"
#include <deque>
#include <mutex>
#include <condition_variable>
#include "generics.h"
#include <cereal/cereal.hpp>

// uses IPC sockets for communication
// creating var spools



struct generic_handle{
    std::shared_ptr<generic> msg; // this has to be a shared pointer for taskAdd(); if not then switch to references here, byt that seems riskier.
    std::unique_ptr<std::condition_variable> flag;
    std::unique_ptr<std::mutex> lock;
    int id;
};

struct task_handle{
    std::shared_ptr<generic> _generic_handle; // this is the owner of internal generics
    int task_occurance;
};


class internal {
private:
    std::unique_ptr<std::deque<std::shared_ptr<generic_handle>>> list = {};
    std::mutex mutex = {};
    std::condition_variable condition = {};
//    int currentRead = 0;
    int v = 0;

public:
    internal(){
        list = std::make_unique<std::deque<std::shared_ptr<generic_handle>>>();
    }

    // thread safe addition
    std::shared_ptr<generic_handle> append(std::shared_ptr<generic> &unit){
        std::shared_ptr<generic_handle> copy;
        {
            std::lock_guard<std::mutex> lock(mutex);
            v++;
            std::shared_ptr<generic_handle> a(new generic_handle({unit, std::make_unique<std::condition_variable>(), std::make_unique<std::mutex>(), v}));
            list->push_back(std::move(a));
            copy = list->back();
        }
        condition.notify_one();
        return copy;
    }


    // comsumption(){}
    void consume(){
        std::unique_lock<std::mutex> lock{mutex};
        while(list->empty()){
            condition.wait(lock);
        }
        std::shared_ptr<generic_handle> tmp = std::move(list->front());
        list->pop_front();
        tmp.reset();
    }

    int size(){
        return list->size();
    }

    std::shared_ptr<generic_handle> getShared(){
        // I think get shared acts as though its running consume on the objects in the server.
        std::unique_lock<std::mutex> lock{mutex};
        while(list->empty()){
            condition.wait(lock);
        }
        std::shared_ptr<generic_handle> tmp = list->front();
        list->pop_front();
        return tmp;
    }
};




#endif //SUNBYTE_INTERNAL_H
