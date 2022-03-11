
/*
 * StaticThread.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: 14055
 */

#include "StaticThread.h"

StaticThread::StaticThread(osThreadFunc_t func, const char *name, void *args ,uint32_t stack_size, osPriority_t priority ) :
	tattrs{.name=name, .stack_size=stack_size, .priority=priority} {
	tid = osThreadNew(func, args, &tattrs);

	// TODO Auto-generated constructor stub

}

StaticThread::~StaticThread() {
	// TODO Auto-generated destructor stub

}
