//
// Created by middleton on 4/23/22.
//

#ifndef SUNBYTE_SENSORS_H
#define SUNBYTE_SENSORS_H

#include "generics.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ina219.h"
#include "bno055.h"

#include <nmea/nmea.h>


/* We have three i2c sensors, one SPI and one serial on the sensor board, so we need code that communicates with all of those.
 * Read loops would be useful for this too, no inter process messaging.
 *
 *
 *
 */


class sensors {

    INA219 currentSensor;
    int i2cfd;







};


class sensors_module : public sensors, public generic{

public:
    sensors_module(){

    }

    void run (){



    }




};

#endif //SUNBYTE_SENSORS_H
