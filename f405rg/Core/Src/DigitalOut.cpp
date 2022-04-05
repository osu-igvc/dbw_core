/*
 * DigitalOut.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: colli
 */

#include "DigitalOut.h"
#include "main.h"

DigitalOut::DigitalOut(GPIO_TypeDef* port, uint16_t pin) : ThreadSafe() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	/* Definitions for can2Mutex */

	mutex = osSemaphoreNew(1,1,NULL);
	if(mutex == NULL)
		Error_Handler();

	this->port = port;
	this->pin = pin;
	state = 0;
}

DigitalOut::~DigitalOut() {
}

void DigitalOut::set() {
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
	state = 1;
}

void DigitalOut::reset() {
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
	state = 0;
}

void DigitalOut::toggle() {
	if(state==1)
		reset();
	else
		set();
}

bool DigitalOut::getState() {
	return (state == 1) ? true : false;
}

void DigitalOut::operator = (const int &val) {
	switch(val) {
	case 0:
		reset();
		break;
	case 1:
		set();
		break;
	default:
		//TODO log error
		break;
	}
}

bool DigitalOut::operator ! () {
	return (state == 0) ? 1 : 0;
}

bool DigitalOut::operator == (const bool &state) {
	return getState() == state;
}

void DigitalOut::lock() {
	osSemaphoreAcquire(mutex, osWaitForever);
}

void DigitalOut::unlock() {
	osSemaphoreAcquire(mutex, osWaitForever);
}
