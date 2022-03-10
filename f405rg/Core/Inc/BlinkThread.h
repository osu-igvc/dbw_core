/*
 * BlinkThread.h
 *
 *  Created on: Jan 19, 2022
 *      Author: colli
 */

#ifndef BLINKTHREAD_H_
#define BLINKTHREAD_H_

#include <Thread.h>
#include "DigitalOut.h"

#include "stm32f4xx_hal.h"

class BlinkThread: public Thread<BlinkThread> {
public:
	BlinkThread(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int period_ms, const char *name, uint32_t stack_size = 512, osPriority_t priority = (osPriority_t)osPriorityNormal);
	virtual ~BlinkThread();

	void run(void *argument);

protected:
	ThreadArg<BlinkThread> *threadArgs;

private:
	DigitalOut led;
	int period_ms;
};

#endif /* BLINKTHREAD_H_ */
