
/*
 * QuadEnc.cpp
 *
 *  Created on: March 25, 2022
 *      Author: Tyler Tucker
 */

#include "QuadEnc.h"

QuadEnc::QuadEnc() {
  HAL_TIM_IC_MspInit();
  HAL_RCC_TIM8_CLK_ENABLE();
  HAL_RCC_GPIO8_CLK_ENABLE();
  HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);   // main channel
  HAL_TIM_IC_Start(&htim8, TIM_CHANNEL_2);   // indirect channel

  HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_1);   // main channel
  HAL_TIM_IC_Start(&htim12, TIM_CHANNEL_2);   // indirect channel

  this->currentPos = 0;
  this->previousPos = 0;
  memset(this->positions, 0);
}

QuadEnc::~QuadEnc() {

}

uint16_t QuadEnc::getCount() {
	return
}

void QuadEnc::resetCount(){
  this->currentPos = 0;
  this->previousPos = 0;
}

void QuadEnc::setCount(uint16_t count){
  this->currentPos = count;
  this->previousPos = count;
}

uint16_t QuadEnc::getSpeed(){
  uint16_t speed = 0;
  for(int i = 0; i < 10; i++){
    speed += positions[0][i];
  }

  speed = speed / (positions[1][0] + positions[1][10])
  return speed;
}
