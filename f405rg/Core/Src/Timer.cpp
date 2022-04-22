/*
 * Timer.cpp
 *
 *  Created on: Mar 31, 2022
 *      Author: root
 */

#include "Timer.h"
#include "cmsis_os.h"

Timer::Timer() {
	// TODO Auto-generated constructor stub

}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::start() {
	startTick = osKernelGetTickCount();
}

void Timer::restart() {
	startTick = osKernelGetTickCount();
}


uint32_t Timer::getElapsedTime() {
	uint32_t elapsedTicks = osKernelGetTickCount()-startTick;
	return 1000*elapsedTicks/osKernelGetTickFreq();
}

uint32_t Timer::read() {
	return getElapsedTime();
}
