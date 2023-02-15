//
// Created by middleton on 02/09/22.
//
#include "settings.h"
#include "gpioDrivers.h"

extern Settings settings;


inline void setTurns(int steps, char * msg, char direction){
    std::string tmp = std::to_string(steps);
    memcpy(msg, tmp.c_str(), 4);
    char * msg_dir = msg+4;
    memcpy(msg_dir, (char*)direction, 1);
}



drivers_message::drivers_message(){
    settings.registerSettings("drivers/motor1", 0, Int);
    settings.registerSettings("drivers/motor2", 0, Int);
}
void drivers_message::run(){
    // get data from the settings
    int motor1 = settings.getInt("drivers/motor1");
    int motor2 = settings.getInt("drivers/motor2");
    if(motor1 != 0 && motor2 != 0) return;

    motor1 > 0 ? m1.setDirection(false):m1.setDirection(true);
    motor2 > 0 ? m2.setDirection(false):m2.setDirection(true);
    if(motor1 != 0 ){
        m1.explicitEnable();
        m1.step(abs(motor1));
        m1.explicitDisable();
    }

    if(motor2 != 0){
        m2.explicitEnable();
        m2.step(abs(motor2));
        m2.explicitDisable();
    }

}