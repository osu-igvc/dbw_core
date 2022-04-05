/*
 * ThreadSafe.h
 *
 *  Created on: Mar 31, 2022
 *      Author: colli
 */

#ifndef INC_THREADSAFE_H_
#define INC_THREADSAFE_H_

#include "cmsis_os.h"

class ThreadSafe {
public:
	ThreadSafe();
	ThreadSafe(uint8_t nSafeInstances);
	virtual ~ThreadSafe();

protected:
	void lock(uint32_t timeout = osWaitForever);
	void unlock();

private:
	osSemaphoreId_t sem;
};

#endif /* INC_THREADSAFE_H_ */
