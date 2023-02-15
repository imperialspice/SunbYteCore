//
// Created by middleton on 8/25/22.
//

#include "motorDrive_UDP.h"
#include "iostream"
#include "thread"
#include <chrono>
#include <cstring>


int total_x_steps = 0;
int total_y_steps = 0;
int x_stop = 32000;
int y_stop=8000;



void move(int stepx, int stepy){

    motorDrive_UDP* motors = new motorDrive_UDP();

    // X AND Y NEED TO BE SWAPPED FOR CUrreNT MOTOR CONF
    int t = stepx;
    stepx = stepy;
    stepy = t;

    std::cout << "Relative position CMD" << std::endl;
    std::cout << "X: " << stepx << " Y: " << stepy << std::endl;
    if(stepx == 0 && stepy == 0) return;
    if(total_x_steps+stepx > x_stop) stepx = x_stop - total_x_steps;
    if(total_y_steps+stepx > y_stop) stepx = y_stop - total_y_steps;
    // construct the serial message to the ard

    std::string dirx = stepx > 0 ? "1":"0";
    std::string diry = stepy > 0 ? "1":"0";
    std::string zero(std::to_string(0));


    // ensure is abs
    stepx = abs(stepx);
    stepy = abs(stepy);

    // take the int and ensure that any overflow is handled
    bool overflowEnabled = false;
    int overflow_x;
    int overflow_y ;
    if(stepx > 9999){
        overflow_x = abs(stepx) - 9999;
        stepx = 9999;
        overflowEnabled = true;
    }
    if(stepy > 9999){
        overflow_y = abs(stepy) - 9999;
        stepy = 9999;
        overflowEnabled = true;
    }
    if(!overflowEnabled){
        int stepxDir = stepx > 0 ? 1:-1;
        int stepyDir = stepy > 0 ? 1:-1;
        stepx = abs(stepx);
        stepy = abs(stepy);
        std::string sstepx = std::to_string(stepx);
        std::string sstepy = std::to_string(stepy);


        while(sstepx.size() < 4){
            sstepx.insert(0, zero);
        }
        while(sstepy.size() < 4){
            sstepy.insert(0, zero);
        }


        std::string motorCMD;
        motorCMD.append(sstepx);
        motorCMD.append(dirx);
        motorCMD.append(sstepy);
        motorCMD.append(diry);

        std::cout << motorCMD << std::endl;


        total_x_steps  = total_x_steps + (stepx*stepxDir);
        total_y_steps = total_y_steps + (stepy*stepyDir);


    }
    else{

    while(overflowEnabled){

        int stepxDir = stepx > 0 ? 1:-1;
        int stepyDir = stepy > 0 ? 1:-1;
        stepx = abs(stepx);
        stepy = abs(stepy);
        std::string sstepx = std::to_string(stepx);
        std::string sstepy = std::to_string(stepy);


        while(sstepx.size() < 4){
            sstepx.insert(0, zero);
        }
        while(sstepy.size() < 4){
            sstepy.insert(0, zero);
        }


        std::string motorCMD;
        motorCMD.append(sstepx);
        motorCMD.append(dirx);
        motorCMD.append(sstepy);
        motorCMD.append(diry);

        std::cout << motorCMD << std::endl;
        motors->send(motorCMD);

        total_x_steps  = total_x_steps + (stepx*stepxDir);
        total_y_steps = total_y_steps + (stepy*stepyDir);


        if(overflow_x>0){
            stepx = overflow_x > 9999 ? 9999:overflow_x;
            stepx = stepx*stepxDir;
            overflow_x = overflow_x-9999>0 ? overflow_x-9999:0;
        }else{
            stepx = 0;
        }
        if(overflow_y>0){
            stepy = overflow_y > 9999 ? 9999:overflow_y;
            stepy = stepy*stepyDir;
            overflow_y = overflow_y-9999>0 ? overflow_y-9999:0;
        }
        else{
            stepy =0;
        }

        if((overflow_y==0&&stepy==0) && (overflow_x==0&&stepx==0)){
            overflowEnabled = false;
        }
    }
    }
}



int main(){
    std::string sx;
    std::string sy;
    //gpioDrivers m1("m1", 3, 28, 24, 25);
    for(;;){
        std::cout << "Current telescope position:\nX: " << std::to_string(total_x_steps) << " | Y: " << std::to_string(total_y_steps) << "\n" << std::flush;

        std::cout << "Rotation Position: " << std::flush;
        std::getline(std::cin, sx);
        std::cout << "Pitch Position: " << std::flush;
        std::getline(std::cin, sy);
        system("clear");

        if(sx.size() == 0 || sy.size() == 0) continue; // dont bother sending anything if it's empty.
        move(std::stoi(sx), std::stoi(sy));

    }
}