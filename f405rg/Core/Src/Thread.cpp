/*
 * Thread.cpp
 *
 *  Created on: Mar 19, 2022
 *      Author: colli
 */


#include "Thread.h"

namespace {
std::map<osThreadId_t, Thread*> objectMap = std::map<osThreadId_t, Thread*>();
}

Thread::Thread(osThreadFunc_t func, void *args, const char *name, uint32_t stack_size, osPriority_t priority) :
		tattrs{.name=name, .attr_bits=osThreadJoinable, .stack_size=stack_size, .priority=priority} {
	tid = osThreadNew(func, args, &tattrs);
}

Thread::Thread(std::function<void(void*)> func, void *args, const char *name, uint32_t stack_size, osPriority_t priority) :
			tattrs{.name=name, .stack_size=stack_size, .priority=priority} {
	fp = func;
	tid = osThreadNew(run, args, &tattrs);
	objectMap.insert(std::pair<osThreadId_t, Thread*>(tid, this));
}

Thread::Thread() {}

void Thread::initThread(std::function<void(void*)> func, void *args, const char *name, uint32_t stack_size, osPriority_t priority) {
	tattrs.name = name;
	tattrs.attr_bits = osThreadJoinable;
	tattrs.stack_size = stack_size;
	tattrs.priority = priority;
	tid = osThreadNew(run, args, &tattrs);
	objectMap.insert(std::pair<osThreadId_t, Thread*>(tid, this));
}

void Thread::run(void *argument) {
	osThreadId_t tid = osThreadGetId();
	std::map<osThreadId_t, Thread*>::iterator itr = objectMap.find(tid);
	if(itr != objectMap.end()) itr->second->fp(argument);
}

void Thread::join() {
	osThreadState_t state = osThreadGetState(tid);
	while(state != osThreadInactive && state != osThreadTerminated && state != osThreadError) {
		osDelay(10);
		state = osThreadGetState(tid);
	}
}
