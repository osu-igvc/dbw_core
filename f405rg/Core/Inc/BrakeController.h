/*
 * BrakeController.h
 *
 *  Created on: Mar 20, 2022
 *      Author: colli
 */

#ifndef SRC_BRAKECONTROLLER_H_
#define SRC_BRAKECONTROLLER_H_

#include <CAN.h>
#include <Thread.h>

#include "PWM.h"
#include "DigitalOut.h"

class BrakeController: public Thread {
public:
	BrakeController(const char *name, int period_ms, DigitalOut *led1, DigitalOut *led2,
			DigitalOut *led3, CAN *can2, uint32_t stack_size = 512);
	virtual ~BrakeController();

	void run(void *argument);

private:
	void thread1(void *argument);
	void thread2(void *argument);

	void canLed2Cb(CanMsg &msg);

	int period_ms;

	DigitalOut *led1, *led2, *led3;
	CAN *can2;
};

#endif /* SRC_BRAKECONTROLLER_H_ */
