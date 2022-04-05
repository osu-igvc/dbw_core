/*
 * i2cEEPROM.h
 *
 *  Created on: Mar 29, 2022
 *      Author: Tyler Tucker
 */

#ifndef i2cEEPROM_H_
#define i2cEEPROM_H_

#define ADDR_READ 0b1010001
#define ADDR_WRITE 0b1010000

#include "stm32f4xx_hal.h"


class i2cEEPROM {
    public:
        void sendData();
        int * receiveData();
};

#endif
