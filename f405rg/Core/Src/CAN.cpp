/*
 * CAN.cpp
 *
 *  Created on: Mar 5, 2022
 *      Author: igvcsp2022
 */

#include <CAN.h>
#include <functional>
#include <cstring>
#include <algorithm>

using namespace std::placeholders;

#define MSG_IN_QUEUE 	 0x01U

CAN_HandleTypeDef hcan1, hcan2;

namespace {
std::map<CAN_HandleTypeDef*, CAN*> objectMap = std::map<CAN_HandleTypeDef*, CAN*>();
typedef CANIrqCb subscriberValue_t;
}



// HAL CALLBACK FUNCTIONS


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CanMsg msg;

	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &msg.rxHeader, msg.data) != HAL_OK) {
		Error_Handler();
	}

	std::map<CAN_HandleTypeDef*, CAN*>::iterator itr = objectMap.find(hcan);
	if(itr != objectMap.end()) itr->second->__fifo0MsgPendingIrq(msg);
}





// CanMsg CLASS IMPLEMENTATION



CanMsg::CanMsg(uint32_t ide) {
	this->txHeader.IDE = ide;
	this->rxHeader.IDE = ide;
}

CanMsg::CanMsg(uint32_t id, uint8_t data[8]) {
	this->txHeader.StdId = id;
	this->txHeader.IDE 	= CAN_ID_STD;
	this->txHeader.RTR 	= CAN_RTR_DATA;
	this->txHeader.TransmitGlobalTime = DISABLE;
	memcpy(this->data, data, sizeof(this->data));
}

CanMsg::CanMsg(uint32_t id, uint32_t ide, uint8_t data[8]) {
	this->txHeader.IDE 	= ide;
	this->rxHeader.IDE = ide;
	this->txHeader.RTR 	= CAN_RTR_DATA;
	this->txHeader.TransmitGlobalTime = DISABLE;
	memcpy(this->data, data, sizeof(this->data));

	switch(ide) {
	case CAN_ID_STD:
		this->txHeader.IDE = CAN_ID_STD;
		break;
	case CAN_ID_EXT:
		this->txHeader.IDE = CAN_ID_EXT;
		break;
	default:
		Error_Handler();
	}
}




// CAN CLASS IMPLEMENTATION



CAN::CAN(CAN_TypeDef *base, uint16_t queueSize) {
	init(base, queueSize);
	this->cb = NULL;
}

CAN::CAN(CAN_TypeDef *base, CANIrqGlobalCb cb, uint16_t queueSize) {
	init(base, queueSize);
	this->cb = cb;
}

CAN::~CAN() {
	HAL_CAN_DeactivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Stop(handle);

	objectMap.erase(handle);
}


void CAN::init(CAN_TypeDef *base, uint16_t queueSize) {
	handle = (base == CAN1) ? &hcan1 : &hcan2;

	// Check to see if we've already made a CAN object for the same hardware base
	if(objectMap.insert(std::pair<CAN_HandleTypeDef*, CAN*>(handle, this)).second == false)
		Error_Handler();


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

	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation 		= CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank 				= 0;
	canfilterconfig.FilterFIFOAssignment 	= CAN_FILTER_FIFO0;
	canfilterconfig.FilterMaskIdHigh 		= 0x0000;
	canfilterconfig.FilterMaskIdLow 		= 0x0000;
	canfilterconfig.FilterMode 				= CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale 			= CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(handle, &canfilterconfig) != HAL_OK)
		Error_Handler();

	if(base == CAN1) {
		HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0U);
		HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	}
	else {
		HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5,0U);
		HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	}

	if(HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
		Error_Handler();

	if(HAL_CAN_Start(handle) != HAL_OK)
		Error_Handler();

	this->queueSize = queueSize;
}



int CAN::send(CanMsg &msg) {
	lock();
	if(HAL_CAN_GetTxMailboxesFreeLevel(handle) == 0) {
		unlock();
		return -1;
	}

	if (HAL_CAN_AddTxMessage(handle, &msg.txHeader, msg.data, &txMailbox) != HAL_OK) {
		Error_Handler();
	}

	unlock();
	return 0;
}

int CAN::read(CanMsg &msg, uint32_t timeout) {
	lock();
	std::map<uint16_t, subscriberValue_t>::iterator itr = subscriptions.find(msg.id);
	if(itr == subscriptions.end()) {
		unlock();
		return 0;
	}

	if(queue.size() == 0) {
		unlock();
		return 0;
	}

	msg = queue.front();
	queue.pop();

	int count = queue.size();
	unlock();

	return count;
}

int CAN::subscribe(uint16_t id, CANIrqCb cb) {
	lock();
	subscriptions.insert(std::pair<uint16_t, subscriberValue_t>(id, cb));
	unlock();
	return 0;
}


bool CAN::isAvailable(uint16_t id) {
	lock();
	int count = queue.size();
	unlock();
	return (count > 0) ? true : false;
}


void CAN::__fifo0MsgPendingIrq(CanMsg &msg) {
	if(cb != NULL) cb(&msg);

	std::map<uint16_t, subscriberValue_t>::iterator itr = subscriptions.find(msg.rxHeader.StdId);
	if(itr != subscriptions.end()) {
		itr->second(msg);
		return;
	}

	queue.push(msg);
	if(queue.size() > queueSize) queue.pop();
}

void CAN::lock() {
	osMutexAcquire(mutex, osWaitForever);
}

void CAN::unlock() {
	osMutexRelease(mutex);
}
