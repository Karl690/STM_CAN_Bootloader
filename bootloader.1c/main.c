
////////////////////////////////////////////////////////////////////////////////
//
// File:	main.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	top level for bootloader project
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
//	
//	Memory Map
//	
//	32- 1Kbyte pages
//	
//	00) 0x0800 0000 - Bootloader
//	01) 0x0800 0400 - Bootloader
//	02) 0x0800 0800 - Bootloader
//	03) 0x0800 0C00 - Bootloader
//	04) 0x0800 1000 - Main Program
//	 :
//	 :
//	27) 0x0800 6C00 - Main Program
//	28) 0x0800 7000 - Device specific Data (RTD Tables, etc)
//	29) 0x0800 7400 - User scratchpad memory
//	30) 0x0800 7800 - Device History Bank A
//	31) 0x0800 7C00 - Device History Bank B
//	
//		0x1FFF F800 - 16-byte Information Memory  (really only 2 useful bytes)
//	
//		0x1FFF F7E8	- 12-byte Unique Device ID
//	
////////////////////////////////////////////////////////////////////////////////
//
// The bootloader will allow a new primary program to be written to the hothead 
// board via the canBus.  The same hyrel canbus high level protocol will be used.
//
// Basic operation:
//		- At powerup, the bootloader starts
//		- Flash some LED's along the way to show status 
//		- Check main program memory for proper checksum (reportError)
//		  (use just a 32-bit uint sum and compare to last 4 bytes of code space
//		  which is the store location for the checksum
//		- Initialize globals, IOs, and ADC
//		- Setup ADC to read position resistor
//		- Delay to allow ADC to settle (100ms or so)
//		- Convert ADC value and use a table lookup to determine device position
//		- Initialize canbus controller
//		- Set up can filters for "device position"
//		- Start a 2+ second counter ....
//		- If a can command to "Load new software" is received before timer ends
//			- enter mode to wait for commands
//				- erase page
//				- set access address
//				- load data
//				- erase/load option bytes
//				- set key based on unique ID
//				- calculate main program checksum and report results
//				- read bootloader rev (TBD)
//				- exit bootloader (jump to the primary program)
//		- Else if a command to start primary program, the jump to primary program
//			- jump to start of primary program
//		- Else (if timer expired)
//			- jump to start of primary program
//
////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_can.h"

#include "hothead.h"
#include "hothead_can.h"
#include "hothead_hw_init.h"
#include "hothead_gpio.h"

globalStruct	_gs; // all globals in a single struct

////////////////////////////////////////////////////////////////////////////////

#define BL_POSITION_ENTRIES 27
// data from the Hot Head Resistor Value and 12-bit ADC Value spec
const positionTableStruct _blDevicePositionTable[BL_POSITION_ENTRIES] = {
	{  108, 91 }, // max adc value for hotbed1 position
	{  294, 26 }, // max adc value for yoke 2 aux2
	{  452, 16 }, // max adc value for yoke 1 aux2
	{  635, 21 }, // max adc value for yoke 2 hot head 1
	{  804, 11 }, // max adc value for yoke 1 hot head 1
	{  943, 22 }, // max adc value for yoke 2 hot head 2
	{ 1082, 12 }, // max adc value for yoke 1 hot head 2
	{ 1228, 23 }, // max adc value for yoke 2 hot head 3
	{ 1389, 13 }, // max adc value for yoke 1 hot head 3
	{ 1563, 24 }, // max adc value for yoke 2 hot head 4
	{ 1751, 14 }, // max adc value for yoke 1 hot head 4
	{ 1946, 25 }, // max adc value for yoke 2 aux1
	{ 2140, 15 }, // max adc value for yoke 1 aux1
	{ 2345, 92 }, // max adc value for hotbed2 position
	{ 2544, 46 }, // max adc value for yoke 4 aux2
	{ 2723, 36 }, // max adc value for yoke 3 aux2
	{ 2901, 41 }, // max adc value for yoke 4 hot head 1
	{ 3065, 31 }, // max adc value for yoke 3 hot head 1
	{ 3200, 42 }, // max adc value for yoke 4 hot head 2
	{ 3317, 32 }, // max adc value for yoke 3 hot head 2
	{ 3439, 43 }, // max adc value for yoke 4 hot head 3
	{ 3557, 33 }, // max adc value for yoke 3 hot head 3
	{ 3667, 44 }, // max adc value for yoke 4 hot head 4
	{ 3780, 34 }, // max adc value for yoke 3 hot head 4
	{ 3906, 45 }, // max adc value for yoke 4 aux1
	{ 4034, 35 }, // max adc value for yoke 3 aux1
	{ MAX_ADC12, POSITION_UNPLUGGED } // max adc value an unplugged hothead
};

