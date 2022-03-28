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
Thread(std::bind(&DashController::run, this, _1), NULL, name, stack_size), led1(TIM3, TIM_CHANNEL_1), led2(LD2_GPIO_Port, LD2_Pin), led3(LD3_GPIO_Port, LD3_Pin) {
	this->period_ms = period_ms;
	can2 = new CAN("CAN2", CAN2);
}

DashController::~DashController() {

}

void DashController::run(void *argument) {
	  can2->subscribe(0x411, std::bind(&DashController::canLed2Cb, this, _1));
	  can2->createPublisher(0x412);

	  uint8_t data[8] = "hello!";

	  int incr = 0;

	  while (1)
	  {
		//can2->send(0x412, data);

		led1 = (incr++ % 11)/10.0;

		led3 = !led3;
		osDelay(period_ms);
	  }
}

void DashController::canLed2Cb(const CanMsg &msg) {
	if(strcmp((char*)msg.data, "thread1") == 0) 	 led2 = !led2;
	else if(strcmp((char*)msg.data, "thread2") == 0) led2 = 1;
}

