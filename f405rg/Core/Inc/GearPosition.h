/*
 * gearPosition.h
 *
 *  Created on: Mar 10, 2022
 *      Author: igvcsp2022
 */

#ifndef GEARPOSITION_H_
#define GEARPOSITION_H_

#include "stm32f4xx_hal.h"
#include "DigitalOut.h"

typedef enum {
	FORWARD,
	NEUTRAL,
	REVERSE
} Gear;

class GearPosition {
public:
	GearPosition(GPIO_TypeDef* s1Port, uint16_t s1Pin, GPIO_TypeDef* s2Port, uint16_t s2Pin);
	void setGear(Gear gearSpot);
	Gear getGearPosistion();

private:
	Gear gear;

	DigitalOut s1;
	DigitalOut s2;
};

#endif /* GEARPOSITION_H_ */
