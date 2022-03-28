
/*
 * CAN.h
 *
 *  Created on: Mar 5, 2022
 *      Author: igvcsp2022
 */

#ifndef SRC_CAN_H_
#define SRC_CAN_H_

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include <map>
#include <queue>
#include <functional>

#include "main.h"
#include "Thread.h"



class CanMsg {
public:
	CanMsg();
	CanMsg(uint32_t idType);
	CanMsg(uint32_t id, uint8_t data[8]);
	CanMsg(uint32_t id, uint32_t idType, uint8_t data[8]);

	CAN_RxHeaderTypeDef rxHeader;
	CAN_TxHeaderTypeDef txHeader;
	uint32_t id;
	uint8_t data[8];
};


typedef std::function<void(CanMsg&)> CANIrqCb;
typedef std::function<void(CanMsg*)> CANIrqGlobalCb;

class CAN {
public:
	/**
	 * @brief	Initialize a CAN instance
	 * @param	name name of the thread
	 * @param 	base must be CAN1 or CAN2. only one instance of each base
	 * 			is allowed
	 * @param	queueSize maximum size of the rX queues
	 * @param	stack_size maximum size of the thread stack
	 */
	CAN(CAN_TypeDef* base, uint16_t queueSize);

	/**
	 * @brief 	Initialize a CAN instance where ALL received messages trigger
	 * 			the interrupt ISR cb
	 * @param 	name name of the thread
	 * @param 	base must be CAN1 or CAN2. only one instance of each base
	 * 			is allowed
	 * @param 	cb ISR handler triggered by ALL received messages
	 * @param	queueSize maximum size of the rX queues
	 * @param 	stack_size maximum size of the thread stack
	 */
	CAN(CAN_TypeDef* base, CANIrqGlobalCb cb, uint16_t queueSize);
	virtual ~CAN();

	/**
	 * @brief add a new message to the transmit queue
	 * @param	id ID of the CAN message
	 * @param	data data to send
	 */
	int send(CanMsg &msg);

	/**
	 * @brief get a message from a subscribed id. timeout must be 0 if in ISR
	 * @param	id specific id for which to look
	 * @param	msg buffer for the received message
	 * @param	timeout integer ranging from 0 to osWaitForever
	 */
	int read(CanMsg &msg, uint32_t timeout=0);

	/**
	 * @brief create a new message queue for a specific CAN id
	 * @param	id specific id for which the subscription should be made
	 * @param	cb optional ISR function
	 */

	int subscribe(uint16_t id, CANIrqCb cb);

	bool isAvailable(uint16_t id);

	void __fifo0MsgPendingIrq(CanMsg &msg);

private:
	void init(CAN_TypeDef* base, uint16_t queueSize);
	void lock();
	void unlock();

	CAN_HandleTypeDef *handle;

	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	CAN_FilterTypeDef canfilterconfig;

	CANIrqGlobalCb cb;

	uint16_t queueSize;
	std::queue<CanMsg> queue;
	std::map<uint16_t, CANIrqCb> subscriptions;

	osMutexId_t mutex;
};

#endif /* SRC_CAN_H_ */
