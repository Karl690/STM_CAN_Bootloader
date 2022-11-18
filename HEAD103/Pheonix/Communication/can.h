#pragma once
#include "configure.h"
#ifdef HH103
#include "1xx/can_1xx.h"

#elif defined(ESR407)
#include "4xx/can_4xx.h"

#elif defined(PHEONIX750)
#include "7xx/can_7xx.h"
#endif

#include "hyrelcan.h"

typedef struct
{
	uint32_t ID;
	uint8_t Immediate;
	uint8_t TargetAddress;
	uint8_t SourceAddress;
	uint8_t MsgType;
	uint8_t MsgId;
	uint8_t Page;
	uint8_t Data[8];
	uint8_t DataSize;
}CANMsg;
////////CAN MESSAGE TYPE////////////////////
#define CAN_READ                    0b00        // 0x00
#define CAN_WRITE                   0b01        // 0x01

#define CAN_MSG_BUFFER_SIZE			0x0F

////////CAN MESSAGE ID////////////////////
#define CAN_MSG_BOOT				0x00
#define CAN_MSG_HEARTBEAT			0x01
#define CAN_MSG_PING				0x02

#define CAN_MSG_ERASE_SOAPSTRING	0x10
#define CAN_MSG_READ_SOAPSTRING		0x11
#define CAN_MSG_WRITE_SOAPSTRING	0x12

#define CAN_MSG_HSS_CONTROL			0x20

extern uint8_t 				RemoteSoapString[0x400];
extern uint16_t				CanRxInIndex;
extern uint16_t				CanRxOutIndex;
extern uint8_t				CanRxTargetId;
extern uint8_t				CanRxSourceId;
extern uint8_t 				CanTransmitMailbox;
extern uint32_t 			CanTrasmitMsgWaitCounter;
extern uint8_t 				CanMessageBuffer[]; //this is a buffer for CAN message transmitting.
extern uint16_t 			CanTxLedCountDown;
extern uint16_t 			CanRxLedCountDown;
extern CANMsg 				CanRxMsgBuffer[CAN_MSG_BUFFER_SIZE];
extern void CheckCanRxMessages(void);
extern void ProcessCanRxMessage(void);
extern void ProcessCanTxMessage(void);
extern void CanAddTxBuffer(uint16_t target, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate,  uint8_t* data, uint8_t size);
