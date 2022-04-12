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
	int period_ms;

	float linearTransform(float x, float x1, float y1, float x2, float y2);
	void setThrottle(float mph);
	void setGear(Gear gear);

	volatile bool throttleButtonRise;
	bool inAccelerationProgram;

	void throttleButtonCb(uint8_t value);

	DigitalOut *led1, *led2, *led3;
	DigitalOut *fnrState0, *fnrState1, *relayEnable;
	DigitalOut *parkingBrake;

	DigitalIn *polarisR, *polarisN, *polarisF;
	DigitalIn *eStop;
	DigitalIn *throttleButton;

	AnalogOut *accel1, *accel2;

	CAN *can1, *can2;
};

#endif /* INC_DASHCONTROLLER_H_ */
