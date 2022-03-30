
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

	virtual uint8_t* toArray();

	CAN_RxHeaderTypeDef rxHeader;
	CAN_TxHeaderTypeDef txHeader;
	uint32_t id;
	uint8_t data[8];
};



typedef enum {
	ID_FB3TORQUE = 0x18ff6713,
	ID_FB7POSVEL = 0x18ecff13,
	ID_FB9STATE  = 0x18ebff13,
	ID_CMD,
	ID_ESTOP,
	ID_CCVS,
	ID_EEC1
} EPS_MSG_ID;

typedef enum {
	OFF 		= 0x00,
	CURRENT  	= 0x01,
	ASSIST 		= 0x02,
	VELOCITY 	= 0x03,
	POSITION 	= 0x04,
	POS_VEL 	= 0x05,
	OFFSET 		= 0x10
} EPS_MODE;

class J1939Msg : public CanMsg {
public:
	J1939Msg();
	J1939Msg(uint32_t id, uint8_t data[8]);
	J1939Msg(CanMsg &msg);

	void fillHeader(uint32_t id);
	uint64_t getDatum(uint8_t data[8]);
	void setData(uint64_t datum);

	virtual void fillMsg(uint8_t data[8]) = 0;

	uint8_t priority   :3;
	uint8_t reserved   :1;
	uint8_t dataPage   :1;
	uint8_t pduFormat  :8;
	uint8_t pduSpec    :8;
	uint8_t sourceAddr :8;
};

class FB3TorqueMsg : public J1939Msg {
public:
	FB3TorqueMsg();
	FB3TorqueMsg(uint8_t data[8]);
	FB3TorqueMsg(CanMsg &msg);

	void fillMsg(uint8_t data[8]);

	uint32_t inputTorque :32;
	uint32_t cmdTorque   :32;
};

class FB7PosVelMsg : public J1939Msg {
public:
	FB7PosVelMsg();
	FB7PosVelMsg(uint8_t data[8]);
	FB7PosVelMsg(CanMsg &msg);

	void fillMsg(uint8_t data[8]);

	uint32_t position :32;
	uint32_t velocity :32;
};

class FB9StateMsg : public J1939Msg {
public:
	FB9StateMsg();
	FB9StateMsg(uint8_t data[8]);
	FB9StateMsg(CanMsg &msg);

	void fillMsg(uint8_t data[8]);

	uint64_t mode 	  :8;
	uint64_t msgCount :8;
	uint64_t 		  :48;
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

	int subscribe(uint32_t id, CANIrqCb cb);

	bool isAvailable(uint32_t id);

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
	std::map<uint32_t, CANIrqCb> subscriptions;

	osSemaphoreId_t mutex;
};

#endif /* SRC_CAN_H_ */
