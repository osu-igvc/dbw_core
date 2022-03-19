/*
 * PWM.h
 *
 *  Created on: Mar 17, 2022
 *      Author: colli
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#include "stm32f4xx_hal.h"


class PWM {
public:
	PWM(TIM_TypeDef* instance, uint32_t channel, uint16_t freq = 20000);
	PWM(TIM_TypeDef* instance, uint32_t channel, uint16_t freq, float dutyCycle);
	virtual ~PWM();

	void set(float dutyCycle);
	void set(uint16_t freq, float dutyCycle);

	void operator = (const float &val);

private:
	void init(TIM_TypeDef* instance, uint32_t channel, uint16_t freq);
	void updateDutyCycle(float dutyCycle);

	TIM_OC_InitTypeDef sConfigOC;

	TIM_HandleTypeDef *handle;
	uint32_t channel;
};

#endif /* SRC_PWM_H_ */
