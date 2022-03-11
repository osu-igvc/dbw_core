/*
 * FNRSwitch.h
 *
 *  Created on: Mar 11, 2022
 *      Author: igvcsp2022
 */

#ifndef INC_FNRSWITCH_H_
#define INC_FNRSWITCH_H_

#include <Thread.h>

#include "DigitalIn.h"
#include "DigitalOut.h"

class FNRSwitch: public Thread<FNRSwitch> {
public:
	FNRSwitch(const char *name);
	virtual ~FNRSwitch();

	void run(void *argument);
};

#endif /* INC_FNRSWITCH_H_ */