////////////////////////////////////////////////////////////////////////////////

void reportError4x8(uint8_t unit, uint8_t code, byte opt1, byte opt2, byte opt3, byte opt4)
{
	// store latest error
	_gs._error.val.fields.code = code;
	_gs._error.val.fields.unit = unit;
	_gs._error.val.fields.count++;
	_gs._error.val.fields.params[0] = opt1;
	_gs._error.val.fields.params[1] = opt2;
	_gs._error.val.fields.params[2] = opt3;
	_gs._error.val.fields.params[3] = opt4;
	canPackIntoTxQueue2x32(CAN_WRITE, _gs._devicePosition, CAN_MSG_REPORT_ERROR, NO_PAGE, FALSE, _gs._error.val.u32[0], _gs._error.val.u32[1]);
}

////////////////////////////////////////////////////////////////////////////////

void reportError2x16(uint8_t unit, uint8_t code, uint16_t opt1, uint16_t opt2)
{
	reportError4x8(unit, code, opt1 & 0xff, (opt1 >> 8) & 0xff, opt2 & 0xff, (opt2 >> 8) & 0xff);
}

////////////////////////////////////////////////////////////////////////////////

void reportError1x32(uint8_t unit, uint8_t code, uint32_t opt)
{
	reportError4x8(unit, code, opt & 0xff, (opt >> 8) & 0xff, (opt >> 16) & 0xff, (opt >> 24) & 0xff);
}

////////////////////////////////////////////////////////////////////////////////

