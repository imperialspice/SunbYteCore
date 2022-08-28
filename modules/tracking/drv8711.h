//
// Created by middleton on 8/19/22.
//

#include <cstdint>
#include <spidev_lib++.h>
#include <string>
#include <vector>

#ifndef SUNBYTE_DRV8711_H
#define SUNBYTE_DRV8711_H

// this is the driver board for the pollou motor drivers in sunbyte
// using memory mapped IO or the GPIORpi library


/// Possible arguments to setStepMode().
enum class HPSDStepMode : uint16_t
{
    MicroStep256 =   0b1000,
    MicroStep128 =   0b0111,
    MicroStep64  =   0b0110,
    MicroStep32  =   0b0101,
    MicroStep16  =   0b0100,
    MicroStep8   =   0b0011,
    MicroStep4   =   0b0010,
    MicroStep2   =   0b0001,
    MicroStep1   =   0b0000,
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
struct HPSDStatusBit
{
    /// Overtemperature shutdown
    bool OTS = false;

    /// Channel A overcurrent shutdown
    bool AOCP = false;

    /// Channel B overcurrent shutdown
    bool BOCP = false;

    /// Channel A predriver fault
    bool APDF = false;

    /// Channel B predriver fault
    bool BPDF = false;

    /// Undervoltage lockout
    bool UVLO = false;

    /// Stall detected
    bool STD = false;

    /// Latched stall detect
    bool STDLAT = false;
};



// hold values
//void digitalWrite(int pin, int value){
//    // do nothing.
//}


class selectChip{
    int chipPin;
public:
    selectChip(int chipPin){
//        digitalWrite(chipPin, 1);
        this->chipPin = chipPin;
    }
    ~selectChip(){
//        digitalWrite(this->chipPin, 0);
    }
};

class drv8711 {

    // uses the SPIDEV library because honestly its easier.
    SPI *_spi;

/* Copied from Pololu High Power Motor Driver */

    struct reg_maps {

        uint16_t ctrl_register ;
/*  0, Enable, 1, R/W, 0
 *  1, Direction, 1, R/W, 0
 *  2, RSTEP, 1, W, 0
 *  3, MODE, 4, R/W, 0
 *  7, EXSTALL, 1, R/W, 0
 *  8, ISGAIN, 2, R/W, 00
 *  10, DTIME, 2, R/W, 11
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

        uint16_t torque_register  ;
/*  0, TORQUE, 8, R/W, 0xFF
 *  8, SMPLTH, 3, R/W, 001
 *  11, RESV, 1
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

        uint16_t off_register  ;
/*  0, TOFF, 8, R/W, 0x80
 *  8, PWMMODE, 1, R/W, 0
 *  9-11, RESV, 3
 *  12, UNUSED, 4,  - DO NOT WRITE
 */


        uint16_t blank_register  ;
/*  0, TBLANK, 8, R/W, 0x80
 *  8, ARBT, 1, R/W, 0
 *  9-11, RESV, 3
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

        uint16_t decay_register  ;
/*  0, TDECAY, 8, R/W, 0x40
 *  8, DECMOD, 3, R/W, 0
 *  11, RESV, 1
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

        uint16_t stall_register  ;
/*  0, SDTHR, 8, R/W, 0x40
 *  8, SDCNT, 1, R/W, 0
 *  9-11, VDIV, 3
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

        uint16_t drive_register  ;
/*  0, OCPTH, 2, R/W, 01
 *  2, OCPDEG, 2, R/W, 10
 *  4, TDRIVEN, 2, R/W, 01
 *  6, TDRIVEP, 2, R/W, 01
 *  8, IDRIVEN, 2, R/W, 10
 *  10, IDRIVEP, 2, R/W, 10
 *  12, UNUSED, 4,  - DO NOT WRITE
 */

        uint16_t status_register  ;
/*  0, OTS, 1, R/W, 0
 *  1, AOCP, 1, R/W, 0
 *  2, BOCP, 1, W, 0
 *  3, APDF, 1, R/W, 0
 *  4, BPDF, 1, R/W, 0
 *  5, UVLO, 1, R/W, 00
 *  6, STD, 1, R/W, 11,
 *  7, STDLAT, 1, R/W, 0
 *  8, RESV, 4,
 *  12, UNUSED, 4 - DO NOT WRITE
 */

    };

    std::vector<reg_maps> registerMaps = {};
    enum struct reg_select : uint8_t;
    uint16_t buf_write = 0;
    uint16_t buf_read = 0;

    reg_maps defaultValues(){
        return reg_maps({0xC10, 0x1FF, 0x030, 0x080, 0x110, 0x040, 0xA59, 0});
    }

    void defaultValues(int csel){
        registerMaps.at(csel) = {0xC10, 0x1FF, 0x030, 0x080, 0x110, 0x040, 0xA59, 0};
    }

public:
    drv8711(const std::string& spidev, int numberOfChips){
        _spi = new SPI(spidev.c_str());
        for(int i = 0;  i < numberOfChips ; i++ ){
            registerMaps.push_back(defaultValues());
        }

    }
// {0xC10, 0x1FF, 0x030, 0x080 ,0x110, 0x040, 0xA59, 0, 0}
    // a hack to ensure that even without chip selects the SPI library can write
    //void switchChip(int ChipSelectPin);

    drv8711(const std::string& spidev, spi_config_t *_config){
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


    void step(int csel);

    bool getDirection(int csel);

    void setDirection(int csel, bool direction);

    void disableDriver(int csel);

    void enableDriver(int csel);

    void setStepMode(int csel, HPSDStepMode step);

    void setTorque(int csel, uint16_t current_limit);

    void setDecayMode(int csel, HPSDDecayMode mode);

    HPSDStatusBit getStatus(int csel);

    HPSDStatusBit getFaults(int csel);

    void clearFaults(int csel);

    void clearStatus(int csel);

    void applySettings(int csel);

    bool verifySettings(int csel);

    void reset(int csel);

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



#endif //SUNBYTE_DRV8711_H
