/*
 * CAN.cpp
 *
 *  Created on: Mar 5, 2022
 *      Author: igvcsp2022
 */

#include "CAN.h"

#include "usbd_cdc_if.h"
#include "cmsis_os.h"

#include <cstring>
#include <algorithm>


std::map<CAN_HandleTypeDef*, CAN*> CAN::objectMap = std::map<CAN_HandleTypeDef*, CAN*>();


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	std::map<CAN_HandleTypeDef*, CAN*>::iterator itr = CAN::objectMap.find(hcan);
	if(itr != CAN::objectMap.end()) itr->second->__fifo0MsgPendingIrq();
}




CAN::CAN(CAN_HandleTypeDef *handle, CAN_TypeDef *base, uint16_t id, uint16_t queueSize, GPIO_TypeDef* ledPort, uint16_t ledPin) : led(ledPort, ledPin){
	// TODO Auto-generated constructor stub
	this->handle = handle;

	handle->Instance 				= base;
	handle->Init.Prescaler = 6;
	handle->Init.Mode = CAN_MODE_NORMAL;
	handle->Init.SyncJumpWidth = CAN_SJW_2TQ;
	handle->Init.TimeSeg1 = CAN_BS1_2TQ;
	handle->Init.TimeSeg2 = CAN_BS2_2TQ;
	handle->Init.TimeTriggeredMode = DISABLE;
	handle->Init.AutoBusOff = DISABLE;
	handle->Init.AutoWakeUp = DISABLE;
	handle->Init.AutoRetransmission = ENABLE;
	handle->Init.ReceiveFifoLocked = DISABLE;
	handle->Init.TransmitFifoPriority = DISABLE;

	if (HAL_CAN_Init(handle) != HAL_OK)
		Error_Handler();

	txHeader.IDE 	= CAN_ID_STD;
	txHeader.StdId 	= id;
	txHeader.RTR 	= CAN_RTR_DATA;
	txHeader.TransmitGlobalTime = DISABLE;

	canfilterconfig.FilterActivation 		= CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank 				= 0;
	canfilterconfig.FilterFIFOAssignment 	= CAN_FILTER_FIFO0;
	canfilterconfig.FilterMaskIdHigh 		= 0x0000;
	canfilterconfig.FilterMaskIdLow 		= 0x0000;
	canfilterconfig.FilterMode 				= CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale 			= CAN_FILTERSCALE_32BIT;

	HAL_CAN_ConfigFilter(handle, &canfilterconfig);


	if(HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
		Error_Handler();

	if(HAL_CAN_Start(handle) != HAL_OK)
		Error_Handler();

	objectMap.insert(std::pair<CAN_HandleTypeDef*, CAN*>(handle, this));

	this->queueSize = queueSize;
}

CAN::~CAN() {
	HAL_CAN_DeactivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Stop(handle);

	objectMap.erase(handle);
}


int CAN::send(uint8_t *dataArray, uint8_t numBytes) {
	txHeader.DLC = numBytes;

	if(HAL_CAN_GetTxMailboxesFreeLevel(handle) == 0)
		return -1;

	if (HAL_CAN_AddTxMessage(handle, &txHeader, dataArray, &txMailbox) != HAL_OK)
		Error_Handler();

	return 0;
}

CanMsg CAN::read() {
	DigitalOut led2(LD2_GPIO_Port, LD2_Pin);

	while(rxBuffer.empty()) osDelay(1);

	CanMsg msg = rxBuffer.front();
	rxBuffer.pop();

	return msg;
}

bool CAN::isAvailable() {
	return !rxBuffer.empty();
}


void CAN::__fifo0MsgPendingIrq() {
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxData[8];

	if(HAL_CAN_GetRxMessage(handle, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK) {
		Error_Handler();
	}

	if(rxHeader.StdId == 0x411) {
		led = !led;
	}

	CanMsg canMsg;
	canMsg.header = rxHeader;

	std::memcpy(canMsg.data, rxData, sizeof(rxData));

	if(rxBuffer.size() >= queueSize) rxBuffer.pop();
	rxBuffer.push(canMsg);


	CDC_Transmit_FS(rxData, strlen((char*)rxData));
}



void CAN::Error_Handler(void) {
	__disable_irq();
	while(1)
	{

	}
}
