/*
 * PWM.cpp
 *
 *  Created on: Mar 17, 2022
 *      Author: colli
 */

#include "PWM.h"
#include "main.h"

TIM_HandleTypeDef htim1, htim2, htim3, htim4, htim5;
TIM_HandleTypeDef htim8, htim9, htim10, htim11, htim12, htim13, htim14;

PWM::PWM(TIM_TypeDef* instance, uint32_t channel, uint16_t freq) : ThreadSafe() {
	init(instance, channel, freq);
}

PWM::PWM(TIM_TypeDef* instance, uint32_t channel, uint16_t freq, float dutyCycle) : ThreadSafe() {
	init(instance, channel, freq);
	set(freq, dutyCycle);
}

PWM::~PWM() {
	HAL_TIM_PWM_Stop(handle, channel);
}


void PWM::init(TIM_TypeDef* instance, uint32_t channel, uint16_t freq) {
	if(instance == TIM1) handle = &htim1;
	else if (instance == TIM2) handle = &htim2;
	else if (instance == TIM3) handle = &htim3;
	else if (instance == TIM4) handle = &htim4;
	else if (instance == TIM5) handle = &htim5;
	else if (instance == TIM8) handle = &htim8;
	else if (instance == TIM9) handle = &htim9;
	else if (instance == TIM10) handle = &htim10;
	else if (instance == TIM11) handle = &htim11;
	else if (instance == TIM12) handle = &htim12;
	else if (instance == TIM13) handle = &htim13;
	else handle = &htim14;

	this->channel = channel;

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	sConfigOC = {0};

	handle->Instance = instance;
	handle->Init.Prescaler = 0;
	handle->Init.CounterMode = TIM_COUNTERMODE_UP;
	handle->Init.Period = SystemCoreClock/(2*freq);
	handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(handle) != HAL_OK)
	{
	Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(handle, &sMasterConfig) != HAL_OK)
	{
	Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(handle, &sConfigOC, channel) != HAL_OK)
	{
	Error_Handler();
	}


	HAL_TIM_MspPostInit(handle);
	return;
}

void PWM::set(float dutyCycle) {
	updateDutyCycle(dutyCycle);
	return;
}


void PWM::set(uint16_t freq, float dutyCycle) {
	handle->Init.Period = SystemCoreClock/(2*freq);

	if (HAL_TIM_PWM_Init(handle) != HAL_OK)
		Error_Handler();

	updateDutyCycle(dutyCycle);
	return;
}

void PWM::updateDutyCycle(float dutyCycle) {
	dutyCycle = (dutyCycle > 1) ? 1 : dutyCycle;
	dutyCycle = (dutyCycle < 0) ? 0 : dutyCycle;

	sConfigOC.Pulse = (handle->Init.Period+1)*dutyCycle;

	HAL_TIM_PWM_Stop(handle, channel);
	HAL_TIM_PWM_ConfigChannel(handle, &sConfigOC, channel);
	HAL_TIM_PWM_Start(handle, channel);
	return;
}


void PWM::operator = (const float &val) {
	set(val);
	return;
}
