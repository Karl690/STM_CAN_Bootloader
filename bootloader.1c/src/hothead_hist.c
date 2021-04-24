////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_hist.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	contains functions that deal the history (usage tracking) functions
//			of the hothead.
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "hothead.h"

void historyStartNewBank(byte *, boolean);

////////////////////////////////////////////////////////////////////////////////

void historyStartNewBank(byte *addr, boolean swap)
{
	FLASH_Status status;
	byte i;
	if (swap == TRUE)
	{
		if (addr == (byte *)HISTORY0_BASE_ADDR)
		{
			addr = (byte *)HISTORY1_BASE_ADDR;
		}
		else if (addr == (byte *)HISTORY1_BASE_ADDR)
		{
			addr = (byte *)HISTORY0_BASE_ADDR;
		}
		else
		{
			reportError1x32(ERROR_UNIT_HISTORY, ERROR_BAD_NEW_BANK_ADDR, (uint32_t)addr);
			return;
		}
	}
	if (!BLOCK_FLASH_WRITES)
	{
		if (addr == (byte *)HISTORY0_BASE_ADDR)
		{
			i = HISTORY0_PAGE_INDEX;
		}
		else if (addr == (byte *)HISTORY1_BASE_ADDR)
		{
			i = HISTORY1_PAGE_INDEX;
		}
		else
		{
			reportError1x32(ERROR_UNIT_HISTORY, ERROR_BAD_NEW_BANK_ADDR, (uint32_t)addr);
			return;
		}
		status = erasePage(_gs._pageNumDef[i]);
		if (status == FLASH_COMPLETE)
		{
			_gs._history.longTermCount[HIST_CNT_ERASE_CYCLES]++; // increment erase count;
			FLASH_Unlock();
			for (i=0; i<NUM_HISTORY_COUNTERS; i++)
			{
				FLASH_ProgramWord((uint32_t)(addr + (4 * i)), _gs._history.longTermCount[i]);
			}
			FLASH_Lock();
			_gs._history.bankAddr = addr;
			_gs._history.bankOffset = 4 * NUM_HISTORY_COUNTERS;	// just past the 32-bit counters;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void initializeHistoryBanks()
{
	// reset both history banks to initial state
	// need to set the erase counter to 0 in bank1 so that it won't appear to be newer than banko next time init is called
	// (would happen because 0xfffffff in bank1 would be greater than 0x00000001
	FLASH_Status status;
	historyStartNewBank((byte *)HISTORY0_BASE_ADDR, FALSE); 			// will erase bank0 and setup initial counters (erase counter to 1)
	status = erasePage(_gs._pageNumDef[HISTORY1_PAGE_INDEX]);	// will erase bank1
	if (status == FLASH_COMPLETE)
	{
		if (!BLOCK_FLASH_WRITES)
		{
			FLASH_Unlock();
			FLASH_ProgramWord((uint32_t)HISTORY1_BASE_ADDR, 0x00000000);				// will write bank1's erase counter to 0
			FLASH_Lock();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void historyInit(historyStruct *hist)
{	// call before main loop control starts
	byte i;
	uint16_t ofs, value;

	// history banks are pingponged and each bank contains a 32-bit counter as the first word in the bank.
	// this counter signifies the number of times a new bank has been started.  in normal operation, the
	// counters will differ by 1, and the bank with the higher counter value is the newest and the one to use.

	// determine which bank is currently used:   look at flashErase counter (first uint32 in each bank).  
	if (*((uint32_t *)HISTORY0_BASE_ADDR) == *((uint32_t *)HISTORY1_BASE_ADDR))
	{   // new part or hosed situation
		if (*((uint32_t *)HISTORY0_BASE_ADDR) == ERASED_WORD32)
		{ // both erased, so a brand new part -- write initial counters
			hist->initOkay = TRUE;
			initializeHistoryBanks();
		}
		else
		{ // something got lost/out of sync, so report error and block use of history
			hist->bankAddr = (byte *)HISTORY0_BASE_ADDR;
			hist->initOkay = FALSE;
		}
	}
	// check to see if counters differ by exactly one
	else if (*((uint32_t *)HISTORY0_BASE_ADDR) == (*((uint32_t *)HISTORY1_BASE_ADDR) + 1))
	{ // get prior counts from BANK0
		hist->bankAddr = (byte *)HISTORY0_BASE_ADDR;
		hist->initOkay = TRUE;
	}
	else if (*((uint32_t *)HISTORY1_BASE_ADDR) == (*((uint32_t *)HISTORY0_BASE_ADDR) + 1))
	{ // get prior counts from BANK1
		hist->bankAddr = (byte *)HISTORY1_BASE_ADDR;
		hist->initOkay = TRUE;
	}
	else
	{
		hist->initOkay = FALSE;
	}

	if (_gs._history.initOkay == FALSE)
	{
		reportErrorNoParams(ERROR_UNIT_HISTORY, ERROR_BAD_INIT);;
	}
	else
	{
		for (i=0; i<NUM_HISTORY_COUNTERS; i++)
		{
			hist->longTermCount[i] = *((uint32_t *)(hist->bankAddr + (4 * i)));	// grab last valid counts from flash
		}

		// search current bank until 0xffff is detected (or end of bank) and add any '1's to long term counts
		ofs = 4 * NUM_HISTORY_COUNTERS;  // first location after long term counters;
		while ((ofs < FLASH_PAGE_SIZE) && (( value = *((uint16_t *)(hist->bankAddr + ofs))) != ERASED_WORD16))
		{
			for (i=0; i<16; i++)
			{
				hist->longTermCount[i] += (value & 0x0001);
				value >>= 1;
			}
			ofs += 2;	// accessing data uint16_t interval;
		}
		if (ofs >= FLASH_PAGE_SIZE)
		{ // bank was completely filled, so initialize next bank and move on.
			historyStartNewBank(hist->bankAddr, TRUE);
		}
		else
		{
			hist->bankOffset = ofs;
		}

		// setup ranges for heater temp tracking
		//   first entry for temps less than 64
		//   next 1 to N-1 entries are for temps 32 greater than prior entry
		//   last entry (N) is MAX temp to capture everything else
		hist->tempRange[HIST_HEATER_TEMP_START_INDEX] = (64 << TEMP_FRAC_BITS) - 1;
		for (i=1; i<HIST_NUM_HEATER_TEMP_RANGES-1; i++)
		{
			hist->tempRange[HIST_HEATER_TEMP_START_INDEX+i] = hist->tempRange[HIST_HEATER_TEMP_START_INDEX+i-1] + (32 << TEMP_FRAC_BITS);
		}
		hist->tempRange[HIST_HEATER_TEMP_START_INDEX+HIST_NUM_HEATER_TEMP_RANGES-1] = MAX_TEMP;

		// setup ranges for motor temp tracking
		//   first entry for temps less than 64
		//   next 1 to N-1 entries are for temps 32 greater than prior entry
		//   last entry (N) is MAX temp to capture everything else
		hist->tempRange[HIST_MOTOR_TEMP_START_INDEX] = (32 << TEMP_FRAC_BITS) - 1;
		for (i=1; i<HIST_NUM_MOTOR_TEMP_RANGES-1; i++)
		{
			hist->tempRange[HIST_MOTOR_TEMP_START_INDEX+i] = hist->tempRange[HIST_MOTOR_TEMP_START_INDEX+i-1] + (32 << TEMP_FRAC_BITS);
		}
		hist->tempRange[HIST_MOTOR_TEMP_START_INDEX+HIST_NUM_MOTOR_TEMP_RANGES-1] = MAX_TEMP;
	}
}

////////////////////////////////////////////////////////////////////////////////

void historyCommitToFlashPart1(void)
{
	// this routine checks the short term counters to see if any have exceeded the predefined threshold.  if so,
	// a 16-bit value is written to flash, with a 1 set in the bit position of corresponding to each short term
	// counter that had exceeded the threshold (and the short term counter is reduced by the amount represented by
	// one 'tick' on the external flash.  additionally, the long term counters are incremented by one so they stay
	// in sync with what's been written to flash.
	int8_t i;
	_gs._historyValue = 0;
	// update every N second if there's any needing to be written
	if (SAVE_HISTORY || ((_gs._seconds & ~HIST_MASK) == 0))
	{	// only proceed at a fixed internal of seconds
		for (i=15; i>=0; i--)
		{
			_gs._historyValue <<= 1;
			if (_gs._history.shortTermCount[i] & HIST_MASK)
			{	// if large enough to trigger an update
				_gs._historyValue |= 1;
				_gs._history.shortTermCount[i] -= (1 << HIST_MASK_BITS);  // reduce counter by amount represent by one tick
				_gs._history.longTermCount[i]++;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void historyCommitToFlashPart2(void)
{
	_gs._extendedSliceTimeNeeded = MAX_EXTENDED_TIME;
	if ((!BLOCK_FLASH_WRITES) && (_gs._historyValue != 0))
	{	// at least one counter overflowed
		if (_gs._history.bankOffset >= FLASH_PAGE_SIZE)
		{ // bank was completely filled, so initialize next bank and move on.
			historyStartNewBank(_gs._history.bankAddr, TRUE);
		}
		else
		{
			FLASH_Unlock();
			FLASH_ProgramHalfWord(((uint32_t)(_gs._history.bankAddr + _gs._history.bankOffset)), _gs._historyValue);
			FLASH_Lock();
			_gs._history.bankOffset += 2;	// accessing data uint16_t interval;
			CLR_SAVE_HISTORY_BIT;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void historyHousekeeping(void)
{
	// needs to be called once per second
	// this routine will look at the current current temperature of the fusion chamber and stepper motor
	// and increment the short term counter for the bin that corresponds to the respective temperatures.
	// each increment of the short term count corresponds to one second spent at that temperature.
	byte i;
	if (!(IGNORE_RTD1 && IGNORE_RTD2))
	{ // have valid heater temp to update counts
		i = HIST_HEATER_TEMP_START_INDEX;
		while (_gs._switchControl[HEATER].actualTemp > _gs._history.tempRange[i])
		{
			i++;
		}
		_gs._history.shortTermCount[i]++;
	}
	if (!IGNORE_RTD3)
	{ // have valid motor temp to update counts
		i = HIST_MOTOR_TEMP_START_INDEX;
		while (_gs._switchControl[FAN].actualTemp > _gs._history.tempRange[i])
		{
			i++;
		}
		_gs._history.shortTermCount[i]++;
	}
	if (_gs._history.motorStepCount > HIST_MOTOR_STEPS_PER_SEC)
	{
		_gs._history.motorStepCount -= HIST_MOTOR_STEPS_PER_SEC;
		_gs._history.shortTermCount[HIST_MOTOR_STEPS_START_INDEX]++;
	}
}

////////////////////////////////////////////////////////////////////////////////
