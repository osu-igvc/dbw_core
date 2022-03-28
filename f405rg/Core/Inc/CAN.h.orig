
/*
 * CAN.h
 *
 *  Created on: Mar 5, 2022
 *      Author: igvcsp2022
 */

#ifndef SRC_CAN_H_
#define SRC_CAN_H_

#include "stm32f4xx_hal.h"
#include "DigitalOut.h"
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOB

#include <map>
#include <queue>


struct CanMsg {
	CAN_RxHeaderTypeDef header;
	uint8_t data[8];
};


class CAN {
public:
	CAN(CAN_HandleTypeDef *handle, CAN_TypeDef* base, uint16_t id, uint16_t queueSize=10, GPIO_TypeDef* port = GPIOB, uint16_t = GPIO_PIN_5);
	virtual ~CAN();

	int send(uint8_t *data, uint8_t numBytes);
	CanMsg read();
	bool isAvailable();

	void __fifo0MsgPendingIrq();

	DigitalOut led;

	static std::map<CAN_HandleTypeDef*, CAN*> objectMap;


private:
	void Error_Handler(void);

	CAN_HandleTypeDef *handle;


	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	CAN_FilterTypeDef canfilterconfig;

	std::queue<CanMsg> rxBuffer;
	uint16_t queueSize;
};

#endif /* SRC_CAN_H_ */