void reportErrorNoParams(uint8_t unit, uint8_t code)
{
	reportError4x8(unit, code, 0, 0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////

void reportErrorCan(uint8_t unit, uint8_t code, canSwStruct *canPtr)
{
	reportError4x8(unit, code, canPtr->msgType, canPtr->msgId, canPtr->page, canPtr->numBytes);
}

////////////////////////////////////////////////////////////////////////////////

void getDevicePosition() {
	// routine to convert the input adcRaw into a devicePostion utilizing the built in table.
	// the table is a linear array of increasing uint16_t representing the maximum adc value
	// for a given position.  the position is defined in the table.
	// 
	// since the table itself can be reloaded by software, this code will also protect from going 
	// past the end of the table in case of a bad table to table load

	uint32_t i;
	uint16_t adcRaw;

	for (i=0; i<0xFFFFF; i++)adcRaw = ADC1->DR + i; // adcRead() ... value from 0 to 4095
	adcRaw = ADC1->DR;

	// adcPositionConvert
	for (i = 0; i < BL_POSITION_ENTRIES; i++) {
		if (adcRaw <= _blDevicePositionTable[i].adcRaw) {
			break;
		}
	}

	if (i == BL_POSITION_ENTRIES) {
		reportError1x32(ERROR_UNIT_ADC, ERROR_EXCEEDED_TABLE_LENGTH, (uint32_t)&_blDevicePositionTable[0]);
	}

	_gs._adcValues[0].adcRaw = adcRaw;
	_gs._devicePosition = _blDevicePositionTable[i].position;
}

////////////////////////////////////////////////////////////////////////////////

FLASH_Status erasePage(uint16_t page)
{
	FLASH_Status status = FLASH_COMPLETE;
	if (!BLOCK_FLASH_WRITES)
	{
		//replaced: FLASH_Unlock();
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
		status = FLASH_ErasePage((uint32_t)(FLASH_BASE + (page * FLASH_PAGE_SIZE)));
		//replaced: FLASH_Lock();
#define CR_LOCK_Set              ((uint32_t)0x00000080)
		FLASH->CR |= CR_LOCK_Set;
		if (status != FLASH_COMPLETE)	{
			reportError1x32(ERROR_UNIT_FLASH, ERROR_ERASE_FAILED, (uint32_t)status << 16 |  (uint32_t)page);
		}
	}
	return(status);
}

////////////////////////////////////////////////////////////////////////////////
void Wait(int timeInMicroseconds)
{
  for(int counter=0;counter<timeInMicroseconds;counter++)
  {
	  setPin(PIN_LED_150);
  }
  clearPin(PIN_LED_150);
}
void applicationMain(void)
{
	pFunction JumpToApplication;
	uint32_t jumpAddress;

	// deinitialize all periperipherals (ADC, CAN)
	RCC->APB2RSTR |= RCC_APB2Periph_ADC1;
	RCC->APB2ENR &= ~RCC_APB2Periph_ADC1;

	RCC->APB1RSTR |= RCC_APB1Periph_CAN1;
	RCC->APB1ENR &= ~RCC_APB1Periph_CAN1;

	// Initialize the user application Stack Pointer
	__set_MSP(*(__IO uint32_t*) CODE_BASE_ADDR);

	// Jump to the user application
	// The stack pointer lives at CODE_BASE_ADDR
	// The reset vector is at CODE_BASE_ADDR + 4
	jumpAddress = *(__IO uint32_t*) (CODE_BASE_ADDR + 4);
	JumpToApplication = (pFunction) jumpAddress;
	JumpToApplication();
}

/////////////////////////////////////////////////////////////////////////////////

void updateLedDisplay(void)
{
	// single LED lit at a time shifting left, then right, then back, etc
	// with a shift occuring every 16 calls
	// use bits [7:4] to determine display value

	uint32_t tmp = (++_gs._canTrafficCount >> 4) & 0x07;
	if (_gs._canTrafficCount & 0x80)	// if MSB is set, going backwards
	{
		_gs._ledDisplay = (0x80 >> tmp);
	}
	else
	{
		_gs._ledDisplay = (1 << tmp);
	}
	writeAllLeds(_gs._ledDisplay);
	}

/////////////////////////////////////////////////////////////////////////////////

int main(void) {
	uint32_t calculatedChecksum;
	uint32_t numCodeBytes;

    globalsInit();
    gpioInit();			// also turns on ALL LEDs
    adcInit();

	getDevicePosition();
	writeAllLeds(_gs._devicePosition);
	canInit();
	SET_CAN_INITIALIZED_BIT;

#ifdef UNOPTIMIZED_BOOTLOADER
	numCodeBytes = ((*(uint16_t *)FLASH_SIZE_REG_ADDR) * 1024) - (16 * FLASH_PAGE_SIZE);	// 12 for BL + 4 for tables == 16
#else
	numCodeBytes = ((*(uint16_t *)FLASH_SIZE_REG_ADDR) * 1024) - (8 * FLASH_PAGE_SIZE);		// 4 for BL + 4 for tables == 8
#endif

	calculatedChecksum = calculateChecksum32((uint32_t *)CODE_BASE_ADDR, numCodeBytes/4);

	canPackIntoTxQueue2x32(CAN_WRITE, _gs._devicePosition, CAN_MSG_EVENT_MESSAGE, CAN_EVENT_BOOTLOADER_ANNOUNCE,
								FALSE, calculatedChecksum, (*(uint32_t *)(CODE_BASE_ADDR + numCodeBytes - 8)));

	while (1)
	{
		// PROCESS CAN RX
		if (CAN1->RF1R & (uint32_t)0x03)    // check if any incomingRX messages in fifo1
		{
			canProcessRxQueue();			// something in the fifo, so process it
			CAN1->RF1R |= CAN_RF1R_RFOM1;	// release the fifo1 entry that was just processed;
			updateLedDisplay();				// update display to show additional transaction
		}

		// PROCESS CAN TX
		if (_gs._canTxQ.numMsg > 0)			// check if any outgoing messages to send
		{
			canProcessTxQueue();			// update queue, move from sw queue into hw and transmit
			updateLedDisplay();				// update display to show additional transaction
			continue;
		}
		setPin(PIN_LED_100);
		Wait(2000000);
		clearPin(PIN_LED_100);
		Wait(1000000);



	}
}

////////////////////////////////////////////////////////////////////////////////
