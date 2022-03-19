/*
 * memory.cpp
 *
 *  Created on: Jan 23, 2022
 *      Author: 14055
 */

#include <ParamServer.h>
#include "main.h"



ParamServer::ParamServer(uint16_t ADDRESS, uint16_t V_Address) {
	(ADDRESS == 0) ? (PAGE_BASE_ADDRESS == PAGE0_BASE_ADDRESS) : (PAGE_BASE_ADDRESS == PAGE1_BASE_ADDRESS);

	this-> V_Address = V_Address; //VirtAddVarTab[V_Address]
	//this-> Write_Data = Write_Data;
	//write(Write_Data);
}

ParamServer::~ParamServer() {
	// TODO Auto-generated destructor stub
}

void ParamServer::write(uint16_t Write_Data){


		if((EE_WriteVariable(VirtAddVarTab[V_Address], Write_Data)) != HAL_OK){
			Error_Handler();
		}

		if((EE_ReadVariable(VirtAddVarTab[V_Address],  &VarDataTab[V_Address])) != HAL_OK){
			Error_Handler();
		}

		if (Write_Data != VarDataTab[V_Address]){
			Error_Handler();
		}

}

uint16_t ParamServer::read(uint16_t V_Address){


		if((EE_ReadVariable(VirtAddVarTab[V_Address],  &VarDataTab[V_Address])) != HAL_OK){
			Error_Handler();
		}
		else return VarDataTab[V_Address];

	return 0;
}
