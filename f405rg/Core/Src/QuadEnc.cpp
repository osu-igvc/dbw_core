
/*
 * QuadEnc.cpp
 *
 *  Created on: March 25, 2022
 *      Author: Tyler Tucker
 */

#include "QuadEnc.h"
#include "DigitalIn.cpp"

QuadEnc::QuadEnc(DigitalIn &ch1, DigitalIn &ch2) {
  this->ch1 = ch1;
  this->ch2 = ch2;
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
  if(count != this->currentPos){
    this->previousPos = this->currentPos;
    this->currentPos = count;
  }
}

uint16_t QuadEnc::getSpeed(){
  uint16_t speed = 0;
  for(int i = 0; i < 10; i++){
    speed += positions[0][i];
  }

  speed = speed / (positions[1][0] + positions[1][10])
  return speed;
}
