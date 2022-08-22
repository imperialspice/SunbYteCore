//
// Created by middleton on 8/19/22.
//

#include <cstdint>
#include <spidev_lib++.h>
#include <string>

#ifndef SUNBYTE_DRV8711_H
#define SUNBYTE_DRV8711_H

// this is the driver board for the pollou motor drivers in sunbyte
// using memory mapped IO or the GPIORpi library

// hold values
void digitalWrite(int pin, int value){}


class selectChip{
    int chipPin;
public:
    selectChip(int chipPin){
        digitalWrite(chipPin, 1);
        this->chipPin = chipPin;
    }
    ~selectChip(){
        digitalWrite(this->chipPin, 0);
    }
};

class drv8711 {

    // uses the SPIDEV library because honestly its easier.
    SPI *_spi;
    int currentChipSelect = 0;
    enum register_maps : uint16_t ;
    enum struct reg_select : uint8_t;
    uint16_t buf_write;
    uint16_t buf_read;

public:
    drv8711(std::string spidev){
        _spi = new SPI(spidev.c_str());
    }

    // a hack to ensure that even without chip selects the SPI library can write
    void switchChip(int ChipSelectPin);

    drv8711(std::string spidev, spi_config_t *_config){
        _spi = new SPI(spidev.c_str(), _config);

    }
    ~drv8711(){
        delete _spi;
    }

    typedef struct {
        uint8_t mode;
        uint8_t bits_per_word;
        uint32_t speed;
        uint16_t delay;
    } spi_config_t;


    void write(int ChipSelect, reg_select reg, uint16_t value);

    uint16_t read(uint8_t ChipSelect, reg_select reg);

    // quick commands
    void enableDriver();
    void disableDriver();

    bool getDirection();

    void setDirection();

    void step(int csel);
};

enum struct drv8711::reg_select : uint8_t {
    CTRL = 0x0,
    TORQUE = 0x01,
    OFF = 0x02,
    BLANK = 0x03,
    DECAY = 0x04,
    STALL = 0x05,
    DRIVE = 0x06,
    STATUS = 0x07
};

enum drv8711::register_maps : uint16_t {

      ctrl_register = 0xC10,
/*  0, Enable, 1, R/W, 0
 *  1, Direction, 1, R/W, 0
 *  2, RSTEP, 1, W, 0
 *  3, MODE, 4, R/W, 0
 *  7, EXSTALL, 1, R/W, 0
 *  8, ISGAIN, 2, R/W, 00
 *  10, DTIME, 2, R/W, 11
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

      torque_register = 0x1FF,
/*  0, TORQUE, 8, R/W, 0xFF
 *  8, SMPLTH, 3, R/W, 001
 *  11, RESV, 1
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

      off_register = 0x030,
/*  0, TOFF, 8, R/W, 0x80
 *  8, PWMMODE, 1, R/W, 0
 *  9-11, RESV, 3
 *  12, UNUSED, 4,  - DO NOT WRITE
 */


      blank_register = 0x080,
/*  0, TBLANK, 8, R/W, 0x80
 *  8, ARBT, 1, R/W, 0
 *  9-11, RESV, 3
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

      decay_register = 0x110,
/*  0, TDECAY, 8, R/W, 0x40
 *  8, DECMOD, 3, R/W, 0
 *  11, RESV, 1
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

      stall_register = 0x040,
/*  0, SDTHR, 8, R/W, 0x40
 *  8, SDCNT, 1, R/W, 0
 *  9-11, VDIV, 3
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

      drive_register  =0xA59,
/*  0, OCPTH, 2, R/W, 01
 *  2, OCPDEG, 2, R/W, 10
 *  4, TDRIVEN, 2, R/W, 01
 *  6, TDRIVEP, 2, R/W, 01
 *  8, IDRIVEN, 2, R/W, 10
 *  10, IDRIVEP, 2, R/W, 10
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

      status_register = 0
/*  0, OTS, 1, R/W, 0
 *  1, AOCP, 1, R/W, 0
 *  2, BOCP, 1, W, 0
 *  3, APDF, 4, R/W, 0
 *  4, BPDF, 1, R/W, 0
 *  5, UVLO, 2, R/W, 00
 *  6, STD, 2, R/W, 11
 *  8, RESV, 4,
 *  12, UNUSED, 4 - DO NOT WRITE
 */

};

/* Copied from Pololu High Power Motor Driver */

/// Possible arguments to setStepMode().
enum class HPSDStepMode : uint16_t
{
    MicroStep256 = 256,
    MicroStep128 = 128,
    MicroStep64  =  64,
    MicroStep32  =  32,
    MicroStep16  =  16,
    MicroStep8   =   8,
    MicroStep4   =   4,
    MicroStep2   =   2,
    MicroStep1   =   1,
};

/// Possible arguments to setDecayMode().
enum class HPSDDecayMode : uint8_t
{
    Slow                = 0b000,
    SlowIncMixedDec     = 0b001,
    Fast                = 0b010,
    Mixed               = 0b011,
    SlowIncAutoMixedDec = 0b100,
    AutoMixed           = 0b101,
};

/// Bits that are set in the return value of readStatus() to indicate status
/// conditions.
///
/// See the DRV8711 datasheet for detailed descriptions of these status
/// conditions.
enum class HPSDStatusBit : uint8_t
{
    /// Overtemperature shutdown
    OTS = 0,

    /// Channel A overcurrent shutdown
    AOCP = 1,

    /// Channel B overcurrent shutdown
    BOCP = 2,

    /// Channel A predriver fault
    APDF = 3,

    /// Channel B predriver fault
    BPDF = 4,

    /// Undervoltage lockout
    UVLO = 5,

    /// Stall detected
    STD = 6,

    /// Latched stall detect
    STDLAT = 7,
};


#endif //SUNBYTE_DRV8711_H
