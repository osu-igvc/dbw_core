/*
 * i2cEEPROM.cpp
 *
 *  Created on: Mar 29, 2022
 *      Author: Tyler Tucker
 */

#include <i2cEEPROM.h>

i2cEEPROM::i2cEEPROM(){
    TM_I2C_Init(I2C3, TM_I2C_PinsPack_1, 50000);
}

i2cEEPROM::~i2cEEPROM(){}
