/*
 * i2cEEPROM.cpp
 *
 *  Created on: Mar 29, 2022
 *      Author: Tyler Tucker
 */

#include <i2cEEPROM.h>
#include <algorithm>

static uint_8 lastReceive[64];

i2cEEPROM::i2cEEPROM(){
    TM_I2C_Init(I2C3, TM_I2C_PinsPack_1, 50000);
}

i2cEEPROM::~i2cEEPROM(){}


void sendData(uint_8 &data[], uint_8 highAddr, uint_8 lowAddr){
    uint_8 arr[1 + sizeof(data)];
    arr[0] = lowAddr;
    std::copy(data, data + sizeof(data), arr + 1);
    HAL_I2C_Master_Transmit();  
}

void receiveData(){
    uint_8 arr[1 + sizeof(data)];
    arr[0] = lowAddr;
    std::copy(data, data + sizeof(data), arr + 1);
    TM_I2C_WriteMulti(I2C3, ADDR_WRITE, highAddr, arr, sizeof(arr));
}
