
/*
 * DigitalIn.cpp
 *
 *  Created on: Jan 20, 2022
 *      Author: 14055
 */

#include "DigitalIn.h"

#include <cstring>
#include <algorithm>

std::map<uint16_t, DigitalIn*> DigitalIn::objectMap = std::map<uint16_t, DigitalIn*>();




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	std::map<uint16_t, DigitalIn*>::iterator itr = DigitalIn::objectMap.find(GPIO_Pin);
	if(itr != DigitalIn::objectMap.end()) itr->second->__EXTI_Interrupt_CB();
}



DigitalIn::DigitalIn(GPIO_TypeDef* port, uint16_t pin, DigitalIn interruptMode, digitalInIQRCb cb, uint32_t pullMode, uint32_t speed) {

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Pull = pullMode;
	GPIO_InitStruct.Speed = speed;


	uint32_t mode;
	switch(interruptMode) {
	case RISING:
		mode = GPIO_MODE_IT_RISING;
		break;
	case FALL:
		mode = GPIO_MODE_IT_FALLING;
		break;
	case CHANGE:
		mode = GPIO_MODE_IT_RISING_FALLING;
		break;
	default:
		Error_Handler();
	}
	GPIO_InitStruct.Mode = mode;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	IRQn_Type irqType;

	switch(pin) {
		case GPIO_PIN_0:
			irqType = EXTI0_IRQn;
			break;
		case GPIO_PIN_1:
			irqType = EXTI1_IRQn;
			break;
		case GPIO_PIN_2:
			irqType = EXTI2_IRQn;
			break;
		case GPIO_PIN_3:
			irqType = EXTI3_IRQn;
			break;
		case GPIO_PIN_4:
			irqType = EXTI4_IRQn;
			break;
		case GPIO_PIN_5:
			irqType = EXTI9_5_IRQn;
			break;
		case GPIO_PIN_6:
			irqType = EXTI9_5_IRQn;
			break;
		case GPIO_PIN_7:
			irqType = EXTI9_5_IRQn;
			break;
		case GPIO_PIN_8:
			irqType = EXTI9_5_IRQn;
			break;
		case GPIO_PIN_9:
			irqType = EXTI9_5_IRQn;
			break;
		default:
			irqType = EXTI15_10_IRQn;
			break;
	}

	HAL_NVIC_SetPriority(irqType, 5, 0);
	HAL_NVIC_EnableIRQ(irqType);

	this->port = port;
	this->pin  = pin;
	this->cb = cb;

	std::map<uint16_t, DigitalIn*>::iterator itr = DigitalIn::objectMap.find(pin);
	if(itr != objectMap.end()) Error_Handler();

	objectMap.insert(std::pair<uint16_t, DigitalIn*>(pin, this));
}



DigitalIn::DigitalIn(GPIO_TypeDef* port, uint16_t pin, uint32_t pullMode, uint32_t speed) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = pullMode;
	GPIO_InitStruct.Speed = speed;
	HAL_GPIO_Init(port, &GPIO_InitStruct);


	this->port = port;
	this->pin  = pin;
}


DigitalIn::~DigitalIn() {
	std::map<uint16_t, DigitalIn*>::iterator itr = DigitalIn::objectMap.find(pin);
	if(itr != objectMap.end())
		objectMap.erase(pin);
}

uint8_t DigitalIn::read() {
	return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET) ? 1 : 0;
}



bool DigitalIn::operator == (const int &val) {
	return (read() == val);
}
bool DigitalIn::operator == (DigitalIn &obj) {
	return (read() == obj.read());
}

bool DigitalIn::operator !=  (const int &val) {
	return (read() != val);
}
bool DigitalIn::operator != (DigitalIn &obj) {
	return (read() != obj.read());
}


void DigitalIn::__EXTI_Interrupt_CB(void) {
	uint8_t value = (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET) ? 1 : 0;
	cb(value);
}


void DigitalIn::Error_Handler() {
	while(1) {
	// Something bad happened
	}
}
