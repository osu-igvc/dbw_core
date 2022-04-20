/*
 * DashController.cpp
 *
 *  Created on: Mar 19, 2022
 *      Author: colli
 */

#include <DashController.h>
#include <functional>
#include "main.h"

#include "Timer.h"


using namespace std::placeholders;

DashController::DashController(const char *name, int period_ms, uint32_t stack_size) :
Thread(std::bind(&DashController::run, this, _1), NULL, name, stack_size) {
	this->period_ms = period_ms;
	throttleButtonRise = false;
	inAccelerationProgram = false;

	this->led1 = new DigitalOut(GPIOB, GPIO_PIN_6);
	this->led2 = new DigitalOut(GPIOB, GPIO_PIN_5);
	this->led3 = new DigitalOut(GPIOB, GPIO_PIN_4);

	this->stmF = new DigitalOut(GPIOB, GPIO_PIN_0);
	this->stmN = new DigitalOut(GPIOB, GPIO_PIN_1);
	this->stmR = new DigitalOut(GPIOB, GPIO_PIN_2);

	this->relayEnable = new DigitalOut(GPIOC, GPIO_PIN_4);
	this->parkingBrake = new DigitalOut(GPIOA, GPIO_PIN_6);

	this->polarisR = new DigitalIn(GPIOC, GPIO_PIN_6);
	this->polarisN = new DigitalIn(GPIOB, GPIO_PIN_14);
	this->polarisF = new DigitalIn(GPIOB, GPIO_PIN_15);

	throttleButton = new DigitalIn(GPIOB, GPIO_PIN_1, RISE, std::bind(&DashController::throttleButtonCb, this, _1), GPIO_PULLDOWN);

	this->eStop = new DigitalIn(GPIOC, GPIO_PIN_8);

	this->accel1 = new AnalogOut(PA4);
	this->accel2 = new AnalogOut(PA5);

	//this->can1 = new CAN(CAN1, 2);
	this->can2 = new CAN(CAN2, 2);

}

DashController::~DashController() {

}

void DashController::run(void *argument) {
	  can2->subscribe(ID_FB9STATE, std::bind(&DashController::canLed2Cb, this, _1));
	  can2->subscribe(ID_SPEEDEFFORT, std::bind(&DashController::speedEffortCb, this, _1));

	  uint8_t data[8] = {0x03, 0x7d, 0x00, 0x7d, 0x00, 0x80, 0x46, 0xff};
	  FB7PosVelMsg msg(data);

	  int incr = 0;

	  relayEnable->set();
	  Timer dt;
	  float mph = 0;

	  while (1)
	  {
		if(throttleButtonRise) {
			inAccelerationProgram = true;
			throttleButtonRise = false;
			mph = 0;
			dt.restart();
			dt.start();
		}

		if(inAccelerationProgram) {
			uint32_t time = dt.getElapsedTime();
			if(time < 5000) {
				mph += 3.0*(float)period_ms/1000.0;
			}
			if(time > 10000) {
				mph -= 15.0*(float)period_ms/1000.0;
			}
			if(time > 11000) {
				mph = 0;
				inAccelerationProgram = false;
				throttleButtonRise = false;
			}
		}

		setGear(NEUTRAL);
		can2->send(msg);

		//setThrottle(mph);
		//setThrottle(10);
		//setGear(REVERSE);
		//led1->set((incr++ % 11)/10.0);

		//if(polarisR->read() == 1)
		//	bool test = false;

		//if(polarisN->read() == 1)
		//	bool test = false;

		//if(polarisF->read() == 1)
		//	bool test = false;


		led1->toggle();
		osDelay(period_ms);
	  }
}

void DashController::canLed2Cb(CanMsg &msg) {
	FB9StateMsg stateMsg(msg);

	switch(stateMsg.mode) {
	case OFF:
		led2->reset();
		break;
	case ASSIST:
		led2->toggle();
		break;
	case POS_VEL:
		led2->set();
		break;
	}
}

void DashController::speedEffortCb(CanMsg &msg) {
	SpeedEffortMsg speedMsg(msg);

	uint16_t test = speedMsg.wheelSpeed;
}

void DashController::throttleButtonCb(uint8_t value) {
	if(throttleButton->read() != 1) return;
	throttleButtonRise = true;
}


void DashController::setThrottle(float mph) {
	mph = (mph < 0) ? 0 : mph;
	mph = (mph > 10.0) ? 10.0 : mph;
	float s1 = linearTransform(mph, 0, 1.082/1.5, 25, 4.14/1.5);
	float s2 = linearTransform(mph, 0, 0.538/1.5, 25, 2.089/1.5);
	accel1->write(s1);
	accel2->write(s2);
}

void DashController::setGear(Gear gear) {
	switch(gear) {
	case FORWARD:
		stmF->set();
		stmN->reset();
		stmR->reset();
		break;
	case NEUTRAL:
		stmF->reset();
		stmN->set();
		stmR->reset();
		break;
	case REVERSE:
		stmF->reset();
		stmN->reset();
		stmR->set();
		break;
	default:
		Error_Handler();
		break;
	}
}

float DashController::linearTransform(float x, float x1, float y1, float x2, float y2) {
	float m = (y2-y1)/(x2-x1);
	return m*(x-x1) + y1;
}

