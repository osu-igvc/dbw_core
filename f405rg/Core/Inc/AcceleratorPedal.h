/*
 * AcceleratorPedal.h
 *
 *  Created on: Mar 11, 2022
 *      Author: igvcsp2022
 */

#ifndef INC_ACCELERATORPEDAL_H_
#define INC_ACCELERATORPEDAL_H_

#include <Thread.h>

class AcceleratorPedal: public Thread<AcceleratorPedal> {
public:
	AcceleratorPedal(const char *name);
	virtual ~AcceleratorPedal();

	void run(void *argument);
};

#endif /* INC_ACCELERATORPEDAL_H_ */
