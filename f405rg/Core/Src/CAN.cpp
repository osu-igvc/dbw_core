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

extern "C" {
CAN_HandleTypeDef hcan1, hcan2;
}

namespace {
std::map<CAN_HandleTypeDef*, CAN*> objectMap = std::map<CAN_HandleTypeDef*, CAN*>();
}



//
///////////////////////////////////////
// HAL CALLBACK FUNCTIONS
//
//

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CanMsg msg;

	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &msg.rxHeader, msg.data) != HAL_OK) {
		Error_Handler();
	}

	if(msg.rxHeader.IDE == CAN_ID_STD)
		msg.id = msg.rxHeader.StdId;
	else
		msg.id = msg.rxHeader.ExtId;

	std::map<CAN_HandleTypeDef*, CAN*>::iterator itr = objectMap.find(hcan);
	if(itr != objectMap.end()) itr->second->__fifo0MsgPendingIrq(msg);
}



//
///////////////////////////////////////
// CanMsg CLASS IMPLEMENTATION
//
//

CanMsg::CanMsg() {
	this->rxHeader.IDE = CAN_ID_STD;
	this->txHeader.IDE = CAN_ID_STD;
	this->txHeader.DLC = 8;
	this->txHeader.TransmitGlobalTime = DISABLE;
	this->txHeader.RTR = CAN_RTR_DATA;
}

CanMsg::CanMsg(uint32_t ide) {
	this->rxHeader.IDE = ide;
	this->txHeader.IDE = ide;
	this->txHeader.DLC = 8;
	this->txHeader.RTR = CAN_RTR_DATA;
}

CanMsg::CanMsg(uint32_t id, uint8_t data[8]) {
	this->txHeader.StdId = id;
	this->txHeader.IDE 	= CAN_ID_STD;
	this->txHeader.RTR 	= CAN_RTR_DATA;
	this->txHeader.TransmitGlobalTime = DISABLE;
	this->txHeader.DLC = 8;
	memcpy(this->data, data, sizeof(this->data));
}

CanMsg::CanMsg(uint32_t id, uint32_t ide, uint8_t data[8]) {
	this->txHeader.IDE 	= ide;
	this->rxHeader.IDE = ide;
	this->txHeader.RTR 	= CAN_RTR_DATA;
	this->txHeader.TransmitGlobalTime = DISABLE;
	this->txHeader.DLC = 8;

	if(data != NULL)
		memcpy(this->data, data, sizeof(this->data));

	switch(ide) {
	case CAN_ID_STD:
		this->txHeader.StdId = id;
		break;
	case CAN_ID_EXT:
		this->txHeader.ExtId = id;
		break;
	default:
		Error_Handler();
	}
}

uint8_t* CanMsg::toArray() {
	return data;
}


//
///////////////////////////////////////
// J1939Msg CLASS IMPLEMENTATION
//
//

J1939Msg::J1939Msg() : CanMsg() {};

J1939Msg::J1939Msg(uint32_t id, uint8_t data[8]) : CanMsg(id, CAN_ID_EXT, data) {
	fillHeader(id);
}

J1939Msg::J1939Msg(CanMsg &msg) : CanMsg(msg.id, CAN_ID_EXT, msg.data) {
	fillHeader(msg.id);
}

void J1939Msg::fillHeader(uint32_t id) {
	priority 	= id >> 26;
	reserved 	= id >> 25;
	dataPage 	= id >> 24;
	pduFormat 	= id >> 16;
	pduSpec 	= id >> 8;
	sourceAddr 	= id;
}

uint64_t J1939Msg::getDatum(uint8_t data[8]) {
	uint64_t datum = 0;
	for(int i=0; i<8; ++i) {
		uint64_t dat = (uint64_t)data[i] << 8*(7-i);
		datum = datum | dat;
	}
	return datum;
}


void J1939Msg::setData(uint64_t datum) {
	memcpy(data, &datum, 8*sizeof(uint8_t));
}


//
///////////////////////////////////////
// FB3TorqueMsg CLASS IMPLEMENTATION
//
//

FB3TorqueMsg::FB3TorqueMsg() : J1939Msg() {}

FB3TorqueMsg::FB3TorqueMsg(uint8_t data[8]) : J1939Msg(ID_FB3TORQUE, data) {
	fillMsg(data);
}

FB3TorqueMsg::FB3TorqueMsg(CanMsg &msg) : J1939Msg(msg) {
	fillMsg(msg.data);
}

FB3TorqueMsg::FB3TorqueMsg(uint32_t inputTorque, uint32_t cmdTorque) : J1939Msg(ID_FB3TORQUE, toArray(inputTorque, cmdTorque)) {
	this->inputTorque = inputTorque;
	this->cmdTorque = cmdTorque;
	toArray(inputTorque, cmdTorque);
}

void FB3TorqueMsg::fillMsg(uint8_t data[8]) {
	uint64_t datum = getDatum(data);

	inputTorque = datum >> 32;
	cmdTorque   = datum;
}

uint8_t* FB3TorqueMsg::toArray(uint32_t inputTorque, uint32_t cmdTorque) {
	uint64_t datum = ((uint64_t)inputTorque << 32) | cmdTorque;
	setData(datum);
	return data;
}


//
///////////////////////////////////////
// FB7PosVelMsg CLASS IMPLEMENTAITON
//
//

FB7PosVelMsg::FB7PosVelMsg() : J1939Msg() {}

