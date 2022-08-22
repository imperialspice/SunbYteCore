//
// Created by middleton on 6/8/22.
//
/*
#ifndef SUNBYTE_EXTERNAL_I2C_H
#define SUNBYTE_EXTERNAL_I2C_H

#include <string>
#include <pigpio.h>
#include "i2c/smbus.h"
#include "linux/i2c-dev.h"
#include <iostream>
#include <memory>
#include <cstring>

//#define 	BSC_FIFO_SIZE   16


class I2CStream{
    u_int8_t I2CBus;
    bool Controller; // IE. Master Mode
    char TxD[BSC_FIFO_SIZE];
    char RxD[BSC_FIFO_SIZE];
    unsigned int address;
    bsc_xfer_t slave;
    int control;
    int flags[14];
    void controlEdit(int address, int *flags);

    void open(int address);

    void read(unsigned int reg);
    void write(unsigned int reg);

    I2CStream(bool isController, u_int8_t I2CBus);
    ~I2CStream();


};

#endif //SUNBYTE_EXTERNAL_I2C_H
*/