/*
 * Thread.h
 *
 *  Created on: Jan 18, 2022
 *      Author: collin thornton
 */

#ifndef SRC_THREAD_H_
#define SRC_THREAD_H_

#include "cmsis_os.h"
#include "string.h"

template<class T>
struct ThreadArg{
	T *classRef;
	void *argument;
};


template<class T>
class Thread {
protected:
	static void _run(void *argument) {
		ThreadArg<T>* args = reinterpret_cast<ThreadArg<T>*>(argument);
		args->classRef->run(argument);
	};

	void *arg;
	bool _kill = false;

public:
	const osThreadAttr_t tattrs;
	osThreadId_t tid;

	Thread(T *objectRef, const char *name, uint32_t stack_size = 512, osPriority_t priority = (osPriority_t)osPriorityNormal) :
		tattrs{.name=name, .stack_size=stack_size, .priority=priority} {
			ThreadArg<T> *threadArgs = new ThreadArg<T>;
			threadArgs->classRef = objectRef;
			arg = static_cast<void*>(threadArgs);
			tid = osThreadNew(_run, arg, &tattrs);
	};

	virtual ~Thread() {};

	virtual void kill() {_kill = true; };

	virtual void run(void *argument) = 0;
};



#endif /* SRC_THREAD_H_ */
