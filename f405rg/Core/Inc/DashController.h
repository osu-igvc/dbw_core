/*
 * DashController.h
 *
 *  Created on: Mar 19, 2022
 *      Author: colli
 */

#ifndef INC_DASHCONTROLLER_H_
#define INC_DASHCONTROLLER_H_

#include <CAN.h>
#include "stm32f4xx_hal.h"
#include <Thread.h>

#include "DigitalIn.h"
#include "DigitalOut.h"
#include "PWM.h"
#include "AnalogOut.h"
#include "Timer.h"

typedef enum {
	FORWARD,
	REVERSE,
	NEUTRAL
} Gear;


class DashController : public Thread {
public:
	DashController(const char *name, int period_ms, uint32_t stack_size = 512);
	virtual ~DashController();

	void run(void *argument);
	void canLed2Cb(CanMsg &msg);

private:
	void speedEffortCb(CanMsg &msg);
	void throttleCmdCb(CanMsg &msg);
	void gearCmdCb(CanMsg &msg);
	void steeringCmdCb(CanMsg &msg);
	void enabledCmdCb(CanMsg &msg);


	int period_ms;

	double linearTransform(double x, double x1, double x2, double y1, double y2);
	double inverseLinearTransform(double y, double x1, double x2, double y1, double y2);
	void setThrottle(float mph);
	void setGear(Gear gear);

	void throttleButtonCb(uint8_t value);

	DigitalOut *led1, *led2, *led3;
	DigitalOut *stmR, *stmN, *stmF, *relayEnable;
	DigitalOut *parkingBrake;

	DigitalIn *polarisR, *polarisN, *polarisF;
	DigitalIn *eStop;

	AnalogOut *accel1, *accel2;

	CAN *can2, *can1;

	Timer *enableTimeout;

	bool enabled = false;
};

#endif /* INC_DASHCONTROLLER_H_ */
