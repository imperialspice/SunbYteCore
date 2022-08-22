//
// Created by middleton on 8/19/22.
//

#include "drv8711.h"




void drv8711::write(int ChipSelect, drv8711::reg_select reg, uint16_t value) {
    selectChip chip(ChipSelect); // auto close on destruction
    uint16_t trans = ((((uint8_t) reg) & 0b111) << 12) | (((uint16_t)value) & 0xFFFF);
    _spi->write(reinterpret_cast<uint8_t *>(trans), 16);
}

uint16_t drv8711::read(uint8_t ChipSelect, drv8711::reg_select reg) {
    selectChip chip(ChipSelect);
    uint16_t recv =0;
    uint16_t tran = (0x8 | (((uint8_t)reg) & 0b111)) << 12;
    _spi->xfer(reinterpret_cast<uint8_t *>(tran), 16, reinterpret_cast<uint8_t *>(recv), 16);
    return recv;
}

void drv8711::enableDriver() {

}

void drv8711::disableDriver() {

}

void drv8711::setDirection(){

}

bool drv8711::getDirection(){
    return register_maps::ctrl_register >> 1 & 1;
}

void drv8711::step(int csel){
    this->write(csel, reg_select::CTRL, register_maps::ctrl_register | (1 << 2));
}