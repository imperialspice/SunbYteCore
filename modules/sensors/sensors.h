//
// Created by middleton on 4/23/22.
//

#ifndef SUNBYTE_SENSORS_H
#define SUNBYTE_SENSORS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <spidev_lib++.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>


/* We have three i2c sensors, one SPI and one serial on the sensor board, so we need code that communicates with all of those.
 * Read loops would be useful for this too, no inter process messaging.
 *
 *
 *
 */


class sensors {

};


#endif //SUNBYTE_SENSORS_H
