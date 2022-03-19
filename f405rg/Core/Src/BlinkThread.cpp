/*
 * BlinkThread.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: colli
 */

#include "BlinkThread.h"
#include <stdlib.h>

using namespace std::placeholders;

BlinkThread::BlinkThread(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int period_ms, const char *name, uint32_t stack_size, osPriority_t priority) :
	Thread(std::bind(&BlinkThread::run, this, _1), NULL, name, stack_size), led(GPIOx, GPIO_Pin) {

	this->period_ms = period_ms;
}

BlinkThread::~BlinkThread() {

}

void BlinkThread::run(void *argument) {
	while(_kill == false) {
		led = !led;
		osDelay(period_ms);
	}
}

