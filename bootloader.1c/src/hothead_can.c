////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_can.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	contains functions that deal with the CANbus hardware and process
//			incoming and outgoing data.  all of the high level protocol is 
//			dealt with in these functions.
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x_can.h"
#include "hothead.h"
#include "hothead_can.h"
#include "hothead_hw_init.h"
#include "stm32f10x_flash.h"

////////////////////////////////////////////////////////////////////////////////

boolean isFlashAddress(uint32_t addr)
{
	if ((addr >= 0x08000000) && (addr <= 0x08007FFF))
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////////////

boolean isValidWriteFlashAddress(uint32_t addr)
{
	if (addr & 0x00000001) // addr must be even (no byte access)
	{
		return(FALSE);
	}
#ifdef BOOTLOADER
	if ((addr >= CODE_BASE_ADDR) && (addr < HISTORY0_BASE_ADDR))
	{ // allow loading of code, tables, and soap ... but not BL or history
		return(TRUE);
	}
#else
	if (((addr >= TABLE_BASE_ADDR) && (addr < HISTORY0_BASE_ADDR)) || ((addr < CODE_BASE_ADDR) && UPDATE_BOOTLOADER))
	{
		// allow loading of bootloader code, tables, and soap .... but not code or history
		return(TRUE);
	}
#endif
	return(FALSE);
}

////////////////////////////////////////////////////////////////////////////////

boolean isRamAddress(uint32_t addr)
{
	if ((addr >= 0x20000000) && (addr < 0x20002800))	// 10KBytes RAM
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////////////

boolean isSpecialAddress(uint32_t addr)
{
	if ((addr >= 0x40000000) && (addr < 0x40003400))
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
	return(FALSE);
}

////////////////////////////////////////////////////////////////////////////////

void sendEventMsg(canEventMsgType eventMsg, uint32_t value)
{
	canPackIntoTxQueue2x32(CAN_WRITE, _gs._devicePosition, CAN_MSG_EVENT_MESSAGE, eventMsg, FALSE, value, 0);
}

////////////////////////////////////////////////////////////////////////////////

void getExtrusionParameters(extrusionIndex index, canSwStruct *canRx)
{
	extrusionStruct *exPtr;

	exPtr = &_gs._extrusion.extrusion[index];

	exPtr->control.u32 = (uint32_t)canRx->page;
	if (exPtr->control.bit.updateRate) {
		exPtr->data.u32.rate = canRx->payload.u32[0];
	}
	if (exPtr->control.bit.updateQty) {
		exPtr->data.u32.qty = canRx->payload.u32[1];
	}
	if (exPtr->control.bit.immediateExtrusion && ((exPtr->control.bit.updateRate == 0) && (exPtr->control.bit.updateQty == 0)))
	{ // special case with immediate to extrude again using last parameters
		exPtr->control.bit.updateRate = 1;
		exPtr->control.bit.updateQty = 1;
	}
	exPtr->localScale = 1;

	if (index == EXT_DWELL)
	{
		exPtr->control.bit.blockStep = 1;
	}
	else
	{
		exPtr->control.bit.blockStep = 0;
	}
	if ((exPtr->control.bit.immediateExtrusion) && (index != EXT_DWELL))  // using dwell to loop on unprime/dwell/prime
	{
		setupExtrusion(exPtr);
	}
}

////////////////////////////////////////////////////////////////////////////////

FLASH_Status FLASH_EraseAndFileOptionBytes(byte optionBytes[])
{
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
#define EraseTimeout         	 ((uint32_t)0x000B0000)
#define ProgramTimeout      	 ((uint32_t)0x00002000)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)

	FLASH_Status status;
	int i;

	status = FLASH_WaitForLastOperation(EraseTimeout); // Wait for last operation to complete
	if (status == FLASH_COMPLETE)
	{
		FLASH->OPTKEYR = FLASH_KEY1;  // Authorize the small information block programming
		FLASH->OPTKEYR = FLASH_KEY2;

		// erase option bytes
		FLASH->CR |= CR_OPTER_Set;
		FLASH->CR |= CR_STRT_Set;
		status = FLASH_WaitForLastOperation(EraseTimeout); // Wait for last operation to complete
		if (status == FLASH_COMPLETE)
		{
			FLASH->CR &= CR_OPTER_Reset;	// disable the OPTER Bit
		}
		else
		{
			return(status);
		}

		for (i=0; i<8; i++)
		{
			FLASH->CR |= CR_OPTPG_Set;  // enable the Option Bytes Programming operation
			*((__IO uint16_t *)(INFO_MEMORY_ADDR + (2 * i))) = (uint8_t)optionBytes[i];  // write the byte
			status = FLASH_WaitForLastOperation(ProgramTimeout); // Wait for last operation to complete
			if (status == FLASH_TIMEOUT)
			{
				break;	// we timed out, so quit
			}
			FLASH->CR &= CR_OPTPG_Reset;
		}
		if (status != FLASH_TIMEOUT)
		{
			FLASH->CR &= CR_OPTPG_Reset;
		}
	}
	return(status);
}

////////////////////////////////////////////////////////////////////////////////

void canProcessRxQueue()
{
	canSwStruct *canRx, canTx;
	byte *accessPtr;
	uint16_t i;
	uint16_t page;
	uint32_t checksum;
	boolean sendMsgOnExit;
	FLASH_Status flashStatus;

	// operate right out of HW FIFO (will release fifo on exit
	canRx = (canSwStruct *)&CAN1->sFIFOMailBox[CAN_FIFO1];

	switch (canRx->msgType)
	{
		case CAN_WRITE :
			switch (canRx->msgId)
			{
				case CAN_MSG_CONTROL_WORD :  // use mask to and/or in the new bits
					// the ONLY control bit that is sent via can used by the bootloader is
					// the request for a software reset....so no need to store the control word, just
					// perform the reset (saves 16 bytes)
					//_gs._controlWord.u32 = (canRx->payload.u32[0] & canRx->payload.u32[1]) | (_gs._controlWord.u32 & ~(canRx->payload.u32[1]));
					//if (SW_RESET)
					if (canRx->payload.u32[0] & canRx->payload.u32[1] & HH_SW_RESET_BIT)
					{
						NVIC_SystemReset();
					};
					break;
				case CAN_MSG_FILL_BUFFER :
					for (i=0; i<WORKING_BUFFER_SIZE; i++)
					{
						_gs._workingBuffer[i] = canRx->payload.u32[0];
					}
					break;
				case CAN_MSG_ACCESS_BUFFER :
					_gs._workingBuffer[(2 * canRx->page)]		= canRx->payload.u32[0];
					_gs._workingBuffer[(2 * canRx->page) + 1]	= canRx->payload.u32[1];
					break;
				case CAN_MSG_COPY_PAGE_TO_BUFFER :
					accessPtr = (byte *)(FLASH_BASE_ADDR + (canRx->page * FLASH_PAGE_SIZE));
					if (isFlashAddress((uint32_t)accessPtr) == FALSE)
					{
						reportErrorCan(ERROR_UNIT_CAN, ERROR_BAD_PAGE_NUM, canRx);
						return;
					}
					copyRam((byte *)_gs._workingBuffer, accessPtr, (WORKING_BUFFER_SIZE<<2));
					break;
				case CAN_MSG_COPY_BUFFER_TO_PAGE :
					accessPtr = (byte *)(FLASH_BASE_ADDR + (canRx->page * FLASH_PAGE_SIZE));
					if (isValidWriteFlashAddress((uint32_t)accessPtr) == FALSE)
					{
						reportErrorCan(ERROR_UNIT_CAN, ERROR_BAD_PAGE_NUM, canRx);
						return;
					}
					checksum = calculateChecksum32(_gs._workingBuffer, WORKING_BUFFER_SIZE);
					if (checksum != canRx->payload.u32[0])
					{
						reportError1x32(ERROR_UNIT_CAN, ERROR_BAD_CHECKSUM, checksum);
						return;
					}
					erasePage(canRx->page);
					for (i=0; i<WORKING_BUFFER_SIZE; i++)
					{
						FLASH_Unlock();
						FLASH_ProgramWord((uint32_t)accessPtr, _gs._workingBuffer[i]);
						FLASH_Lock();
						accessPtr += 4;
					}
					// checksum newly written page
					checksum = calculateChecksum32((uint32_t *)(FLASH_BASE_ADDR + (canRx->page * FLASH_PAGE_SIZE)), WORKING_BUFFER_SIZE);
					if (checksum != canRx->payload.u32[0])
					{
						reportError1x32(ERROR_UNIT_CAN, ERROR_PAGE_WRITE_FAILED, canRx->page);
					}
					break;
				case CAN_MSG_ACCESS_OPTION_BYTES :
					// page will have a bitwise mask of bytes to update
					// currently allow update of ONLY
					//[0] - RDP (don't screw with this one
					//[1] - USER upper 5-bits are used for deviceRev ...(requires special code)
					//[2] - DATA0 - used for deviceType
					//[3] - DATA1 - used for key
					//[7:4] - WRP[3:0] - write protection
					FLASH_Unlock();
					flashStatus = FLASH_EraseAndFileOptionBytes(canRx->payload.u8);
					FLASH_Lock();
					if (flashStatus != FLASH_COMPLETE)
					{
						reportError1x32(ERROR_UNIT_CAN, ERROR_OPTION_BYTE_PROGRAMMING_FAILED, (uint32_t)flashStatus);
					}
					break;
				case CAN_MSG_START_PRIMARY_PROGRAM :
					applicationMain();
					break;
				case CAN_MSG_LOOPBACK :
					canTx = *canRx;
					canTx.device	= _gs._devicePosition;
					//canTx.msgType	= CAN_WRITE;
					//canTx.msgId		= canRx->msgId;
					//canTx.immediate	= FALSE;			// ACK's are sent as immediate so sender can proceed
					//canTx.page		= canRx->page;
					//canTx.numBytes	= canRx->numBytes;
					//canTx.payload	= canRx->payload
					canAddToTxQueue(&canTx);
					break;
				default :
					reportErrorCan(ERROR_UNIT_CAN, ERROR_SWITCH_MSG_ID, canRx);
					break;

			}
			break;
			case CAN_READ :
				// setup the outgoing datapacket
				canTx = *canRx;
				canTx.device	= _gs._devicePosition;
				canTx.msgType	= CAN_RETURN_DATA;
				// zeroRam(&canTx.payload.u8[0], 8); // clear return data;
				fillRam(&canTx.payload.u8[0], 8, 0); // clear return data;
				sendMsgOnExit	= TRUE;
				switch (canRx->msgId)					// these are RTR transfers, so numBytes is set, but no payload
				{
					case CAN_MSG_DEVICE_INFO :
						// return specific info about the device
						canTx.payload.u8[0]	= *(((byte *)INFO_MEMORY_ADDR) + INFO_MEM_DEV_TYPE_OFFSET);	// deviceType
						// deviceRevision is in the 5 upper 5-bits of the USER option byte
						canTx.payload.u8[1]	= (*(((byte *)INFO_MEMORY_ADDR) + INFO_MEM_DEV_REV_OFFSET)) >> 3;
						canTx.payload.u8[2]	= (byte)SOFTWARE_MAJOR_REVISION;				// compiled in constant
						canTx.payload.u8[3]	= (byte)SOFTWARE_MINOR_REVISION;				// compiled in constant
						canTx.payload.u8[4]	= *(((byte *)INFO_MEMORY_ADDR) + INFO_MEM_DEV_KEY_OFFSET);
						break;
					case CAN_MSG_FLASH_CONFIG :
						canTx.payload.u16[0]	= *(uint16_t *)FLASH_SIZE_REG_ADDR;		// number of KByte of flash
						canTx.payload.u16[1]	= (uint16_t)FLASH_PAGE_SIZE;			// flash page size for this part
						canTx.payload.u32[1]	= FLASH_BASE_ADDR;
						break;
					case CAN_MSG_UNIQUE_ID :
						for (page=0; page<2; page++)
						{
							canTx.page = page;
							for (i=0; i<6; i++)
							{
								canTx.payload.u8[i] = *(byte *)(UNIQUE_ID_REG_ADDR + (6 * page) + i);
							}
							canAddToTxQueue(&canTx);
						}
						sendMsgOnExit = FALSE;
						break;
					case CAN_MSG_ACCESS_BUFFER :
						for (i=0; i<WORKING_BUFFER_SIZE; i+=2)
						{
							canTx.page = i / 2;
							canTx.payload.u32[0] = _gs._workingBuffer[i];
							canTx.payload.u32[1] = _gs._workingBuffer[i+1];
							canAddToTxQueue(&canTx);
						}
						sendMsgOnExit = FALSE;
						break;
					case CAN_MSG_PAGE_CHECKSUM :
						canTx.payload.u32[0] = calculateChecksum32((uint32_t *)(FLASH_BASE_ADDR + (canRx->page * FLASH_PAGE_SIZE)), FLASH_PAGE_SIZE/4);
						break;
					case CAN_MSG_PAGE_DEF :
						copyRam((byte *)canTx.payload.u16, (byte *)_gs._pageNumDef, 8);
						break;
					case CAN_MSG_STATUS :
						// this is part of the repetier boot sequence to request temperature page.  BOOTLOADER
						// does not process temperatures, so no info to return, but at the same time we do not
						// want to generate and error message, so we need to 'eat' this request.
						sendMsgOnExit = FALSE;
						break;
					case CAN_MSG_ACCESS_OPTION_BYTES :
						for (page=0; page<2; page++)
						{
							canTx.page = page;
							copyRam(canTx.payload.u8, (byte *)(INFO_MEMORY_ADDR + (8 * page)), 8);
							canAddToTxQueue(&canTx);
						}
						sendMsgOnExit = FALSE;
						break;
					default :
						reportErrorCan(ERROR_UNIT_CAN, ERROR_SWITCH_MSG_ID, canRx);
						return;
						break;
				}
				if (sendMsgOnExit == TRUE)
				{
					canAddToTxQueue(&canTx);
				}
				break;
		//case CAN_WRITE_ACK : //XXX we don't handle ACKs from host at this time....will generate an error
			//break;
		//case CAN_RETURN_DATA : //XXX we don't handle return data from the host at this time....will generate an error
			//break;
		default :
			reportErrorCan(ERROR_UNIT_CAN, ERROR_SWITCH_MSG_TYPE, canRx);
						return;
			break;
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////
