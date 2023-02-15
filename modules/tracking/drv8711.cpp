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

void drv8711::enableDriver() {
    registerMaps.ctrl_register |= (1 << 0);
    write(shadowSet, reg_select::CTRL, registerMaps.ctrl_register);

}

void drv8711::disableDriver() {
    registerMaps.ctrl_register &= ~(1 << 0);
    write(shadowSet, reg_select::CTRL, registerMaps.ctrl_register);
}

/*
 *  direction: true for forward, false for backward
 *
 */
void drv8711::setDirection(bool direction = false){
    if(direction){
        registerMaps.ctrl_register  |= (1 << 1);
    }
    else{
        registerMaps.ctrl_register &= ~(1 << 1);
    }
    write(shadowSet, reg_select::CTRL, registerMaps.ctrl_register);
}

bool drv8711::getDirection(){
    return registerMaps.ctrl_register >> 1 & 1;
}

void drv8711::step(){
    this->write(shadowSet, reg_select::CTRL, registerMaps.ctrl_register | (1 << 2));
}

void drv8711::setStepMode( HPSDStepMode step = HPSDStepMode::MicroStep4){
    registerMaps.ctrl_register =
            (registerMaps.ctrl_register & 0b111110000111) | (((uint8_t) step) << 3);

    this->write(shadowSet, reg_select::CTRL, registerMaps.ctrl_register);
}

void drv8711::setTorque( uint16_t current_limit){
    current_limit = (current_limit > 8000) ? 8000:current_limit;
    uint8_t isgainBits = 0b11;
    uint16_t torqueBits = ((uint32_t)768  * current_limit) / 6875;

    // Halve the gain and TORQUE until the TORQUE value fits in 8 bits.
    while (torqueBits > 0xFF)
    {
        isgainBits--;
        torqueBits >>= 1;
    }

    registerMaps.ctrl_register =
            (registerMaps.ctrl_register & 0b110011111111) | (isgainBits << 8);
    registerMaps.torque_register =
            (registerMaps.torque_register & 0b111100000000) | torqueBits;

    write(shadowSet, reg_select::CTRL, registerMaps.ctrl_register);
    write(shadowSet, reg_select::TORQUE, registerMaps.torque_register);
}

void drv8711::setDecayMode(HPSDDecayMode mode){
    registerMaps.decay_register =
            (registerMaps.decay_register & 0b00011111111) | (((uint8_t)mode & 0b111) << 8);
    write(shadowSet, reg_select::DECAY, registerMaps.decay_register);
}

HPSDStatusBit drv8711::getStatus(){
    uint16_t rd = read(shadowSet, reg_select::STATUS);
    registerMaps.status_register = rd;
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

HPSDStatusBit drv8711::getFaults(){
    return getStatus();
}

void drv8711::clearFaults(){
    registerMaps.status_register = ~0b00111111;
    write(shadowSet, reg_select::STATUS, registerMaps.status_register);
}
void drv8711::clearStatus(){
    registerMaps.status_register = 0;
    write(shadowSet, reg_select::STATUS, registerMaps.status_register);
}

void drv8711::applySettings(){
    write(shadowSet, reg_select::CTRL, registerMaps.ctrl_register);
    write(shadowSet, reg_select::STATUS, registerMaps.status_register);
    write(shadowSet, reg_select::DECAY, registerMaps.decay_register);
    write(shadowSet, reg_select::TORQUE, registerMaps.torque_register);
    write(shadowSet, reg_select::BLANK, registerMaps.blank_register);
    write(shadowSet, reg_select::DRIVE, registerMaps.drive_register);
    write(shadowSet, reg_select::OFF, registerMaps.off_register);
    write(shadowSet, reg_select::STALL, registerMaps.stall_register);
}

bool drv8711::verifySettings(){
    return
                    read(shadowSet, reg_select::CTRL) == registerMaps.ctrl_register &&
                    read(shadowSet, reg_select::TORQUE) == registerMaps.torque_register &&
                    read(shadowSet, reg_select::OFF) == registerMaps.off_register &&
                    read(shadowSet, reg_select::BLANK) == registerMaps.blank_register &&
                    read(shadowSet, reg_select::DECAY) == registerMaps.decay_register &&
                    read(shadowSet, reg_select::STALL) == registerMaps.stall_register &&
                    read(shadowSet, reg_select::DRIVE) == registerMaps.drive_register;

}

void drv8711::reset( ){
    defaultValues();
    applySettings();
}