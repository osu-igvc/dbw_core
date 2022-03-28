
/*
 * QuadEnc.cpp
 *
 *  Created on: March 25, 2022
 *      Author: Tyler Tucker
 */

#include "QuadEnc.h"

QuadEnc::QuadEnc(DigitalIn ch1, DigitalIn ch2) {

	this->ch1 = ch1;
	this->ch2 = ch2;
  this->currentPos = 0;
  this->previousPos = 0;
}

QuadEnc::~QuadEnc() {

}

uint16_t QuadEnc::getPos() {
	return
}

void QuadEnc::resetPos(){
  this->currentPos = 0;
  this->previousPos = 0;
}

void QuadEnc::incrementPos(){
  this->currentPos = 0;
  this->previousPos = 0;
}
