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


BrakeController::BrakeController(const char *name, int period_ms, CAN *can2, uint32_t stack_size) :
Thread(std::bind(&BrakeController::run, this, _1), NULL, name, stack_size), led1(LD1_GPIO_Port, LD1_Pin), led2(LD2_GPIO_Port, LD2_Pin), led3(LD3_GPIO_Port, LD3_Pin) {
	this->period_ms = period_ms;
	this->can2 = can2;
}

BrakeController::~BrakeController() {

}

void BrakeController::run(void *argument) {
	can2->subscribe(0x412, std::bind(&BrakeController::canLed2Cb, this));
	can2->createPublisher(0x411);
	uint8_t data[8] = "thread1";

	while(1) {
		can2->send(0x411,data);
		led2 = !led2;
		osDelay(period_ms/2);
	}

	Thread *t1 = new Thread(std::bind(&BrakeController::thread1, this, _1), NULL, "thread1", 512, osPriorityHigh);
	Thread *t2 = new Thread(std::bind(&BrakeController::thread2, this, _1), NULL, "thread2", 512, osPriorityHigh);
	t1->join();
	t2->join();
}

void BrakeController::thread1(void *argument) {
	uint8_t data[8] = "thread1";
	can2->createPublisher(0x411);
	while(1) {
		can2->send(0x411, data);
		led1 = !led3;
		osDelay(period_ms);
	}
}

void BrakeController::thread2(void *argument) {
	uint8_t data[8] = "thread2";
	can2->createPublisher(0x411);
	while(1) {
		can2->send(0x412, data);
		led3 = !led3;
		osDelay(period_ms/2);
	}
}

void BrakeController::canLed2Cb() {
	CanMsg msg;
	can2->read(0x411, &msg, 0);
	led2 = !led2;
}