FB7PosVelMsg::FB7PosVelMsg(uint8_t data[8]) : J1939Msg(ID_FB7POSVEL, data) {
	fillMsg(data);
}

FB7PosVelMsg::FB7PosVelMsg(CanMsg &msg) : J1939Msg(msg) {
	fillMsg(msg.data);
}

FB7PosVelMsg::FB7PosVelMsg(uint32_t position, uint32_t velocity) : J1939Msg(ID_FB7POSVEL, toArray(position, velocity)) {
	this->position = position;
	this->velocity = velocity;
}

void FB7PosVelMsg::fillMsg(uint8_t data[8]) {
	uint64_t datum = getDatum(data);

	position = datum >> 32;
	velocity = datum;
}

uint8_t* FB7PosVelMsg::toArray(uint32_t position, uint32_t velocity) {
	uint64_t datum = ((uint64_t)position << 32) | velocity;
	setData(datum);
	return data;
}



//
///////////////////////////////////////
// FB9StateMsg CLASS IMPLEMENTATION
//
//

FB9StateMsg::FB9StateMsg() : J1939Msg() {}

FB9StateMsg::FB9StateMsg(uint8_t data[8]) : J1939Msg(ID_FB9STATE, data) {
	fillMsg(data);
}

FB9StateMsg::FB9StateMsg(CanMsg &msg) : J1939Msg(msg) {
	fillMsg(msg.data);
}

void FB9StateMsg::fillMsg(uint8_t data[8]) {
	uint64_t datum = getDatum(data);

	mode 	 = datum >> 56;
	msgCount = datum >> 48;
}



//
///////////////////////////////////////
// SpeedEffortMsg CLASS IMPLEMENTATION
//
//
SpeedEffortMsg::SpeedEffortMsg() : J1939Msg() {}

SpeedEffortMsg::SpeedEffortMsg(uint8_t data[8]) : J1939Msg(ID_SPEEDEFFORT, data) {
	fillMsg(data);
}

SpeedEffortMsg::SpeedEffortMsg(CanMsg &msg) : J1939Msg(msg) {
	fillMsg(msg.data);
}

void SpeedEffortMsg::fillMsg(uint8_t data[8]) {
	wheelSpeed 	 = (data[6] << 8) + data[5];
	motorEffort  = (data[2] << 8) + data[3];
}



//
///////////////////////////////////////
// CAN CLASS IMPLEMENTATION
//


CAN::CAN(CAN_TypeDef *base, uint16_t queueSize) : ThreadSafe() {
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


	if(base == CAN1) {
		HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0U);
		HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

		CAN_FilterTypeDef canfilterconfig;
		canfilterconfig.FilterActivation 		= CAN_FILTER_ENABLE;
		canfilterconfig.FilterFIFOAssignment 	= CAN_FILTER_FIFO0;
		canfilterconfig.FilterIdHigh			= 0x0000;
		canfilterconfig.FilterIdLow				= 0x0000;
		canfilterconfig.FilterMaskIdHigh 		= 0x0000;
		canfilterconfig.FilterMaskIdLow 		= 0x0000;
		canfilterconfig.FilterMode 				= CAN_FILTERMODE_IDMASK;
		canfilterconfig.FilterScale 			= CAN_FILTERSCALE_32BIT;
		canfilterconfig.FilterBank = 19;

		canfilterconfig.SlaveStartFilterBank	= 20;

		if(HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig) != HAL_OK)
			Error_Handler();
	}
	else {
		HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5,0U);
		HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);

		CAN_FilterTypeDef canfilterconfig;
		canfilterconfig.FilterActivation 		= CAN_FILTER_ENABLE;
		canfilterconfig.FilterFIFOAssignment 	= CAN_FILTER_FIFO0;
		canfilterconfig.FilterIdHigh			= 0x0000;
		canfilterconfig.FilterIdLow				= 0x0000;
		canfilterconfig.FilterMaskIdHigh 		= 0x0000;
		canfilterconfig.FilterMaskIdLow 		= 0x0000;
		canfilterconfig.FilterMode 				= CAN_FILTERMODE_IDMASK;
		canfilterconfig.FilterScale 			= CAN_FILTERSCALE_32BIT;
		canfilterconfig.FilterBank 				= 27;
		canfilterconfig.SlaveStartFilterBank	= 20;

		if(HAL_CAN_ConfigFilter(&hcan2, &canfilterconfig) != HAL_OK)
			Error_Handler();
	}

	if(HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
		Error_Handler();

	if(HAL_CAN_Start(handle) != HAL_OK)
		Error_Handler();


	/* Definitions for can2Mutex */


	mutex = osSemaphoreNew(1,1,NULL);
	if(mutex == NULL)
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
	std::map<uint32_t, CANIrqCb>::iterator itr = subscriptions.find(msg.id);
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

int CAN::subscribe(uint32_t id, CANIrqCb cb) {
	lock();
	subscriptions.insert(std::pair<uint32_t, CANIrqCb>(id, cb));
	unlock();
	return 0;
}


bool CAN::isAvailable(uint32_t id) {
	lock();
	int count = queue.size();
	unlock();
	return (count > 0) ? true : false;
}


void CAN::__fifo0MsgPendingIrq(CanMsg &msg) {
	if(cb != NULL) cb(&msg);

	std::map<uint32_t, CANIrqCb>::iterator itr = subscriptions.find(msg.id);
	if(itr != subscriptions.end()) {
		itr->second(msg);
		return;
	}

	queue.push(msg);
	if(queue.size() > queueSize) queue.pop();
}
