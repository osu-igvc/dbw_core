/*
 * Timer.h
 *
 *  Created on: Mar 31, 2022
 *      Author: root
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "stm32f4xx_hal.h"

class Timer {
public:
	Timer();
	virtual ~Timer();

	void start();
	void restart();
	uint32_t getElapsedTime(); //milliseconds
	uint32_t read(); //milliseconds

private:
	uint32_t startTick;
};

#endif /* SRC_TIMER_H_ */
