////////////////////////////////////////////////////////////////////////////////
//
// File:	hyrel_can.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	common can related function used byte both hot head and main board
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifdef STM32F10X_LD
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_can.h"
#include "hothead.h"
#endif 

#ifdef STM32F4XX
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_can.h"
#include "main.h"
#endif


////////////////////////////////////////////////////////////////////////////////

uint32_t calculateChecksum8(uint8_t data[], uint32_t numWords)
{
	// common routine needed by 407 and HH to validate can block transfers
	// drop value of 0xffffffff from checksum such that different page sizes and memory
	// sizes can use the same routines without altering the checksum

	uint32_t checksum=0;
	uint32_t i;

	for (i=0; i<numWords; i++)
	{
		if (data[i] != 0xff)
		{
			checksum += data[i];
		}
	}
	return(checksum);
}

////////////////////////////////////////////////////////////////////////////////

uint32_t calculateChecksum16(uint16_t data[], uint32_t numWords)
{
	// common routine needed by 407 and HH to validate can block transfers
	// drop value of 0xffffffff from checksum such that different page sizes and memory
	// sizes can use the same routines without altering the checksum

	uint32_t checksum=0;
	uint32_t i;

	for (i=0; i<numWords; i++)
	{
		if (data[i] != 0xffff)
		{
			checksum += data[i];
		}
	}
	return(checksum);
}

////////////////////////////////////////////////////////////////////////////////

uint32_t calculateChecksum32(uint32_t data[], uint32_t numWords)
{
	// common routine needed by 407 and HH to validate can block transfers
	// drop value of 0xffffffff from checksum such that different page sizes and memory
	// sizes can use the same routines without altering the checksum

	uint32_t checksum=0;
	uint32_t i;

	for (i=0; i<numWords; i++)
	{
		if (data[i] != 0xffffffff)
		{
			checksum += data[i];
		}
	}
	return(checksum);
}

////////////////////////////////////////////////////////////////////////////////

void canInitFilter(CAN_TypeDef *CANx, byte filterNum, byte alias, boolean immediate, boolean enable)
{
#define FMR_FINIT    ((uint32_t)0x00000001) // Filter init mode

	uint32_t filterBit;

	filterBit	= (1 << filterNum);

	CANx->FMR |= FMR_FINIT;					// Initialization mode for the filter
	CANx->FA1R &= ~filterBit;				// Filter Deactivation

    CANx->FS1R |= filterBit;				// 32-bit scale for the filter
    CANx->FM1R &= ~filterBit;				// Id+Mask mode for the filter

	if (immediate)
	{
    	CANx->FFA1R |= filterBit;			// assign FIFO 1 for immediate mode the filters
    	CANx->sFilterRegister[filterNum].FR1 = FILTER_CONSTANT_VALUE | FILTER_IMMEDIATE_BIT;					// compare value
    	CANx->sFilterRegister[filterNum].FR2 = FILTER_MASK_CONSTANT;										 		// compare mask
	}
	else
	{
    	CANx->FFA1R &= ~filterBit;			// assign FIFO 0 for non-immediate mode (buffered) filters
    	CANx->sFilterRegister[filterNum].FR1 = FILTER_CONSTANT_VALUE | (alias << FILTER_DEVICE_LSB_POSITION);	// compare value
    	CANx->sFilterRegister[filterNum].FR2 = (FILTER_MASK_CONSTANT | FILTER_MASK_DEVICE);					 	// compare mask
	}

	if (enable == TRUE)
	{
		CANx->FA1R |= filterBit;			// Filter Activation
	}

	CANx->FMR &= ~FMR_FINIT;
}

////////////////////////////////////////////////////////////////////////////////

