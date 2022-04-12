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

#include "ThreadSafe.h"


class DigitalOut : ThreadSafe {
public:
	DigitalOut(GPIO_TypeDef* port, uint16_t pin, uint32_t pull = GPIO_NOPULL);
	virtual ~DigitalOut();

	bool getState();

	void set();
	void reset();
	void toggle();

	void operator = (const int &val);
	bool operator ! ();
	bool operator == (const bool &state);

private:
	void lock();
	void unlock();

	GPIO_TypeDef* port;
	uint16_t pin;
	uint8_t state;

	osSemaphoreId_t mutex;
};


#endif /* DIGITALOUT_H_ */
