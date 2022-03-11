/*
 * FNRSwitch.cpp
 *
 *  Created on: Mar 11, 2022
 *      Author: igvcsp2022
 */

#include <FNRSwitch.h>
#include "GearPosition.h"


FNRSwitch::FNRSwitch(const char *name) : Thread<FNRSwitch>(this, name) {
	// TODO Auto-generated constructor stub

}

FNRSwitch::~FNRSwitch() {
	// TODO Auto-generated destructor stub
}


void FNRSwitch::run(void *argument) {
	GearPosition gearSelect(GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_3);


	while(1) {
		gearSelect.set(Gear::FORWARD);
		osDelay(100);
	}
}
