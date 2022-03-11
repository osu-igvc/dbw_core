/*
 * EmergencyBrake.h
 *
 *  Created on: Mar 11, 2022
 *      Author: igvcsp2022
 */

#ifndef INC_EMERGENCYBRAKE_H_
#define INC_EMERGENCYBRAKE_H_

#include <Thread.h>

#include "DigitalOut.h"
#include "stm32f4xx_hal.h"

class EmergencyBrake: public Thread<EmergencyBrake> {
public:
	EmergencyBrake(const char* name);
	virtual ~EmergencyBrake();

	void run(void *argument);
};

#endif /* INC_EMERGENCYBRAKE_H_ */
