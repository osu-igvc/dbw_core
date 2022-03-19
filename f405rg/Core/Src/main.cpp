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

#include "initDevice.h"
#include "dbw_polaris_can/dispatch.h"

#include "CAN.h"
#include "DigitalOut.h"
#include "PWM.h"

typedef enum {
	BRAKE_BOARD,
	DASH_BOARD,
	LOW_LEVEL_CONTROLLER,
	DUAL_CAN
} BoardType;

using namespace dbw_polaris_can;

#define BOARD DASH_BOARD
#if BOARD == DASH_BOARD
uint16_t CANID_TX = ID_STEERING_REPORT;
uint16_t CANID_RX = ID_STEERING_CMD;
uint32_t delayTime = 200;
#elif BOARD == BRAKE_BOARD
uint16_t CANID_TX = ID_BRAKE_REPORT;
uint16_t CANID_RX = ID_BRAKE_CMD;
uint32_t delayTime = 500;
#elif BOARD == LOW_LEVEL_CONTROLLER
uint16_t CANID_TX = 0x412;
uint16_t CANID_RX = 0x411;
uint32_t delayTime = 500;
#endif

/*
 * Network layout
 * 	Each board assigned a META message
 * 		Used as a heartbeat
 * 		Contains <BOARD_TYPE; TIME; >
 */


void canCb(CanMsg *msg);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  initDevice();
  MX_USB_DEVICE_Init();

  BoardType boardType = BoardType::DASH_BOARD;

  switch(boardType) {
  case BRAKE_BOARD:

	  break;
  case DASH_BOARD:

	  break;
  case LOW_LEVEL_CONTROLLER:

	  break;
  case DUAL_CAN:

	  break;
  }



  CAN can2(CAN2, 10);
  can2.subscribe(CANID_RX, canCb);

  PWM led1(TIM3, TIM_CHANNEL_1);

  DigitalOut led3(LD3_GPIO_Port, LD3_Pin);

  uint8_t data[8] = "hello!";

  int incr = 0;

  while (1)
  {
	can2.send(CANID_TX, data);

	led1 = (incr++ % 11)/10.0;

	led3 = !led3;
	HAL_Delay(delayTime);
  }
}

static DigitalOut led2(LD2_GPIO_Port, LD2_Pin);
void canCb(CanMsg *msg) {
	led2 = !led2;
}
