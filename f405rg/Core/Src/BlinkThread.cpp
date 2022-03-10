/*
 * BlinkThread.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: colli
 */

#include "BlinkThread.h"
#include <stdlib.h>

BlinkThread::BlinkThread(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int period_ms, const char *name, uint32_t stack_size, osPriority_t priority) :
	Thread<BlinkThread>(this, name), led(GPIOx, GPIO_Pin) {
	// TODO Auto-generated constructor stub
	this->period_ms = period_ms;
}

BlinkThread::~BlinkThread() {
	// TODO Auto-generated destructor stub
	delete threadArgs;
}

void BlinkThread::run(void *argument) {
	while(_kill == false) {
		led = !led;
		osDelay(period_ms);
	}
}

