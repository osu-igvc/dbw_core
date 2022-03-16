/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

//#include "cmsis_os.h"
#include "usb_device.h"

#include "initDevice.h"

#include "CAN.h"


#define BOARD 2

#if BOARD == 1
uint16_t CANID = 0x411;
#elif BOARD == 2
uint16_t CANID = 0x412;
#else
uint16_t CANID = 0x400;
#endif

/*
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
*/



void StartDefaultTask(void *argument);
void BlinkLD2Task(void *argument);

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  initDevice();
  MX_USB_DEVICE_Init();

  //osKernelInitialize();

  //defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  //osKernelStart();


  CAN can2(&hcan2, CAN2, CANID, 10, LD1_GPIO_Port, LD1_Pin);

  DigitalOut led2(LD2_GPIO_Port, LD2_Pin);
  DigitalOut led3(LD3_GPIO_Port, LD3_Pin);

  uint8_t data[8] = "hello!";
  // Should never get here
  while (1)
  {
	can2.send(data, 8);


	bool avail = can2.isAvailable();
	if(avail) {
		CanMsg msg;
		can2.read(&msg);
		if(msg.header.StdId == 0x411) led2 = !led2;
	}

	led3 = !led3;
	HAL_Delay(500);
  }
}



/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  MX_USB_DEVICE_Init();

  CAN can2(&hcan2, CAN2, CANID, 10, LD1_GPIO_Port, LD1_Pin);

  DigitalOut led2(LD2_GPIO_Port, LD2_Pin);
  DigitalOut led3(LD3_GPIO_Port, LD3_Pin);

  uint8_t data[8] = "hello!";

  for(;;)
  {
	//can2.send(data, 8);


	bool avail = false;//can2.isAvailable();
	if(avail) {
		CanMsg msg;
		can2.read(&msg);
		if(msg.header.StdId == 0x411) led2 = !led2;
	}

	led3 = !led3;
	//osDelay(500);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
