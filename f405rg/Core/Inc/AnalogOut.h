/*
 * DAC.h
 *
 *  Created on: Mar 31, 2022
 *      Author: colli
 */

#ifndef SRC_DAC_H_
#define SRC_DAC_H_

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "ThreadSafe.h"


typedef enum {
	PA4,
	PA5
} AnalogPin;

class AnalogOut : ThreadSafe {
public:
	AnalogOut(AnalogPin pin);
	virtual ~AnalogOut();

	void write(uint16_t value);
	void write(float value);

private:
	DAC_HandleTypeDef *handle;
	uint32_t channel;
};

#endif /* SRC_DAC_H_ */
