
/*
 * DigitalIn.h
 *
 *  Created on: Jan 20, 2022
 *      Author: 14055
 */

#ifndef DIGITALIN_H_
#define DIGITALIN_H_

#include "stm32f4xx_hal.h"

#include <map>

typedef void (*digitalInIQRCb)(uint8_t pinValue);



class DigitalIn {
public:
	DigitalIn(GPIO_TypeDef* port, uint16_t pin, uint32_t interruptMode, digitalInIQRCb cb, uint32_t pullMode = GPIO_NOPULL, uint32_t speed = GPIO_SPEED_FREQ_LOW);
	DigitalIn(GPIO_TypeDef* port, uint16_t pin, uint32_t pullMode = GPIO_NOPULL, uint32_t speed = GPIO_SPEED_FREQ_LOW);
	virtual ~DigitalIn();

	uint8_t read(void);
	void __EXTI_Interrupt_CB(void);

	bool operator == (const int &val);
	bool operator == (DigitalIn &obj);
	bool operator != (const int &val);
	bool operator != (DigitalIn &obj);


	static std::map<uint16_t, DigitalIn*> objectMap;


private:
	digitalInIQRCb cb;

	GPIO_TypeDef *port;
	uint16_t pin;

	void Error_Handler();
};

#endif /* DIGITALIN_H_ */
