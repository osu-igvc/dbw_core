
/*
 * StaticThread.h
 *
 *  Created on: Jan 19, 2022
 *      Author: 14055
 */

#ifndef STATICTHREAD_H_
#define STATICTHREAD_H_

#include "cmsis_os.h"
#include "string.h"

class StaticThread  {
	void *args;
	bool _kill = false;

public:
	const osThreadAttr_t tattrs;
	osThreadId_t tid;

	StaticThread(osThreadFunc_t func, const char *name, void *args = NULL , uint32_t stack_size = 512, osPriority_t priority =(osPriority_t)osPriorityNormal);
	virtual ~StaticThread() ;
};

#endif /* STATICTHREAD_H_ */
