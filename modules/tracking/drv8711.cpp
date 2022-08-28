//
// Created by middleton on 8/19/22.
//

#include "drv8711.h"
#include <iostream>


void drv8711::write(int ChipSelect, drv8711::reg_select reg, uint16_t value) {
    selectChip chip(ChipSelect); // auto close on destruction
    uint16_t trans = ((((uint8_t) reg) & 0b111) << 12) | (((uint16_t)value) & 0xFFFF);
    std::cout << "Write: " << std::to_string(trans) << std::endl;
    _spi->write(reinterpret_cast<uint8_t *>(trans), 16);
}

uint16_t drv8711::read(uint8_t ChipSelect, drv8711::reg_select reg) {
    selectChip chip(ChipSelect);
    uint16_t recv =0;
    uint16_t tran = (0x8 | (((uint8_t)reg) & 0b111)) << 12;
    std::cout << "Read: " << std::to_string(tran) << std::endl;
    _spi->xfer(reinterpret_cast<uint8_t *>(tran), 16, reinterpret_cast<uint8_t *>(recv), 16);
    return recv;
}

void drv8711::enableDriver(int csel = 0) {
    registerMaps.at(csel).ctrl_register |= (1 << 0);
    write(csel, reg_select::CTRL, registerMaps.at(csel).ctrl_register);

}

void drv8711::disableDriver(int csel = 0) {
    registerMaps.at(csel).ctrl_register &= ~(1 << 0);
    write(csel, reg_select::CTRL, registerMaps.at(csel).ctrl_register);
}

/*
 *  direction: true for forward, false for backward
 *
 */
void drv8711::setDirection(int csel = 0, bool direction = false){
    if(direction){
        registerMaps.at(csel).ctrl_register  |= (1 << 1);
    }
    else{
        registerMaps.at(csel).ctrl_register &= ~(1 << 1);
    }
    write(csel, reg_select::CTRL, registerMaps.at(csel).ctrl_register);
}

bool drv8711::getDirection(int csel = 0){
    return registerMaps.at(csel).ctrl_register >> 1 & 1;
}

void drv8711::step(int csel = 0){
    this->write(csel, reg_select::CTRL, registerMaps.at(csel).ctrl_register | (1 << 2));
}

void drv8711::setStepMode(int csel = 0, HPSDStepMode step = HPSDStepMode::MicroStep4){
    registerMaps.at(csel).ctrl_register =
            (registerMaps.at(csel).ctrl_register & 0b111110000111) | (((uint8_t) step) << 3);

    this->write(csel, reg_select::CTRL, registerMaps.at(0).ctrl_register);
}

void drv8711::setTorque(int csel, uint16_t current_limit){
    current_limit = (current_limit > 8000) ? 8000:current_limit;
    uint8_t isgainBits = 0b11;
    uint16_t torqueBits = ((uint32_t)768  * current_limit) / 6875;

    // Halve the gain and TORQUE until the TORQUE value fits in 8 bits.
    while (torqueBits > 0xFF)
    {
        isgainBits--;
        torqueBits >>= 1;
    }

    registerMaps.at(csel).ctrl_register =
            (registerMaps.at(csel).ctrl_register & 0b110011111111) | (isgainBits << 8);
    registerMaps.at(csel).torque_register =
            (registerMaps.at(csel).torque_register & 0b111100000000) | torqueBits;

    write(csel, reg_select::CTRL, registerMaps.at(csel).ctrl_register);
    write(csel, reg_select::TORQUE, registerMaps.at(csel).torque_register);
}

void drv8711::setDecayMode(int csel, HPSDDecayMode mode){
    registerMaps.at(csel).decay_register =
            (registerMaps.at(csel).decay_register & 0b00011111111) | (((uint8_t)mode & 0b111) << 8);
    write(csel, reg_select::DECAY, registerMaps.at(csel).decay_register);
}

HPSDStatusBit drv8711::getStatus(int csel){
    uint16_t rd = read(csel, reg_select::STATUS);
    registerMaps.at(csel).status_register = rd;
    HPSDStatusBit tn;
    tn.OTS = rd >> 0;
    tn.AOCP = rd >> 1;
    tn.BOCP = rd >> 2;
    tn.APDF = rd >> 3;
    tn.BPDF = rd >> 4;
    tn.UVLO = rd >> 5;
    tn.STD = rd >> 6;
    tn.STDLAT = rd >> 7;
    return tn;
}

HPSDStatusBit drv8711::getFaults(int csel){
    return getStatus(csel);
}

void drv8711::clearFaults(int csel){
    registerMaps.at(csel).status_register = ~0b00111111;
    write(csel, reg_select::STATUS, registerMaps.at(csel).status_register);
}
void drv8711::clearStatus(int csel){
    registerMaps.at(csel).status_register = 0;
    write(csel, reg_select::STATUS, registerMaps.at(csel).status_register);
}

void drv8711::applySettings(int csel){
    write(csel, reg_select::CTRL, registerMaps.at(csel).ctrl_register);
    write(csel, reg_select::STATUS, registerMaps.at(csel).status_register);
    write(csel, reg_select::DECAY, registerMaps.at(csel).decay_register);
    write(csel, reg_select::TORQUE, registerMaps.at(csel).torque_register);
    write(csel, reg_select::BLANK, registerMaps.at(csel).blank_register);
    write(csel, reg_select::DRIVE, registerMaps.at(csel).drive_register);
    write(csel, reg_select::OFF, registerMaps.at(csel).off_register);
    write(csel, reg_select::STALL, registerMaps.at(csel).stall_register);
}

bool drv8711::verifySettings(int csel){
    return
                    read(csel, reg_select::CTRL) == registerMaps.at(csel).ctrl_register &&
                    read(csel, reg_select::TORQUE) == registerMaps.at(csel).torque_register &&
                    read(csel, reg_select::OFF) == registerMaps.at(csel).off_register &&
                    read(csel, reg_select::BLANK) == registerMaps.at(csel).blank_register &&
                    read(csel, reg_select::DECAY) == registerMaps.at(csel).decay_register &&
                    read(csel, reg_select::STALL) == registerMaps.at(csel).stall_register &&
                    read(csel, reg_select::DRIVE) == registerMaps.at(csel).drive_register;

}

void drv8711::reset(int csel =0 ){
    defaultValues(csel);
    applySettings(csel);
}