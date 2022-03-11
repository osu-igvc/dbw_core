/*
 * gearPosition.cpp
 *
 *  Created on: Mar 10, 2022
 *      Author: igvcsp2022
 */

#include <GearPosition.h>


GearPosition::GearPosition(GPIO_TypeDef* s1Port, uint16_t s1Pin, GPIO_TypeDef* s2Port, uint16_t s2Pin) :
		s1(s1Port, s1Pin), s2(s2Port, s2Pin) {
	gear = Gear::NEUTRAL;
	set(gear);
}

void GearPosition::set(Gear gearSpot){
	switch(gearSpot) {
	case Gear::FORWARD:
		s1 = 0;
		s2 = 0;
		break;
	case Gear::NEUTRAL:
		s1 = 0;
		s2 = 1;
		break;
	case Gear::REVERSE:
		s1 = 1;
		s2 = 0;
		break;
	}

	gear = gearSpot;
}


Gear GearPosition::getGearPosistion(){
	return gear;
}
