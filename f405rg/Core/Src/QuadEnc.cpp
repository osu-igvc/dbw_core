
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
  ch2Prev = ch2.read();
}

QuadEnc::~QuadEnc() {

}

digitalInIQRCb QuadEnc::ch1_cb(uint8_t value) {
  ch2Cur = ch2.read();
  if(ch2Cur > ch2Prev){
    count++;
    ch2Prev = ch2Cur;
  }else{
    count--;
    ch2Prev = ch2Cur;
  }
}


uint16_t QuadEnc::getCount() {
	return this->count;
}

void QuadEnc::resetCount(){
  this->count = 0;
}


uint16_t QuadEnc::getSpeed(){
  uint16_t speed = 0;
  for(int i = 0; i < 10; i++){
    speed += positions[0][i];
  }

  speed = speed / (positions[1][0] + positions[1][10])
  return speed;
}
