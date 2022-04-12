/*
 * Throttle.cpp
 *
 *  Created on: Mar 9, 2022
 *      Author: igvcsp2022
 */

#include "Throttle.h"
#include "CAN.h"
#include "main.h"
#include "initDevice.h"
DAC_HandleTypeDef hdacThrottle;  //dac

Throttle::Throttle() {
	MX_DAC_Init();
}

/*
1023 <=> 0.825V
2048 <=> 1.65V
3071 <=> 2.475V
4095 <=> 3.3V
*/
void Throttle::setThrottle(uint16_t throttlePostion){

	float voltThrottle1  = 0.5380 + (0.01551 *  throttlePostion);
	float voltThrottle2  = 1.0820 + (0.03058 *  throttlePostion);
	//float voltThrottle2  = 1.0820 + (0.03058 *  throttlePostion);

	float Throttle1 = voltThrottle1 * 1263.94052;
	float Throttle2 = voltThrottle2 * 1263.94052;

    HAL_DAC_SetValue(&hdacThrottle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Throttle1);  // pin by name pin1
    HAL_DAC_SetValue(&hdacThrottle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, Throttle2);  // pin by name pin2

    HAL_DAC_Start(&hdacThrottle,DAC_CHANNEL_1);
    HAL_DAC_Start(&hdacThrottle,DAC_CHANNEL_2);
}


void Throttle::MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdacThrottle.Instance = DAC;
  if (HAL_DAC_Init(&hdacThrottle) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdacThrottle, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdacThrottle, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}
