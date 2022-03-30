/*
 * DashController.cpp
 *
 *  Created on: Mar 19, 2022
 *      Author: colli
 */

#include <DashController.h>
#include <functional>
#include "main.h"


using namespace std::placeholders;

DashController::DashController(const char *name, int period_ms, PWM *led1,
		DigitalOut *led2, DigitalOut *led3, CAN *can2, uint32_t stack_size) :
Thread(std::bind(&DashController::run, this, _1), NULL, name, stack_size) {
	this->period_ms = period_ms;
	this->led1 = led1;
	this->led2 = led2;
	this->led3 = led3;
	this->can2 = can2;
}

DashController::~DashController() {

}

void DashController::run(void *argument) {
	  can2->subscribe(ID_FB9STATE, std::bind(&DashController::canLed2Cb, this, _1));

	  uint8_t data[8] = {0x03, 0x7d, 0x00, 0x7d, 0x00, 0x80, 0x46, 0xff};
	  FB7PosVelMsg msg(data);

	  int incr = 0;

	  while (1)
	  {
		can2->send(msg);

		led1->set((incr++ % 11)/10.0);

		led3->toggle();
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
