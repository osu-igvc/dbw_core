/*
 * DAC.cpp
 *
 *  Created on: Mar 31, 2022
 *      Author: colli
 */

#include <AnalogOut.h>

#include "main.h"

DAC_HandleTypeDef hdac;

AnalogOut::AnalogOut(AnalogPin pin) : ThreadSafe() {
	DAC_ChannelConfTypeDef sConfig = {0};

	switch(pin) {
	case PA4:
		handle = &hdac;
		channel = DAC_CHANNEL_1;
		break;
	case PA5:
		handle = &hdac;
		channel = DAC_CHANNEL_2;
		break;
	default:
		Error_Handler();
	}

	handle->Instance = DAC;

	if(HAL_DAC_Init(handle) != HAL_OK)
		Error_Handler();

	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

	if(HAL_DAC_ConfigChannel(handle, &sConfig, channel) != HAL_OK)
		Error_Handler();

	if(HAL_DAC_Start(handle, channel) != HAL_OK)
		Error_Handler();
}

AnalogOut::~AnalogOut() {
	if(HAL_DAC_Stop(handle, channel) != HAL_OK)
		Error_Handler();
}

void AnalogOut::write(uint16_t value) {
	value = (value > 4095) ? 4095 : value;
	value = (value < 0) ? 0 : value;

	if(HAL_DAC_SetValue(handle, channel, DAC_ALIGN_12B_R, value) != HAL_OK)
		Error_Handler();
}

void AnalogOut::write(float value) {
	value = (value > 3.3) ? 3.3 : value;
	value = (value < 0.0) ? 0.0 : value;

	uint16_t output = value*4096/3.3;

	if(HAL_DAC_SetValue(handle, channel, DAC_ALIGN_12B_R, output) != HAL_OK)
		Error_Handler();
}