void canAddUserDefinedAlias(byte newAlias, byte aliases[])
{
	byte i;
		
	// first, find an open slot within the range specified
	for (i=0; i<NUM_USER_DEFINED_ALIASES; i++)
	{
		if (aliases[i] == ALIAS_UNUSED)
		{
			canInitFilter(CAN1, NUM_PRE_DEFINED_ALIASES+i, newAlias, FALSE, TRUE);			// filters are ordered predef(4), the user(8)
			aliases[i] = newAlias;
			break;
		}
	}
	if (i == NUM_USER_DEFINED_ALIASES)
	{
#ifdef STM32F10X_LD
		reportError1x32(ERROR_UNIT_CAN, ERROR_TOO_MANY_ALIASES, (uint32_t)newAlias);
#endif
#ifdef STM32F4XX
		sprintf(_errorStr, "ERROR_TOO_MANY_ALIASES: newAlias=%d", newAlias);
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////

void canRemoveUserDefinedAlias(byte oldAlias, byte aliases[])
{
	byte i;
	
	// find matching alias and disable it
	for (i=0; i<NUM_USER_DEFINED_ALIASES; i++)
	{
		if (aliases[i] == oldAlias)
		{
			canInitFilter(CAN1, NUM_PRE_DEFINED_ALIASES+i, ALIAS_UNUSED, FALSE, FALSE);			// filters are ordered predef(4), the user(8)
			aliases[i] = ALIAS_UNUSED;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

boolean canIsValidAlias(byte device)
{
	byte i;

	for (i=0; i<NUM_PRE_DEFINED_ALIASES; i++)
	{
		if (_gs._preDefinedAliases[i] == device)	// match!!! so it is for us
		{
			return(TRUE);
		}
	}

	for (i=0; i<NUM_USER_DEFINED_ALIASES; i++)
	{
		if (_gs._userDefinedAliases[i] == device)	// match!!! so it is for us
		{
			return(TRUE);
		}
	}
	return(FALSE);
}

////////////////////////////////////////////////////////////////////////////////

boolean canPageOutOfRange(byte msgId, byte page)
{
	// function to check if page number in a can packet is valid based on the msgId

	byte low, high;
	low = 0;
	high = 0;
	switch (msgId)
	{
		case CAN_MSG_SWITCH_DUTY :
		case CAN_MSG_SWITCH_PWM :
		case CAN_MSG_SWITCH_TEMP :
		case CAN_MSG_KARL_FACTORS :
			high = HH_NUM_SWITCHES-1;
			break;
		case CAN_MSG_UNIQUE_ID :
		case CAN_MSG_TABLE_START_OFFSETS :
			high = 1;
			break;
		case CAN_MSG_AUTO_STATUS_CONTROL :	// page used as valid flags
			high = 3;
			break;
		case CAN_MSG_LED_CONTROL :
		case CAN_MSG_UNPRIME_DWELL_PRIME :
			high = 7;
			break;
    	case CAN_MSG_EXTREMES :
		case CAN_MSG_HISTORY_RANGES :
		case CAN_MSG_HISTORY_COUNTERS :
			high = 15;
			break;
		case CAN_MSG_AUTO_STATUS :
			high = 19;
			break;
		case CAN_MSG_STATUS :
			high = 32;
			break;
		case CAN_MSG_EXTRUSION_MAIN		:	// page used as valid flags
		case CAN_MSG_EXTRUSION_UNPRIME	:	// page used as valid flags
		case CAN_MSG_EXTRUSION_PRIME	:	// page used as valid flags
		case CAN_MSG_EXTRUSION_MANUAL	:	// page used as valid flags
		case CAN_MSG_EXTRUSION_DWELL	:	// page used as valid flags
			high = 63;
			break;
		case CAN_MSG_ACCESS_OPTION_BYTES :	// page used as valid flags
		case CAN_MSG_ACCESS_BUFFER :
		case CAN_MSG_COPY_PAGE_TO_BUFFER :
		case CAN_MSG_COPY_BUFFER_TO_PAGE :
		case CAN_MSG_PAGE_CHECKSUM :
		case CAN_MSG_LOOPBACK :
		case CAN_MSG_EVENT_MESSAGE :
		case CAN_MSG_REPORT_ERROR :
			high = 0xff;
			break;
		default:
			high = 0;
			break;
	}
    if ((page >= low) && (page <= high))
	{
        return(FALSE);
	}
	else
	{
        return(TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////

byte canMsgIdToNumBytes(byte msgId)
{
	// function to convert the msgId into the expected number of bytes

	switch (msgId)
	{
		case CAN_MSG_COPY_PAGE_TO_BUFFER :
		case CAN_MSG_START_PRIMARY_PROGRAM :
			return(CAN_BYTES_0);

		case CAN_MSG_DEVICE_POSITION :
		case CAN_MSG_ADD_ALIAS :
		case CAN_MSG_REMOVE_ALIAS :
		case CAN_MSG_MOTOR_ENABLE :
		case CAN_MSG_MICROSTEPS :
		case CAN_MSG_CURRENT_BOOST :
			return(CAN_BYTES_1);

		case CAN_MSG_HISTORY_RANGES :
		case CAN_MSG_LED_OVERRIDE :
		case CAN_MSG_ERROR_REPORT_INTERVAL :
			return(CAN_BYTES_2);

		case CAN_MSG_LED_CONTROL :
			return(CAN_BYTES_3);

		case CAN_MSG_PRE_DEFINED_ALIASES :
		case CAN_MSG_EXTRUSION_OVERRIDE_PCT :
		case CAN_MSG_EXTRUSION_STEPS_PER_NL :
		case CAN_MSG_UNPRIME_DWELL_PRIME :
		case CAN_MSG_FILL_BUFFER :
		case CAN_MSG_COPY_BUFFER_TO_PAGE :
		case CAN_MSG_PAGE_CHECKSUM :
			return(CAN_BYTES_4);

		case CAN_MSG_DEVICE_INFO :
		case CAN_MSG_HISTORY_COUNTERS :
			return(CAN_BYTES_5);

		case CAN_MSG_UNIQUE_ID :
		case CAN_MSG_HISTORY_RANGE_DEF :
		case CAN_MSG_AUTO_STATUS_CONTROL :
		case CAN_MSG_SWITCH_DUTY :
		case CAN_MSG_SWITCH_TEMP :
		case CAN_MSG_KARL_FACTORS :
			return(CAN_BYTES_6);

		case CAN_MSG_SWITCH_PWM :
			return(CAN_BYTES_7);

		case CAN_MSG_FLASH_CONFIG :
		case CAN_MSG_USER_DEFINED_ALIASES :
		case CAN_MSG_STATUS :
		case CAN_MSG_PAGE_DEF :
		case CAN_MSG_TABLE_START_OFFSETS :
		case CAN_MSG_GLOBAL_STRUCT_INFO :
		case CAN_MSG_CONTROL_WORD :
		case CAN_MSG_EXTREMES :
		case CAN_MSG_EXTRUSION_MAIN :
		case CAN_MSG_EXTRUSION_UNPRIME :
		case CAN_MSG_EXTRUSION_PRIME :
		case CAN_MSG_EXTRUSION_MANUAL :
		case CAN_MSG_EXTRUSION_DWELL :
		case CAN_MSG_ACCESS_BUFFER :
		case CAN_MSG_COPY_ADDR_TO_BUFFER :
		case CAN_MSG_ACCESS_OPTION_BYTES :
		case CAN_MSG_EVENT_MESSAGE :
		case CAN_MSG_REPORT_ERROR :
		case CAN_MSG_AUTO_STATUS :
		case CAN_MSG_LOOPBACK :
			return(CAN_BYTES_8);
		default :
			break;
	}
	return(CAN_BYTES_FF);
}

////////////////////////////////////////////////////////////////////////////////

byte canCheckCanStruct(canStruct *canStructPtr)
{
	// routine to check that the number of byte and page number are within valid range
    byte msgIdNumBytes;

    msgIdNumBytes   = canMsgIdToNumBytes(canStructPtr->sw.msgId);
    if (msgIdNumBytes != canStructPtr->sw.numBytes)
	{
#ifdef STM32F10X_LD
       	reportError4x8(ERROR_UNIT_CAN, ERROR_NUM_BYTES_MISMATCH,
									canStructPtr->sw.msgType, canStructPtr->sw.msgId, msgIdNumBytes, canStructPtr->sw.numBytes);
#endif
#ifdef STM32F4XX
		sprintf(_errorStr, "ERROR_NUM_BYTES_MISMATCH: msgType=%d msgId=0x%02x expected=%d received=%d",
									canStructPtr->sw.msgType, canStructPtr->sw.msgId, msgIdNumBytes, canStructPtr->sw.numBytes);
       	sendError(_errorStr);
#endif
       	return(ERROR_NUM_BYTES_MISMATCH);
    }

	if (canPageOutOfRange(canStructPtr->sw.msgId, canStructPtr->sw.page))
	{
#ifdef STM32F10X_LD
		reportErrorCan(ERROR_UNIT_CAN, ERROR_BAD_PAGE_NUM, (canSwStruct *)canStructPtr);
#endif
#ifdef STM32F4XX
		sprintf(_errorStr, "ERROR_BAD_PAGE_NUM: msgType=%d msgId=0x%02x page=%d numByte=%d",
									canStructPtr->sw.msgType, canStructPtr->sw.msgId, canStructPtr->sw.page, canStructPtr->sw.numBytes);
       	sendError(_errorStr);
#endif
		return(ERROR_BAD_PAGE_NUM);
	}
	return(PASS);
}

////////////////////////////////////////////////////////////////////////////////

byte canTransmit(CAN_TypeDef *CANx, canHwStruct *canHwStructPtr)
{
	CAN_TxMailBox_TypeDef *mailboxPtr;
	// low level routine to convert the canStruct to hardware registers and tell hardware to begin transfer
  	uint8_t transmitMailbox = CAN_TxStatus_NoMailBox;

	/* Select one empty transmit mailbox */
	if ((CANx->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
	{
		transmitMailbox = 0;
	}
	else if ((CANx->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
	{
		transmitMailbox = 1;
	}
	else if ((CANx->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
	{
		transmitMailbox = 2;
	}
	else
	{
		transmitMailbox = CAN_TxStatus_NoMailBox;
	}

	if (transmitMailbox != CAN_TxStatus_NoMailBox)
	{
		mailboxPtr = &CANx->sTxMailBox[transmitMailbox];

		mailboxPtr->TIR		= canHwStructPtr->IR;
		mailboxPtr->TDTR	= canHwStructPtr->DTR;
		mailboxPtr->TDLR	= canHwStructPtr->DLR;
		mailboxPtr->TDHR	= canHwStructPtr->DHR;

		mailboxPtr->TIR	   |= (uint32_t)0x00000001; // TMIDxR_TXRQ -- tell CAN that HW packet is ready to transmit
	}
	return(transmitMailbox);
}

////////////////////////////////////////////////////////////////////////////////

void updateTxQueueIndices(void)
{
	_gs._canTxQ.numMsg--;
	_gs._canTxQ.nextOut++;
	if (_gs._canTxQ.nextOut == CAN_TX_QUEUE_SIZE)
	{
		_gs._canTxQ.nextOut = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////

byte canProcessTxQueue()
{
	// function to take packet from global TX queue and put in in the CAN HW
	byte retValue;
#ifndef BOOTLOADER // only allow multiple tx per slice for non bootloader
	byte i;
	byte canNumTxTransfersPerSlice = CAN_NUM_TX_TRANSFERS_PER_SLICE;
#ifdef STM32F4XX
	if (_gs._bl.started == TRUE)
	{
		canNumTxTransfersPerSlice = 1;	// slow down bootloader comm as bootloader does not have a large input fifo
	}
#endif

	for (i=0; i<canNumTxTransfersPerSlice; i++)
#endif // BOOTLOADER
	{
#ifdef STM32F10X_LD
		if ((!CAN_INITIALIZED) || (_gs._canTxQ.numMsg == 0))	// not ready to send message OR nothing to send
		{
			return(CAN_TX_OK);
		}
#endif

#ifdef STM32F4XX
		if (_gs._canTxQ.numMsg == 0)	// nothing to send
		{
			return(CAN_TX_OK);	// Not ready to send output
		}
#endif 

		retValue =  canTransmit(CAN1, (canHwStruct *)&_gs._canTxQ.Q[_gs._canTxQ.nextOut]);	// try to send

		if (retValue == CAN_TxStatus_NoMailBox)		// no room in the HW, so exit without changing queue indicies
		{
			return(WARNING_TX_MAILBOX_FULL);
		}
		
		// otherwise, message was sent, so update TX queue indicies
		updateTxQueueIndices();

#ifndef BOOTLOADER
#ifdef STM32F10X_LD
		// activate status lights for transmit
		_gs._led.statusValue |= LED_STATUS_CAN_TX;
		_gs._led.progressValue |= LED_PROGRESS_CAN_TX;
		_gs._led.canTxLedCnt = 2; // will result in 10ms to 20ms on time;
#endif
#endif
	}
	return(CAN_TX_OK);
}

////////////////////////////////////////////////////////////////////////////////

void canProcessTxQueueUntilEmpty()
{
	while (_gs._canTxQ.numMsg > 0)
		canProcessTxQueue();
}

////////////////////////////////////////////////////////////////////////////////

canStruct *canGetTxQueueNextInPtr()
{
	// function to return next open spot in the queue.  if there's no slot, wait at least 1ms to
#ifdef STM32F10X_LD
	if (_gs._canTxQ.numMsg >= CAN_TX_QUEUE_SIZE)
	{
		// PROBLEM .... if no room in queue, the the errorReport will not make it into the queue!!!!
		updateTxQueueIndices();				//   so....open up one space in the queue (overwriting oldest

		reportError1x32(ERROR_UNIT_CAN, ERROR_TX_QUEUE_FULL, CAN_TX_QUEUE_SIZE);
		return((canStruct *)0x00000000);	// return NIL pointer if no room in the queue
	}
#endif
#ifdef STM32F4XX
	if (_gs._canTxQ.numMsg >= CAN_TX_QUEUE_SIZE)
	{
		sendError("ERROR_TX_QUEUE_FULL");
		return((canStruct *)0x00000000);	// return NIL pointer if no room in the queue
	}
#endif
	else
	{
		return(&_gs._canTxQ.Q[_gs._canTxQ.nextIn]);
	}
}

////////////////////////////////////////////////////////////////////////////////

void canFillOutCanStruct(canSwStruct *canSwStructPtr)
{
	// fill in reserved fields, numBytes, IDE, RTR, etc
	// TIR = Transmit Identifier Regsister (from CAN hardware)
	//
	//     = stdID[10:0], extID[17:0], IDE, RTR, 0
	//
	//     = 0b100, msgType[1:0], device[7:0], msgId[7:0], page[7:0], IDE, RTR, 0
	//
	//        31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //     =   1  0  0 | mT|      devicePosition    |      msgId[7:0]       |        page[7:0]      |XI|RT| 0|
	//																							     XI = IDE
	//																							     RT = RTR
	// IDE = Extended IDentifier (additional 18 bits of header info)
	// RTR = Return Transmit Request = (msgType == CAN_READ)
	//
	// IDE is always set
	// upper 3 bits always == 0b100
	// lsb is always 0

	canSwStructPtr->fixed_b100 = 0b100;
	canSwStructPtr->IDE = 1;
	canSwStructPtr->RTR = (canSwStructPtr->msgType == CAN_READ); 		// set RTR on CAN_READ, 0 otherwise
	canSwStructPtr->fixed_b0 = 0b0;
	canSwStructPtr->numBytes = canMsgIdToNumBytes(canSwStructPtr->msgId);
#ifdef STM32F4XX
	canSwStructPtr->device = remapDevicePosition(canSwStructPtr->device); // remap alias
#endif
}

////////////////////////////////////////////////////////////////////////////////

byte canAddToTxQueue(canSwStruct *canSwStructPtr)
{
	// function to add incoming can hw Struct to transmit queue
	// incoming pointer may already be pointing into the queue, in which case, just update
	// indices, otherwise, data much be copied into the queue from the *pointer.

	canStruct *nextInPtr;

	canFillOutCanStruct(canSwStructPtr);	// fill in reserved fields, numBytes, IDE, RTR, etc

    if ((nextInPtr = canGetTxQueueNextInPtr()) == 0)
	{
		return(ERROR_TX_QUEUE_FULL);
	}

	if ((canStruct *)canSwStructPtr != nextInPtr)	// data is not in Q, so copy over
	{
		*nextInPtr = *(canStruct *)canSwStructPtr;
	}

	_gs._canTxQ.numMsg++; _gs._canTxQ.nextIn++;
	if (_gs._canTxQ.nextIn == CAN_TX_QUEUE_SIZE)
	{
		_gs._canTxQ.nextIn = 0;
	}
	return(CAN_TX_OK);
}

////////////////////////////////////////////////////////////////////////////////

byte canPackIntoTxQueue2x32(byte msgType, byte device, byte msgId, byte page, boolean immediate, uint32_t u32_0, uint32_t u32_1)
{
	// function to pack can info and add to transmit queue

	canSwStruct *canSwStructPtr;

    if ((canSwStructPtr = (canSwStruct *)canGetTxQueueNextInPtr()) == 0)
	{
		return(ERROR_TX_QUEUE_FULL);
	}

	canSwStructPtr->msgType			= msgType;
	canSwStructPtr->device			= device;
	canSwStructPtr->msgId			= msgId;
	canSwStructPtr->immediate		= immediate;
	canSwStructPtr->page			= page;
	canSwStructPtr->payload.u32[0]	= u32_0;
	canSwStructPtr->payload.u32[1]	= u32_1;
	return(canAddToTxQueue(canSwStructPtr));
}

////////////////////////////////////////////////////////////////////////////////

byte canPackIntoTxQueue4x16(byte msgType, byte device, byte msgId, byte page, boolean immediate,
																uint16_t u16_0, uint16_t u16_1, uint16_t u16_2, uint16_t u16_3)
{
	// function to pack can info and add to transmit queue

	canSwStruct *canSwStructPtr;

	if ((canSwStructPtr = (canSwStruct *)canGetTxQueueNextInPtr()) == 0)
	{
		return(ERROR_TX_QUEUE_FULL);
	}

	canSwStructPtr->msgType			= msgType;
	canSwStructPtr->device			= device;
	canSwStructPtr->msgId			= msgId;
	canSwStructPtr->immediate		= immediate;
	canSwStructPtr->page			= page;
	canSwStructPtr->payload.u16[0]	= u16_0;
	canSwStructPtr->payload.u16[1]	= u16_1;
	canSwStructPtr->payload.u16[2]	= u16_2;
	canSwStructPtr->payload.u16[3]	= u16_3;
	return(canAddToTxQueue(canSwStructPtr));
}

////////////////////////////////////////////////////////////////////////////////

byte canPackIntoTxQueue8x8(byte msgType, byte device, byte msgId, byte page, boolean immediate, byte payload[])
{
	// function to pack can info and add to transmit queue

	canSwStruct *canSwStructPtr;
	byte i;

	if ((canSwStructPtr = (canSwStruct *)canGetTxQueueNextInPtr()) == 0)
	{
		return(ERROR_TX_QUEUE_FULL);
	}

	canSwStructPtr->msgType			= msgType;
	canSwStructPtr->device			= device;
	canSwStructPtr->msgId			= msgId;
	canSwStructPtr->immediate		= immediate;
	canSwStructPtr->page			= page;
	for (i=0; i<8; i++)
	{
		canSwStructPtr->payload.u8[i]	= payload[i];
	}
	return(canAddToTxQueue(canSwStructPtr));
}

////////////////////////////////////////////////////////////////////////////////
// RECEIVE ROUTINES ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void canGetMailboxData(CAN_FIFOMailBox_TypeDef *mailboxPtr, canHwStruct *canHwStructPtr)
{
	canHwStructPtr->IR	= mailboxPtr->RIR;
	canHwStructPtr->DTR	= mailboxPtr->RDTR;
	canHwStructPtr->DLR	= mailboxPtr->RDLR;
	canHwStructPtr->DHR	= mailboxPtr->RDHR;
}

////////////////////////////////////////////////////////////////////////////////

boolean canMsgWaitingFifo0(CAN_TypeDef *CANx)
{
	return((uint8_t)(CANx->RF0R&(uint32_t)0x03));
}

////////////////////////////////////////////////////////////////////////////////

boolean canMsgWaitingFifo1(CAN_TypeDef *CANx)
{
	return((uint8_t)(CANx->RF1R&(uint32_t)0x03));
}

////////////////////////////////////////////////////////////////////////////////

byte canReceiveFifo0(CAN_TypeDef *CANx, canHwStruct *canHwStructPtr)
{
	// low level routine to copy data from the CAN hardware unit 

	CAN_FIFOMailBox_TypeDef *mailboxPtr;

	if (canMsgWaitingFifo0(CANx))
	{
		mailboxPtr = &CANx->sFIFOMailBox[CAN_FIFO0];
	}
	else
	{
		return(WARNING_RX_NO_MSG_WAITING);
	}

	canGetMailboxData(mailboxPtr, canHwStructPtr);

	CANx->RF0R |= CAN_RF0R_RFOM0;	// Release FIFO0
	return(CAN_RX_OK);
}

////////////////////////////////////////////////////////////////////////////////

byte canReceiveFifo1(CAN_TypeDef *CANx, canHwStruct *canHwStructPtr)
{
	// low level routine to copy data from the CAN hardware unit 

	CAN_FIFOMailBox_TypeDef *mailboxPtr;

	if (canMsgWaitingFifo1(CANx))
	{
		mailboxPtr = &CANx->sFIFOMailBox[CAN_FIFO1];
	}
	else
	{
		return(WARNING_RX_NO_MSG_WAITING);
	}

	canGetMailboxData(mailboxPtr, canHwStructPtr);

	CANx->RF1R |= CAN_RF1R_RFOM1;	// Release FIFO1
	return(CAN_RX_OK);
}

////////////////////////////////////////////////////////////////////////////////

byte canReceive(CAN_TypeDef *CANx, uint8_t FifoNumber, canHwStruct *canHwStructPtr)
{
	// low level routine to copy data from the CAN hardware unit 

	if (FifoNumber == CAN_FIFO0)
	{
		return(canReceiveFifo0(CANx, canHwStructPtr));
	}
	else //if (FifoNumber == CAN_FIFO1)
	{
		return(canReceiveFifo1(CANx, canHwStructPtr));
	}
}

////////////////////////////////////////////////////////////////////////////////

byte canAddToImmediateRxQueue()
{
	// first check if a message is wait and if so, is it an immediate mode
	// ALL immediate messages are directed to Fifo1 (independent of device id/alias)
	// so also need to check against alias table to know the message is for this device

	if (canMsgWaitingFifo1(CAN1) == FALSE)
	{
		return(WARNING_RX_NO_MSG_WAITING);
	}

	//canSwStructPtr = (canSwStruct *)&CAN1->sFIFOMailBox[CAN_FIFO1];

	if (!canIsValidAlias(((canSwStruct *)&CAN1->sFIFOMailBox[CAN_FIFO1])->device))	// not for us, so reject here
	{
		CAN1->RF1R |= CAN_RF1R_RFOM1;	// Release FIFO1
	}
	else if (_gs._canRxIsAvail == FALSE)	// single immediate message buffer is empty
	{
		canReceiveFifo1(CAN1, (canHwStruct *)&_gs._canImmediateRx);
		_gs._canImmediateRxIsAvail = TRUE;
	}
	return(CAN_RX_OK);
}

////////////////////////////////////////////////////////////////////////////////

byte canAddToRxQueue()
{
	// function to take data from the CAN hw and add it to the global RX queue
	// if data available in CAN hardware, grab it and put it in the RX queue

	if (canMsgWaitingFifo0(CAN1) == FALSE)
	{
		return(WARNING_RX_NO_MSG_WAITING);
	}

#ifdef STM32F10X_LD
    if (!CAN_INITIALIZED)
	{
        return(CAN_RX_OK);  // Not ready to look for input
	}
	if (_gs._canRxQ.numMsg >= CAN_RX_QUEUE_SIZE)
	{
		reportError1x32(ERROR_UNIT_CAN, ERROR_RX_QUEUE_FULL, CAN_RX_QUEUE_SIZE);
		return(ERROR_RX_QUEUE_FULL);
	}
#endif

#ifdef STM32F4XX
	if (_gs._canRxQ.numMsg >= CAN_RX_QUEUE_SIZE)
	{
		sendError("ERROR_RX_QUEUE_FULL");
		return(ERROR_RX_QUEUE_FULL);
	}
#endif

	canReceiveFifo0(CAN1, (canHwStruct *)&_gs._canRxQ.Q[_gs._canRxQ.nextIn]);

	_gs._canRxQ.numMsg++;
	_gs._canRxQ.nextIn++;
	if (_gs._canRxQ.nextIn == CAN_RX_QUEUE_SIZE)
	{
		_gs._canRxQ.nextIn = 0;
	}
#ifdef STM32F10X_LD
	// activate status lights for receive
	_gs._led.statusValue |= LED_STATUS_CAN_RX;
	_gs._led.progressValue |= LED_PROGRESS_CAN_RX;
	_gs._led.canRxLedCnt = 2; // will result in 10ms to 20ms on time;
#endif
	return(CAN_RX_OK);
}

////////////////////////////////////////////////////////////////////////////////

void canPrepNextRx()
{
	// grab next RX data from queue and convert it to format more readily used by hw
	_gs._canRxIsAvail = FALSE;
#ifdef STM32F10X_LD
	if ((EXTRUDING) && !CONTINUOUS_EXTRUDE)  //block until current extrusion is finished.
		return;
#endif

	if (_gs._canRxQ.numMsg > 0)
	{
		_gs._canRx = _gs._canRxQ.Q[_gs._canRxQ.nextOut];  // copy from queue to staging area for execution
		_gs._canRxQ.numMsg--;
		_gs._canRxQ.nextOut++;

		if (_gs._canRxQ.nextOut == CAN_RX_QUEUE_SIZE)
		{
			_gs._canRxQ.nextOut = 0;
		}
		if (canCheckCanStruct(&_gs._canRx) == PASS) {
			_gs._canRxIsAvail = TRUE;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void canAddToRxQueueNoReturn(void) // provided so that function can be used in the main control loop which ca
{
    canAddToRxQueue();
}

////////////////////////////////////////////////////////////////////////////////


void canProcessTxQueueNoReturn(void)
{
    canProcessTxQueue();
}

////////////////////////////////////////////////////////////////////////////////
