/*
 * DashController.cpp
 *
 *  Created on: Mar 19, 2022
 *      Author: colli
 */

#include <DashController.h>
#include <functional>

#include "math.h"


#include "main.h"

#include "dbw_polaris_can/dispatch.h"

using namespace dbw_polaris_can;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;



using namespace std::placeholders;

DashController::DashController(const char *name, int period_ms, uint32_t stack_size) :
Thread(std::bind(&DashController::run, this, _1), NULL, name, stack_size) {
	this->period_ms = period_ms;

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

	this->eStop = new DigitalIn(GPIOC, GPIO_PIN_8);

	this->accel1 = new AnalogOut(PA4);
	this->accel2 = new AnalogOut(PA5);

	//this->can1 = new CAN(CAN1, 2);
	this->can2 = new CAN(CAN2, 5);

	this->enableTimeout = new Timer();
	this->enableTimeout->start();

}

DashController::~DashController() {

}

void DashController::run(void *argument) {
	  can2->subscribe(ID_FB9STATE, std::bind(&DashController::canLed2Cb, this, _1));
	  can2->subscribe(ID_SPEEDEFFORT, std::bind(&DashController::speedEffortCb, this, _1));

	  can2->subscribe(ID_THROTTLE_CMD, std::bind(&DashController::throttleCmdCb, this, _1));
	  can2->subscribe(ID_GEAR_CMD, std::bind(&DashController::gearCmdCb, this, _1));

	  can2->subscribe(1, std::bind(&DashController::enabledCmdCb, this, _1));

	  uint8_t data[8] = {0x03, 0x7d, 0x00, 0x7d, 0x00, 0x80, 0x46, 0xff};
	  FB7PosVelMsg msg(data);


	  relayEnable->reset();
	  Timer dt;

	  while (1)
	  {
		if(enableTimeout->read() > 100) enabled = false;

		if(enabled)
			relayEnable->set();
		else
			relayEnable->reset();

		uint32_t fill = HAL_CAN_GetRxFifoFillLevel(&hcan2, CAN_RX_FIFO0);

		int ret = can2->send(msg);
		//int ret2 = can1->send(msg);

		led1->toggle();
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

void DashController::speedEffortCb(CanMsg &msg) {
	SpeedEffortMsg speedMsg(msg);

	uint16_t test = speedMsg.wheelSpeed;
}



void DashController::throttleCmdCb(CanMsg &msg) {
	uint16_t cmd = ((uint16_t)msg.data[0]<<8) + (uint16_t)msg.data[1];
	float throttle = inverseLinearTransform(cmd, 0.0, 25.0, 0.0, pow(2,16));
	setThrottle(throttle);
}

void DashController::gearCmdCb(CanMsg &msg) {
	uint16_t cmd = msg.data[0];
	switch(cmd) {
	case 0:
		setGear(FORWARD);
		break;
	case 1:
		setGear(REVERSE);
		break;
	case 2:
		setGear(NEUTRAL);
		break;
	}
}

void DashController::enabledCmdCb(CanMsg &msg) {
	enabled = (msg.data[0] == 1) ? true : false;
	enableTimeout->restart();
}





void DashController::setThrottle(float mph) {
	float s1 = linearTransform(mph, 0, 10.0, 1.082/2, 4.14/2);
	float s2 = linearTransform(mph, 0, 10.0, 0.538/2, 2.089/2);
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

double DashController::linearTransform(double x, double x1, double x2, double y1, double y2) {
	double m = (y2-y1)/(x2-x1);
	double y = m*(x-x1) + y1;
	return y;
}

double DashController::inverseLinearTransform(double y, double x1, double x2, double y1, double y2) {
	double m = (y2-y1)/(x2-x1);
	double x = (y-y1)/m + x1;
	return x;
}

