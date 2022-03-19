
/*
 * CAN.h
 *
 *  Created on: Mar 5, 2022
 *      Author: igvcsp2022
 */

#ifndef SRC_CAN_H_
#define SRC_CAN_H_

#include "stm32f4xx_hal.h"

#include <map>
#include <queue>
#include <functional>

#include "main.h"


typedef struct {
	CAN_RxHeaderTypeDef header;
	uint8_t data[8];
} CanMsg;


typedef std::function<void(CanMsg*)> CANIrqCb;

class CAN {
public:
	CAN(CAN_TypeDef* base, uint16_t queueSize=10);
	CAN(CAN_TypeDef* base, CANIrqCb cb, uint16_t queueSize=10);
	virtual ~CAN();

	int send(uint16_t id, uint8_t data[8]);
	int read(CanMsg *msg);
	//template<class T> void subscribe(uint16_t id, void (T::*)(CanMsg *msg) cb, T *obj);
	void subscribe(uint16_t id, CANIrqCb cb);

	bool isAvailable();

	void __fifo0MsgPendingIrq();

private:
	void init(CAN_TypeDef* base, uint16_t queueSize);

	CANIrqCb cb;

	CAN_HandleTypeDef *handle;


	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	CAN_FilterTypeDef canfilterconfig;

	std::queue<CanMsg> rxBuffer;
	uint16_t queueSize;

	std::map<uint16_t, CANIrqCb> subscriptions;
};

#endif /* SRC_CAN_H_ */
