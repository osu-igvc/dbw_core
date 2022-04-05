/*
 * ThreadSafe.cpp
 *
 *  Created on: Mar 31, 2022
 *      Author: colli
 */

#include <ThreadSafe.h>
#include "main.h"


ThreadSafe::ThreadSafe() {
	sem = osSemaphoreNew(1, 1, NULL);
	if(sem == NULL)
		Error_Handler();
}

ThreadSafe::ThreadSafe(uint8_t nSafeInstances) {
	sem = osSemaphoreNew(nSafeInstances, nSafeInstances, NULL);
	if(sem == NULL)
		Error_Handler();
}

ThreadSafe::~ThreadSafe() {
	if(osSemaphoreDelete(sem) != osOK)
		Error_Handler();
}

void ThreadSafe::lock(uint32_t timeout) {
	if(osSemaphoreAcquire(sem, timeout) != osOK)
		Error_Handler();
}

void ThreadSafe::unlock() {
	if(osSemaphoreRelease(sem) != osOK)
		Error_Handler();
}
