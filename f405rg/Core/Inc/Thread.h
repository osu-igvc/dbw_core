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

#include <functional>
#include <map>


class Thread {
protected:
	bool _kill = false;

private:
	osThreadAttr_t tattrs;
	osThreadId_t tid;

	std::function<void(void*)> fp;


public:
	Thread();
	Thread(osThreadFunc_t func, void *args, const char *name, uint32_t stack_size = 512,
			osPriority_t priority = (osPriority_t)osPriorityNormal);
	Thread(std::function<void(void*)> func, void *args, const char *name, uint32_t stack_size = 512,
			osPriority_t priority = (osPriority_t)osPriorityNormal);

	void initThread(std::function<void(void*)> func, void *args, const char *name, uint32_t stack_size, osPriority_t priority);

	void join();

	virtual ~Thread() {};
	virtual void kill() {_kill = true; };
	static void run(void *argument);
};



#endif /* SRC_THREAD_H_ */
