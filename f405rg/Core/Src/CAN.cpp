/*
 * CAN.cpp
 *
 *  Created on: Mar 5, 2022
 *      Author: igvcsp2022
 */

#include "CAN.h"

#include <cstring>
#include <algorithm>

CAN_HandleTypeDef hcan1, hcan2;

std::unordered_map<CAN_HandleTypeDef*, CAN*> CAN::objectMap = std::unordered_map<CAN_HandleTypeDef*, CAN*>();


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	std::unordered_map<CAN_HandleTypeDef*, CAN*>::iterator itr = CAN::objectMap.find(hcan);
	if(itr != CAN::objectMap.end()) itr->second->__fifo0MsgPendingIrq();
}



CAN::CAN(CAN_TypeDef *base, uint16_t queueSize) :
		rxBuffer() {
	init(base, queueSize);
	this->cb = NULL;
}

CAN::CAN(CAN_TypeDef *base, CANIRQCb cb, uint16_t queueSize) :
		rxBuffer() {
	init(base, queueSize);
	this->cb = cb;
}

CAN::~CAN() {
	HAL_CAN_DeactivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Stop(handle);

	while(!rxBuffer.empty()) {
		rxBuffer.pop();
	}

	objectMap.erase(handle);
}


void CAN::init(CAN_TypeDef *base, uint16_t queueSize) {
	this->handle = (base == CAN1) ? &hcan1 : &hcan2;

	handle->Instance = base;
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


int CAN::send(uint16_t id, uint8_t dataArray[8]) {
	txHeader.DLC = 8;
	txHeader.StdId 	= id;

	if(HAL_CAN_GetTxMailboxesFreeLevel(handle) == 0)
		return -1;

	if (HAL_CAN_AddTxMessage(handle, &txHeader, dataArray, &txMailbox) != HAL_OK)
		Error_Handler();

	return 0;
}

int CAN::read(CanMsg *msg) {
	if(rxBuffer.empty()) return -1;

	*msg = rxBuffer.front();
	rxBuffer.pop();

	return rxBuffer.size();
}

void CAN::subscribe(uint16_t id, CANIRQCb cb) {
	subscriptions.insert(std::pair<uint16_t, CANIRQCb>(id, cb));
	return;
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

	if(rxBuffer.size() >= queueSize) {
		rxBuffer.pop();
	}

	CanMsg msg = {.header=rxHeader};
	memcpy(msg.data, rxData, sizeof(msg.data));
	rxBuffer.push(msg);

	if(cb != NULL) cb(&msg);

	std::unordered_map<uint16_t, CANIRQCb>::iterator itr = subscriptions.find(rxHeader.StdId);
	if(itr != subscriptions.end()) itr->second(&msg);
}
