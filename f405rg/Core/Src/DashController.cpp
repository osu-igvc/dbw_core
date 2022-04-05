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

DashController::DashController(const char *name, int period_ms, uint32_t stack_size) :
Thread(std::bind(&DashController::run, this, _1), NULL, name, stack_size) {
	this->period_ms = period_ms;

	this->led1 = new DigitalOut(GPIOB, GPIO_Pin_6);
	this->led2 = new DigitalOut(GPIOB, GPIO_Pin_5);
	this->led3 = new DigitalOut(GPIOB, GPIO_Pin_4);

	this->fnrState0 = new DigitalOut(GPIOB, GPIO_Pin_0);
	this->fnrState1 = new DigitalOut(GPIOB, GPIO_Pin_1);
	this->relayEnable = new DigitalOut(GPIOC, GPIO_Pin_4);
	this->parkingBrake = new DigitalOut(GPIOA, GPIO_Pin_6);

	this->accel1 = new AnalogOut(GPIOA, GPIO_Pin_4);
	this->accel2 = new AnalogOut(GPIOA, GPIO_Pin_5);

	this->can1 = new CAN(CAN1, 2);
	this->can2 = new CAN(CAN2, 2);

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

		//led1->set((incr++ % 11)/10.0);

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
