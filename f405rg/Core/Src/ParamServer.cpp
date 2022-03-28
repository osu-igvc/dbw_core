/*
 * memory.cpp
 *
 *  Created on: Jan 23, 2022
 *      Author: 14055
 */

#include <functional>

#include <ParamServer.h>
#include "main.h"

using namespace std::placeholders;

ParamServer::ParamServer(const char *name, uint16_t ADDRESS, uint16_t V_Address) {
	(ADDRESS == 0) ? (PAGE_BASE_ADDRESS == PAGE0_BASE_ADDRESS) : (PAGE_BASE_ADDRESS == PAGE1_BASE_ADDRESS);

	this-> V_Address = V_Address;

	const osMutexAttr_t mutexAttr = {
			"ParamServerMutex",
			osMutexRecursive,
			NULL,
			0U
	};
	mutex = osMutexNew(&mutexAttr);

	//VirtAddVarTab[V_Address]
	//this-> Write_Data = Write_Data;
	//write(Write_Data);
}

ParamServer::~ParamServer() {
	osMutexDelete(mutex);
}


void ParamServer::write(uint16_t Write_Data){
	osMutexAcquire(mutex, osWaitForever);

	if((EE_WriteVariable(VirtAddVarTab[V_Address], Write_Data)) != HAL_OK) {
		osMutexRelease(mutex);
		Error_Handler();
	}

	if((EE_ReadVariable(VirtAddVarTab[V_Address],  &VarDataTab[V_Address])) != HAL_OK) {
		osMutexRelease(mutex);
		Error_Handler();
	}

	if (Write_Data != VarDataTab[V_Address]){
		osMutexRelease(mutex);
		Error_Handler();
	}

}

uint16_t ParamServer::read(uint16_t V_Address){
	if((EE_ReadVariable(VirtAddVarTab[V_Address],  &VarDataTab[V_Address])) != HAL_OK)
		Error_Handler();

	else
		return VarDataTab[V_Address];

	return 0;
}
