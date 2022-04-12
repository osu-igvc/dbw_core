/*
 * Throttle.h
 *
 *  Created on: Mar 9, 2022
 *      Author: igvcsp2022
 */

#ifndef SRC_THROTTLE_H_
#define SRC_THROTTLE_H_

#include "main.h"
#include "CAN.h"

class Throttle {
public:
	Throttle();
	void setThrottle(uint16_t throttlePostion);
	uint16_t getThrottlePo(CAN *CanThrottle);
	uint16_t getWheelSpeed(CAN *CanThrottle);
	void MX_DAC_Init(); //dac

private:


};

#endif /* SRC_THROTTLE_H_ */
