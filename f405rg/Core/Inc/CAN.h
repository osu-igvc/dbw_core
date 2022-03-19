
/*
 * CAN.h
 *
 *  Created on: Mar 5, 2022
 *      Author: igvcsp2022
 */

#ifndef SRC_CAN_H_
#define SRC_CAN_H_

#include "stm32f4xx_hal.h"

#include <unordered_map>
#include <queue>

#include "main.h"


typedef struct {
	CAN_RxHeaderTypeDef header;
	uint8_t data[8];
} CanMsg;


typedef void (*CANIRQCb)(CanMsg *msg);

class CAN {
public:
	CAN(CAN_TypeDef* base, uint16_t queueSize=10);
	CAN(CAN_TypeDef* base, CANIRQCb cb, uint16_t queueSize=10);
	virtual ~CAN();

	int send(uint16_t id, uint8_t data[8]);
	int read(CanMsg *msg);
	void subscribe(uint16_t id, CANIRQCb cb);

	bool isAvailable();

	void __fifo0MsgPendingIrq();

	static std::unordered_map<CAN_HandleTypeDef*, CAN*> objectMap;


private:
	void init(CAN_TypeDef* base, uint16_t queueSize);

	CANIRQCb cb;

	CAN_HandleTypeDef *handle;


	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	CAN_FilterTypeDef canfilterconfig;

	std::queue<CanMsg> rxBuffer;
	uint16_t queueSize;

	std::unordered_map<uint16_t, CANIRQCb> subscriptions;
};

#endif /* SRC_CAN_H_ */
