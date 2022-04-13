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

	eBrake  = new PWM(TIM5, TIM_CHANNEL_2, 200);
	brake = new PWM(TIM9, TIM_CHANNEL_1, 200);

	led1 = new DigitalOut(GPIOB, GPIO_PIN_4);
	led2 = new DigitalOut(LD2_GPIO_Port, LD2_Pin);
	led3 = new DigitalOut(LD3_GPIO_Port, LD3_Pin);
	brake_dir = new DigitalOut(GPIOA, GPIO_PIN_0);
	eBrake_dir = new DigitalOut(GPIOC, GPIO_PIN_3);

	can2 = new CAN(CAN2, 2);

	quad_ch1 = new DigitalIn(GPIOC, GPIO_PIN_6, RISE, std::bind(&BrakeController::QuadCb1, this, _1), GPIO_PULLDOWN);
	quad_ch2 = new DigitalIn(GPIOB, GPIO_PIN_15, RISE, GPIO_PULLDOWN);

	brake_forward_ls = new DigitalIn(GPIOC, GPIO_PIN_5, RISE, std::bind(&BrakeController::brake_forward_ls_Cb, this, _1), GPIO_PULLDOWN);
	brake_reverse_ls = new DigitalIn(GPIOC, GPIO_PIN_4, RISE, std::bind(&BrakeController::brake_reverse_ls_Cb, this, _1), GPIO_PULLDOWN);
	eBrake_forward_ls = new DigitalIn(GPIOB, GPIO_PIN_0, RISE, std::bind(&BrakeController::eBrake_forward_ls_Cb, this, _1), GPIO_PULLDOWN);
	eBrake_reverse_ls = new DigitalIn(GPIOB, GPIO_PIN_1, RISE, std::bind(&BrakeController::eBrake_reverse_ls_Cb, this, _1), GPIO_PULLDOWN);

	tim1.start();
	tim2.start();
	encTimer.start();
}

BrakeController::~BrakeController() {

}

void BrakeController::run(void *argument) {
	can2->subscribe(ID_FB7POSVEL, std::bind(&BrakeController::canLed2Cb, this, _1));

	//Thread *t1 = new Thread(std::bind(&BrakeController::thread1, this, _1), NULL, "thread1", 512, osPriorityHigh);
	//Thread *t2 = new Thread(std::bind(&BrakeController::thread2, this, _1), NULL, "thread2", 512, osPriorityHigh);
	//t1->join();
	//t2->join();

	brake->set(0.2);
	eBrake->set(0.72);
	brake_dir->set();



	// If brakeButtonRise
	// 	Accelerate to 15 mph over 5 seconds
	// 	Hold for 5 seconds
	//	Decelerate over 1 second


	//float mph = 0;

	uint8_t data[8] = {ASSIST, 12, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	FB9StateMsg msg(data);
	while(1) {
		//can2->send(msg);

		led1->toggle();
		led2->toggle();
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

/*
	Using the count from the encoder we can determine the speed of the motor.
	The relative speed (counts/millisecond) is then converted into a usable form (centimeters/second)
	These functions are public in order to be used in other classes.
	This class needs to be called fairly often in order to minimize error.
*/
float BrakeController::getBrakeSpeed(){
	float speed = (encoderCount-prevEncoderCount)/(encTimer.getElapsedTime());
	encTimer.restart();
	prevEncoderCount = encoderCount;
	encoderCount = 0;
	return speed;
}

/*
	In order to change the direction of either linear actuator for the brakes
	just call one of the functions below. In order to prevent damage to the motor
	controller or linear actuator the motor is brought to a stop before changing directions
*/
void BrakeController::changeBrakeDirection(){
	brake->set(0);
	brake_dir->toggle();
}

void BrakeController::changeEBrakeDirection(){
	eBrake->set(0);
	eBrake_dir->toggle();
}

void BrakeController::canLed2Cb(CanMsg &msg) {
	FB7PosVelMsg posVelMsg(msg);

	if(posVelMsg.position == 0x037d007d)
		led2->toggle();
}

void BrakeController::digitalInCb2(uint8_t value) {
	if(dIn2->read() != 0) return;

	uint32_t time = tim2.getElapsedTime();
	led3->reset();
	tim2.restart();
}

/*
	These Quad callbacks are for when the quadrature encoder has a
	rising edge on one of its channels. Whenever both
*/
void BrakeController::QuadCb1(uint8_t value) {
	if(value == 0) return;
	if(!quad_ch2->read())
		encoderCount++;
	else
		encoderCount--;

}


/*
	The below functions are for when any of the linear actuators hit a limit switch
	Whenever there is an interupt for the limit switch the linear actuator will
	automatically be cut off.
*/
void BrakeController::brake_forward_ls_Cb(uint8_t value) {
	if(value == 0) return;
	brake->set(0);
}

void BrakeController::brake_reverse_ls_Cb(uint8_t value) {
	if(value == 0) return;
	brake->set(0);
}

void BrakeController::eBrake_forward_ls_Cb(uint8_t value) {
	if(value == 0) return;
	eBrake->set(0);
}

void BrakeController::eBrake_reverse_ls_Cb(uint8_t value) {
	if(value == 0) return;
	eBrake->set(0);
}
