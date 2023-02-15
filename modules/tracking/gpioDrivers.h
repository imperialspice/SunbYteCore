//
// Created by middleton on 9/2/22.
//

#ifndef SUNBYTE_GPIODRIVERS_H
#define SUNBYTE_GPIODRIVERS_H

#include <gpiod.hpp>
#include <cstdio>
#include <chrono>
#include <thread>
#include <generics.h>
#include <iostream>
#include <libudev.h>

class gpioDrivers {
    std::string name;
    gpiod_chip *chip;
    gpiod_line *step_line;
    gpiod_line *dir;
    gpiod_line *enable_line;
    bool isEnabled = false;

public:
    gpioDrivers() = default;

    gpioDrivers(std::string Inname, int chipNumber, int stepInt, int direction, int enableInt){
#ifdef DEBUG
        name = Inname;
        chip = gpiod_chip_open_by_number(chipNumber);
        if(chip == nullptr) return;
        // open the lines
        step_line = gpiod_chip_get_line(chip, stepInt);
        dir = gpiod_chip_get_line(chip, direction);
        enable_line = gpiod_chip_get_line(chip,enableInt);
        // move to output
        gpiod_line_request_output(step_line, name.c_str(), 0);
        gpiod_line_request_output(dir, name.c_str(), 0);
        gpiod_line_request_output(enable_line, name.c_str(), 1);
#endif
    }


    void step(){
#ifdef DEBUG
        gpiod_line_set_value(step_line, 1);
        std::this_thread::sleep_for(std::chrono::microseconds(20));
        gpiod_line_set_value(step_line, 0);
#endif
    }


    void step(int numberOfSteps){
#ifdef DEBUG
        explicitEnable();
        for(int i = 0; i<numberOfSteps; i++){
            step();
        }
        explicitDisable();
#endif
    }


    void setDirection(bool Direction){
#ifdef DEBUG
        if(Direction){
            gpiod_line_set_value(dir, 0);
        }
        else{
            gpiod_line_set_value(dir, 1);
        }
#endif
    }
    void enable(){
#ifdef DEBUG
        if(isEnabled){
            gpiod_line_set_value(enable_line, 1);
            isEnabled = false;
        }
        else{
            gpiod_line_set_value(enable_line, 0);
            isEnabled = true;
        }
#endif
    }

    void explicitEnable(){
#ifdef DEBUG
        gpiod_line_set_value(enable_line, 0);
        isEnabled = true;
#endif
    }


    void explicitDisable(){
#ifdef DEBUG
        gpiod_line_set_value(enable_line, 1);
        isEnabled = false;
#endif
    }

    ~gpioDrivers(){
#ifdef DEBUG
        gpiod_line_release(step_line);
        gpiod_line_release(dir);
        gpiod_line_release(enable_line);
        gpiod_chip_close(chip);
#endif
    }

};
//
class drivers_message : public generic {
    // acts on settings data, allows a setting to be set with the desired motor, direction and step and will act on that
    gpioDrivers m1 = gpioDrivers("m1", 3, 28, 24, 25);
    gpioDrivers m2 = gpioDrivers("m2", 3, 29, 27, 26);

public:

    drivers_message();

    void run() override;

};

//CEREAL_REGISTER_TYPE(drivers_message);
//
//CEREAL_REGISTER_POLYMORPHIC_RELATION(generic, drivers_message);
//

#endif //SUNBYTE_GPIODRIVERS_H
