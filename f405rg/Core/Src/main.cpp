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
#include <CAN.h>
#include "main.h"

#include "usb_device.h"
#include "cmsis_os.h"

#include "initDevice.h"
#include "dbw_polaris_can/dispatch.h"

#include "DashController.h"
#include "BrakeController.h"
#include "Thread.h"

#include "DigitalOut.h"
#include "ParamServer.h"
#include "PWM.h"

#include <functional>

typedef enum {
	BRAKE_BOARD,
	DASH_BOARD,
	LOW_LEVEL_CONTROLLER,
	DUAL_CAN
} BoardType;

using namespace dbw_polaris_can;


/*
 * Network layout
 * 	Each board assigned a META message
 * 		Used as a heartbeat
 * 		Contains <BOARD_TYPE; TIME; >
 */


/**
  * @brief  The application entry point.
  * @retval intd
  */
int main(void)
{
  HAL_Init();
  initDevice();
  MX_USB_DEVICE_Init();



  osKernelInitialize();


  BoardType boardType = BoardType::BRAKE_BOARD;

  Thread *applicationThread;

  if(boardType == BRAKE_BOARD) {
	  applicationThread = new BrakeController("BrakeController", 200, 1024);
  }
  else if(boardType == DASH_BOARD) {
	  PWM 		 *led1 = new PWM(TIM3, TIM_CHANNEL_1);
	  DigitalOut *led2 = new DigitalOut(LD2_GPIO_Port, LD2_Pin);
	  DigitalOut *led3 = new DigitalOut(LD3_GPIO_Port, LD3_Pin);
	  CAN 		 *can2 = new CAN(CAN2, 2);

	  applicationThread = new DashController("DashController", 100, led1, led2, led3, can2, 1024);
  }
  else if(boardType == LOW_LEVEL_CONTROLLER) {}
  else if(boardType == DUAL_CAN) {}
  else
	  Error_Handler();

  osKernelStart();
  Error_Handler();	// Should never get to this line
}
