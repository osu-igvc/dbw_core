/*
 * memory.h
 *
 *  Created on: Jan 23, 2022
 *      Author: 14055
 */

#ifndef PARAM_SERVER_H_
#define PARAM_SERVER_H_


#include "eeprom.h"
#include "stm32f4xx_hal_flash_ex.h"

class ParamServer {
public:
	ParamServer(uint16_t ADDRESS, uint16_t V_Address);
	virtual ~ParamServer();
	void write(uint16_t Write_Data);
	uint16_t read(uint16_t V_Address);


private:
	uint16_t PAGE_BASE_ADDRESS;
	uint16_t V_Address;
	uint16_t Write_Data;
	uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
	uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
	uint16_t VarValue,VarDataTmp = 0;
};
#endif /* MEMORY_H_ */
