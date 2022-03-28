
/*
 * DigitalIn.cpp
 *
 *  Created on: Jan 20, 2022
 *      Author: 14055
 */

#include "DigitalIn.h"

DigitalIn::DigitalIn(GPIO_TypeDef* port, uint16_t pin, uint32_t pullMode, uint32_t speed) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = pullMode;
	GPIO_InitStruct.Speed = speed;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	this->port = port;
	this->pin = pin;
}

DigitalIn::~DigitalIn() {

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
