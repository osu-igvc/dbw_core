/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define brake_dir_Pin GPIO_PIN_3
#define brake_dir_GPIO_Port GPIOC
#define parking_dir_Pin GPIO_PIN_0
#define parking_dir_GPIO_Port GPIOA
#define parking_pwm_Pin GPIO_PIN_1
#define parking_pwm_GPIO_Port GPIOA
#define brake_pwm_Pin GPIO_PIN_2
#define brake_pwm_GPIO_Port GPIOA
#define stm_relay_Pin GPIO_PIN_6
#define stm_relay_GPIO_Port GPIOA
#define brake_ls_reverse_Pin GPIO_PIN_4
#define brake_ls_reverse_GPIO_Port GPIOC
#define brake_ls_forward_Pin GPIO_PIN_5
#define brake_ls_forward_GPIO_Port GPIOC
#define park_ls_forward_Pin GPIO_PIN_0
#define park_ls_forward_GPIO_Port GPIOB
#define park_ls_reverse_Pin GPIO_PIN_1
#define park_ls_reverse_GPIO_Port GPIOB
#define ethernet_led_1_Pin GPIO_PIN_2
#define ethernet_led_1_GPIO_Port GPIOB
#define ethernet_led_2_Pin GPIO_PIN_10
#define ethernet_led_2_GPIO_Port GPIOB
#define quad_ch_2_Pin GPIO_PIN_15
#define quad_ch_2_GPIO_Port GPIOB
#define quad_ch_1_Pin GPIO_PIN_6
#define quad_ch_1_GPIO_Port GPIOC
#define extra_conn_Pin GPIO_PIN_7
#define extra_conn_GPIO_Port GPIOC
#define EStop_Pin GPIO_PIN_8
#define EStop_GPIO_Port GPIOC
#define Status_3_Pin GPIO_PIN_4
#define Status_3_GPIO_Port GPIOB
#define Status_2_Pin GPIO_PIN_5
#define Status_2_GPIO_Port GPIOB
#define Status_1_Pin GPIO_PIN_6
#define Status_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
