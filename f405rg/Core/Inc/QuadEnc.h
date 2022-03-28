/*
 * QuadEnc.h
 *
 *  Created on: March 25, 2022
 *      Author: Tyler Tucker
 */

#ifndef QUADENC_H_
#define QUADENC_H_

#include <Thread.h>
#include "DigitalIn.h"

#include "stm32f4xx_hal.h"

class QuadEnc	: public Thread<QuadEnc> {
public:
	QuadEnc(DigitalIn ch1, DigitalIn ch2, const char *name, uint32_t stack_size = 512, osPriority_t priority = (osPriority_t)osPriorityNormal);
	virtual ~QuadEnc();

	void run(void *argument);

protected:
	ThreadArg<QuadEnc> *threadArgs;

private:
	DigitalIn ch1;
	DigitalIn ch2;
	uint16_t currentPos;
	uint16_t previousPos;
};

#endif /* QUADENC_H_ */
