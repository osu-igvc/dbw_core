/*
 * QuadEnc.h
 *
 *  Created on: March 25, 2022
 *      Author: Tyler Tucker
 */

#ifndef QUADENC_H_
#define QUADENC_H_

#define INPERPULSE 0.000878

#include <Thread.h>

#include "stm32f4xx_hal.h"

class QuadEnc	: public QuadEnc {
public:
	QuadEnc(const char *name, uint32_t stack_size = 512, osPriority_t priority = (osPriority_t)osPriorityNormal);
	virtual ~QuadEnc();

	uint16_t getCount();
	void resetCount();
	uint16_t getSpeed();
	void run(void *argument);

private:
	uint8_t count;
	uint8_t ch2Prev;
	uint16_t positions[2][10];
};

#endif /* QUADENC_H_ */
