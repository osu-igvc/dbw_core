/*
 * DigitalOut.h
 *
 *  Created on: Jan 19, 2022
 *      Author: colli
 */

#ifndef DIGITALOUT_H_
#define DIGITALOUT_H_

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

class DigitalOut {
public:
	DigitalOut(GPIO_TypeDef* port, uint16_t pin);
	virtual ~DigitalOut();

	bool getState();

	void set();
	void reset();

	void operator = (const int &val);
	bool operator ! ();
	bool operator == (const bool &state);
protected:
	uint8_t state;
private:
	GPIO_TypeDef* port;
	uint16_t pin;
};


#endif /* DIGITALOUT_H_ */
