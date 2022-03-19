/*
 * DashController.h
 *
 *  Created on: Mar 19, 2022
 *      Author: colli
 */

#ifndef INC_DASHCONTROLLER_H_
#define INC_DASHCONTROLLER_H_

#include "stm32f4xx_hal.h"
#include <Thread.h>

#include "Can.h"
#include "DigitalOut.h"
#include "PWM.h"
#include "main.h"

class DashController : public Thread {
public:
	DashController(const char *name, int period_ms, uint32_t stack_size = 512);
	virtual ~DashController();

	void run(void *argument);
	void canLed2Cb(CanMsg *msg);

private:
	int period_ms;

	PWM led1;
	DigitalOut led2, led3;
	CAN can2;
};

#endif /* INC_DASHCONTROLLER_H_ */
