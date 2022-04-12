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
#include "DigitalIn.h"
#include "Timer.h"

class BrakeController: public Thread {
public:
	BrakeController(const char *name, int period_ms, uint32_t stack_size = 512);
	virtual ~BrakeController();

	void run(void *argument);

private:
	void thread1(void *argument);
	void thread2(void *argument);

	void canLed2Cb(CanMsg &msg);
	void digitalInCb2(uint8_t value);

	int period_ms;

	PWM *brake, *eBrake;

	DigitalOut *led1, *led2, *led3;
	DigitalIn *dIn2;
	CAN *can2;

	Timer tim1, tim2;
};

#endif /* SRC_BRAKECONTROLLER_H_ */
