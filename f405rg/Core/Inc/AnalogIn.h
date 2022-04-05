/*
 * AnalogIn.h
 *
 *  Created on: Apr 1, 2022
 *      Author: colli
 */

#ifndef SRC_ANALOGIN_H_
#define SRC_ANALOGIN_H_

#include <ThreadSafe.h>

class AnalogIn: public ThreadSafe {
public:
	AnalogIn();
	virtual ~AnalogIn();
};

#endif /* SRC_ANALOGIN_H_ */
