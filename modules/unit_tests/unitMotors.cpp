//
// Created by middleton on 8/25/22.
//

#include "drv8711.h"
#include "iostream"

int main(){
    drv8711 test("/dev/spidev0.0", 1);
    std::cout << test.verifySettings(0) << std::endl;
    test.enableDriver(0);
    test.getStatus(0);
    test.step(0);


    drv8711 test2("/dev/spidev0.1", 1);
    std::cout << test.verifySettings(0) << std::endl;
    test2.enableDriver(0);
    test2.getStatus(0);
    test2.step(0);




}