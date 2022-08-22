//
// Created by middleton on 6/8/22.
//
/*
#include "external_i2c.h"


void I2CStream::write(unsigned int reg) {
    int status;
    if(this->Controller) {
        int _h = i2cOpen(this->I2CBus, this->address, 0);
        // test f m
        status = i2cWriteWordData(_h, reg, 0xFFFF);
        if (status == PI_BAD_HANDLE || status == PI_BAD_PARAM || status == PI_I2C_WRITE_FAILED) {
            std::cout << "Write Failed" << std::endl;
        }
    }
    else{
        // TEST NOTES
        this->slave.control = this->control;
        memccpy(this->slave.txBuf, "ABCD", 0, 4);
        this->slave.txCnt = 4;
        int status = bscXfer(&this->slave);
        if(status >= 0){

        }

    }

}
    void I2CStream::controlEdit(int address, int *flags) {
*/

/*
    Excerpt from http://abyz.me.uk/rpi/pigpio/cif.html#bscXfer regarding the control bits:

    22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    a  a  a  a  a  a  a  -  -  IT HC TF IR RE TE BK EC ES PL PH I2 SP EN

    Bits 0-13 are copied unchanged to the BSC CR register. See pages 163-165 of the Broadcom
    peripherals document for full details.

            aaaaaaa defines the I2C slave address (only relevant in I2C mode)
    IT  invert transmit status flags
    HC  enable host control
    TF  enable test FIFO
    IR  invert receive status flags
    RE  enable receive
    TE  enable transmit
    BK  abort operation and clear FIFOs
    EC  send control register as first I2C byte
    ES  send status register as first I2C byte
    PL  set SPI polarity high
    PH  set SPI phase high
    I2  enable I2C mode
    SP  enable SPI mode
    EN  enable BSC peripheral
*/
    // Flags like this: 0b/*IT:*/0/*HC:*/0/*TF:*/0/*IR:*/0/*RE:*/0/*TE:*/0/*BK:*/0/*EC:*/0/*ES:*/0/*PL:*/0/*PH:*/0/*I2:*/0/*SP:*/0/*EN:*/0;
/*
    int f =0; int *flag_c;
    flag_c = flags;
    for(int offset = 17; offset > 0; offset++ ){
        f = (*(flag_c + (17 - offset)) << offset);
    }

    this->control = (address << 16) | f;


}


// I2C Streams


I2CStream::I2CStream(bool isController, u_int8_t I2CBus){
    int state;
    this->Controller = isController;
    this->I2CBus = I2CBus;
    gpioInitialise();

    state = gpioSetPullUpDown(0, PI_PUD_UP);
    if(state == PI_BAD_GPIO || state == PI_BAD_PUD) std::cout << "Pull Up Failed" << std::endl;
    state = gpioSetPullUpDown(1, PI_PUD_UP);
    if(state == PI_BAD_GPIO || state == PI_BAD_PUD) std::cout << "Pull Up Failed" << std::endl;
}

I2CStream::~I2CStream(){
    gpioTerminate();
};

void I2CStream::open(int address) {
    this->address = address;
}

void I2CStream::read(unsigned int reg) {
    int status;
    if(this->Controller){
        int _h = i2cOpen(this->I2CBus, this->address, 0);
        // test for the moment

        status = i2cReadWordData(_h, reg);
        if(status == PI_BAD_HANDLE || status == PI_BAD_PARAM || status == PI_I2C_READ_FAILED) {
            std::cout << "Read Failed" << std::endl;
        }

    }
    else{
        this->slave.control = this->control;
        status = bscXfer(&this->slave);
        if(status >= 0){

        }

    }

    std::cout << status << std::endl;
}
*/