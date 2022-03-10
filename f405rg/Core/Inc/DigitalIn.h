
/*
 * DigitalIn.h
 *
 *  Created on: Jan 20, 2022
 *      Author: 14055
 */

#ifndef DIGITALIN_H_
#define DIGITALIN_H_

#include "stm32f4xx_hal.h"

class DigitalIn {
public:
	DigitalIn(GPIO_TypeDef* port, uint16_t pin, uint32_t pullMode = GPIO_NOPULL, uint32_t speed = GPIO_SPEED_FREQ_LOW);
	virtual ~DigitalIn();

	uint8_t read();

	bool operator == (const int &val);
	bool operator == (DigitalIn &obj);
	bool operator != (const int &val);
	bool operator != (DigitalIn &obj);


private:
	GPIO_TypeDef* port;
	uint16_t pin;
};

#endif /* DIGITALIN_H_ */
