/*
 * i2cEEPROM.h
 *
 *  Created on: Mar 29, 2022
 *      Author: Tyler Tucker
 */

#ifndef i2cEEPROM_H_
#define i2cEEPROM_H_

#define ADDR

#include "stm32f4xx_hal.h"


class i2cEEPROM {
    public:
        void sendData();
        void sendByte();
        uint_8 receiveByte();
    private:
};

#endif
