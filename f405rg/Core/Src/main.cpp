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

#include "cmsis_os.h"
#include "usb_device.h"

#include "initDevice.h"

#include "DigitalIn.h"
#include "GearPosition.h"
#include "BlinkThread.h"
#include "CAN.h"
#include "ParamServer.h"

#define BOARD 2

#if BOARD == 1
uint16_t CANID = 0x411;
#elif BOARD == 2
uint16_t CANID = 0x412;
#else
uint16_t CANID = 0x400;
#endif


osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};




void StartDefaultTask(void *argument);
void BlinkLD2Task(void *argument);

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue,VarDataTmp = 0;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  initDevice();

  HAL_FLASH_Unlock();
  if( EE_Init() != EE_OK)
  {
    Error_Handler();
  }

  osKernelInitialize();

  //BlinkThread* blinkLD2Task = new BlinkThread(LD2_GPIO_Port, LD2_Pin, 200, "blinkLD2Task");
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);



  osKernelStart();

  // Should never get here
  while (1)
  {

  }
}



DigitalOut led1(LD1_GPIO_Port, LD1_Pin);
void cb(uint8_t value) {
	led1 = !led1;
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

  DigitalOut led2(LD2_GPIO_Port, LD2_Pin);
  DigitalOut led3(LD3_GPIO_Port, LD3_Pin);


  ParamServer testEEprom(1,0);
  //testEEprom.write(0x010);

  for(;;)
  {


	  if(testEEprom.read(0)==0x010){
		  led3 = !led3;
	  }
	  else{
		  led2 = !led2;
	  }

	osDelay(500);
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
