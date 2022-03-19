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

#include "usb_device.h"
#include "cmsis_os.h"

#include "initDevice.h"
#include "dbw_polaris_can/dispatch.h"

#include "DashController.h"
#include "Thread.h"

#include "CAN.h"
#include "DigitalOut.h"
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

void canCb(CanMsg *msg);
void dash(void *args);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  initDevice();
  MX_USB_DEVICE_Init();

  osKernelInitialize();


  BoardType boardType = BoardType::DASH_BOARD;

  Thread *applicationThread;

  switch(boardType) {
  case BRAKE_BOARD:

	  break;
  case DASH_BOARD:
	  applicationThread = new DashController("DashController", 500);
	  break;
  case LOW_LEVEL_CONTROLLER:

	  break;
  case DUAL_CAN:

	  break;
  }

  osKernelStart();
  Error_Handler();	// Should never get to this line
}
