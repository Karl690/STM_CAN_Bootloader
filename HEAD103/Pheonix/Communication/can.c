/*
 * can.c
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */

#include "configure.h"
#include "taskmanager.h"
#include "can.h"
#include "RCC/rcc.h"
#include "FLASH/flash.h"

uint8_t 				CanTransmitMailbox;
uint32_t 				CanTrasmitMsgWaitCounter = 0;
uint8_t					CanRxTargetId;
uint8_t					CanRxSourceId;
uint8_t					CanRxPage;
uint8_t					CanRxMsgType;
uint8_t					CanRxImmediateFlag = 0;
uint8_t 				CanMessageBuffer[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}; //this is a buffer for CAN message transmitting.


uint16_t				CanRxInIndex = 0;
uint16_t				CanRxOutIndex = 0;
CANMsg 					CanRxMsgBuffer[CAN_MSG_BUFFER_SIZE];
uint16_t				CanTxInIndex = 0;
uint16_t				CanTxOutIndex = 0;
CANMsg 					CanTxMsgBuffer[CAN_MSG_BUFFER_SIZE];

uint32_t GenerateFrameID(uint16_t target, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate)
{
	return (uint32_t)((immediate << 28) + (target << 20) + ((uint32_t)HeadPosition << 12) + (msgType << 11) + (msgId << 4) + page);
}

void CanAddTxBuffer(uint16_t target, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate,  uint8_t* data, uint8_t size)
{
	CANMsg* pOutBuffer = &CanTxMsgBuffer[CanTxOutIndex];
	pOutBuffer->ID = GenerateFrameID(target, msgType, msgId, page, immediate);

	if(data && size > 0) memcpy(pOutBuffer->Data, data, size);
	pOutBuffer->DataSize = size;

	pOutBuffer->Immediate = immediate;
	pOutBuffer->MsgType = msgType;
	pOutBuffer->MsgId = msgId;
	pOutBuffer->Page = page;
	pOutBuffer->SourceAddress = HeadPosition;
	pOutBuffer->TargetAddress = target;

	CanTxInIndex ++;
	if(CanTxInIndex >= CAN_MSG_BUFFER_SIZE) CanTxInIndex =0;
}

void ProcessCanRxMessage(void)
{
	if(CanRxInIndex == CanRxOutIndex) return;
	CANMsg* pOutBuffer = &CanRxMsgBuffer[CanRxOutIndex];
	uint16_t address = 0;
	switch(pOutBuffer->MsgType)
	{
	case CAN_WRITE:
		switch(pOutBuffer->MsgId)
		{
		case CAN_MSG_ERASE_SOAPSTRING:
			CanMsgProcessorCount = 0;
			memset(SoapString, 0, FLASH_SOAP_SIZE);
			CanMsgProcessorType = TASK_CAN_ERASE_SOAPSTRING;
			break;
		case CAN_MSG_WRITE_SOAPSTRING:
			address = (uint16_t)(pOutBuffer->Data[0] + (pOutBuffer->Data[1] << 8));
			address *= 6;
			for(uint8_t i = 2; i < pOutBuffer->DataSize; i ++)
			{
				SoapString[address + i -2] = pOutBuffer->Data[i];
				if(SoapString[address + i -2] == 0) {
					CanMsgProcessorCount = 0;
					CanMsgProcessorType = TASK_CAN_WRITE_SOAPSTRING; //Start the task to write SoapString to Flash 0x8007C00 address
					break;
				}
			}
			break;
		case CAN_MSG_HSS_CONTROL:
			switch(pOutBuffer->Page) {
			case SWITCH_CONTROL_BY_TEMP:
				DesiredTemperature = pOutBuffer->Data[0] + (pOutBuffer->Data[1] << 8);
				break;
			case SWITCH_CONTROL_BY_FAN:
				DesiredFanDutyCycle = pOutBuffer->Data[0] + (pOutBuffer->Data[1] << 8);
				break;
			case SWITCH_CONTROL_BY_FEEDRATE:
				DesiredFeedRate = pOutBuffer->Data[0] + (pOutBuffer->Data[1] << 8);
				break;
			}
			break;
		}

		break;
	case CAN_READ:
		switch(pOutBuffer->MsgId)
		{
		case CAN_MSG_READ_SOAPSTRING:
			//                   							Address                            Length                           Out buffer
			ReadFlashData(FLASH_SOAP_START_ADDRESS + BYTES2UINT32(&pOutBuffer->Data[0]), BYTES2UINT32(&pOutBuffer->Data[4]), SoapString); //Read the soap string from Flash
			CanMsgProcessorCount = 0;
			CanMsgProcessorType = TASK_CAN_READ_SOAPSTRING;
			break;
		case CAN_MSG_PING:
			CanAddTxBuffer(CAN_DEV_HOST, CAN_READ, CAN_MSG_PING, 0, 0, 0, 0);
			break;
		}
		break;
	}
	CanRxOutIndex ++;
	if(CanRxOutIndex >= CAN_MSG_BUFFER_SIZE) CanRxOutIndex =0;
}
void ProcessCanTxMessage(void)
{
	if(CanTxInIndex == CanTxOutIndex) return;
	CANMsg* pOutBuffer = &CanTxMsgBuffer[CanTxOutIndex];
	if(SendCanMessage(pOutBuffer->ID, pOutBuffer->Data, pOutBuffer->DataSize) == SUCCESS) {
		CanTxOutIndex ++;
		if(CanTxOutIndex >= CAN_MSG_BUFFER_SIZE) CanTxOutIndex = 0;
	}
}
