/*
 * BrakeController.cpp
 *
 *  Created on: Mar 20, 2022
 *      Author: colli
 */

#include "BrakeController.h"

#include <functional>
#include "main.h"

using namespace std::placeholders;


BrakeController::BrakeController(const char *name, int period_ms, uint32_t stack_size) :
Thread(std::bind(&BrakeController::run, this, _1), NULL, name, stack_size) {
	this->period_ms = period_ms;
	led1 = new DigitalOut(LD1_GPIO_Port, LD1_Pin);
	led2 = new DigitalOut(LD2_GPIO_Port, LD2_Pin);
	led3 = new DigitalOut(LD3_GPIO_Port, LD3_Pin);
	can2 = new CAN(CAN2, 2);

	dIn1 = new DigitalIn(GPIOC, GPIO_PIN_5, RISE, std::bind(&BrakeController::digitalInCb1, this, _1), GPIO_PULLDOWN);
	dIn2 = new DigitalIn(GPIOC, GPIO_PIN_4, RISE, std::bind(&BrakeController::digitalInCb2, this, _1), GPIO_PULLDOWN);
}

BrakeController::~BrakeController() {

}

void BrakeController::run(void *argument) {
	can2->subscribe(ID_FB7POSVEL, std::bind(&BrakeController::canLed2Cb, this, _1));

	//Thread *t1 = new Thread(std::bind(&BrakeController::thread1, this, _1), NULL, "thread1", 512, osPriorityHigh);
	//Thread *t2 = new Thread(std::bind(&BrakeController::thread2, this, _1), NULL, "thread2", 512, osPriorityHigh);
	//t1->join();
	//t2->join();

	uint8_t data[8] = {ASSIST, 12, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	FB9StateMsg msg(data);
	while(1) {
		can2->send(msg);
		led1->toggle();
		osDelay(period_ms);
	}
}

void BrakeController::thread1(void *argument) {
	uint8_t data[8] = "thread1";
	//CanMsg msg(0x411, data);
	while(1) {
		//can2->send(msg);
		led1->toggle();
		osDelay(period_ms);
	}
}

void BrakeController::thread2(void *argument) {
	uint8_t data[8] = "thread2";
	//CanMsg msg(0x411, data);
	while(1) {
		//can2->send(msg);
		led3->toggle();;
		osDelay(period_ms/2);
	}
}

void BrakeController::canLed2Cb(CanMsg &msg) {
	FB7PosVelMsg posVelMsg(msg);

	if(posVelMsg.position == 0x037d007d)
		led2->toggle();
}

void BrakeController::digitalInCb1(uint8_t value) {
	if(dIn1->read() == 1)
		led3->set();

	//uint8_t pinVal = dIn2->read();
}

void BrakeController::digitalInCb2(uint8_t value) {
	if(dIn2->read() == 1)
		led3->reset();
}
