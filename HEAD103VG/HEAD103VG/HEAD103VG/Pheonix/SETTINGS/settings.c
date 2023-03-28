/*
 * settings.c
 *
 *  Created on: Apr 27, 2022
 *      Author: hyrel
 */
#include "configure.h"
#include "global.h"
#include "settings.h"
#include "FLASH/flash.h"

uint32_t soapstring_address = FLASH_LAST_ADDRESS;//init to first byte of sector 7

uint32_t current_read_address = 0;
uint32_t current_write_address = 0;
uint16_t ReadSoapString(uint32_t address, uint8_t* data)
{
	uint8_t NumberOfBytesRead = read_memory(address, (uint8_t*)data, 8);
	uint8_t dataSize = 0;
	for (uint8_t i = 0; i < NumberOfBytesRead; i++) {
		if (data[i] == 0x0 || data[i] == SOAP_STRING_LINE_DELIMETER) {
			current_read_address ++;
			dataSize++;
			break;
		}
		current_read_address ++;
		dataSize++;
	}	
	return dataSize;
}


void EraseSoapString()
{
	erase_flash();
	
	soapstring_address = SOAP_STRING_BASE_ADDRESS;
	current_read_address = soapstring_address;
}

uint32_t getNextWriteAddress(uint32_t start_address)
{
	uint32_t writePtr = start_address;
	uint32_t lastWritablePtr = (SOAP_STRING_BASE_ADDRESS + SOAP_STRING_FLASH_SIZE - SOAP_STRING_BUFFERSIZE);
	if (writePtr >= lastWritablePtr) return 0; // writable address is over ..
	while (*(uint8_t*)writePtr != 0xFF && writePtr < lastWritablePtr) writePtr++;
	return writePtr;
}

void AppendToSoapString(uint8_t* StringPointer, uint8_t size)
{
	uint32_t flashTargetAddress = FindFirst0xffInSoapString();
	uint16_t len = size;
	uint32_t limit_address = SOAP_STRING_BASE_ADDRESS + SOAP_STRING_FLASH_SIZE - len; // that is the address that subtracts the length of the string from the last soap string address.
	if ((flashTargetAddress < SOAP_STRING_BASE_ADDRESS) || flashTargetAddress > limit_address)
	{
		//fault condition, lvanakarl   add error message here
		//SendUsbVcpString("ERROR: Soapstring address is over");
		return;
	}
	
	checkAndClearFlashStatus();// clear the flash plese
	FLASH_Unlock();
	uint8_t index = 0;
	while (flashTargetAddress < limit_address)//do not loop past sector end
	{
		if (FLASH_ProgramOptionByteData((uint32_t)flashTargetAddress, StringPointer[index]) != FLASH_COMPLETE)
		{
			break;//fault so quit
		}
		if (index >= size) break;//found the null,which is end of line, so leave for now
		flashTargetAddress++; //point to the next flash target address
		index++;		
	}	
	FLASH_Lock();	
}

// see if we can write data in this address 
uint8_t ValidSoapStringPairStartAddress(uint32_t address)
{
	if (address >= SOAP_STRING_BASE_ADDRESS + SOAP_STRING_FLASH_SIZE - SOAP_STRING_BLOCK_SIZE) return 0; // it need to erase soap string flash sector.
	return 1;
}

uint32_t FindStartAddressOfSoapString()
{
	uint32_t SoapStringEndAddress = FindFirst0xffInSoapString();
	uint32_t SoapStringStartAddress = SoapStringEndAddress - 1;
	while (*(uint8_t*)SoapStringStartAddress != 0xff)
	{
		if (SoapStringStartAddress < SOAP_STRING_BASE_ADDRESS) return SOAP_STRING_BASE_ADDRESS;
		if (*(uint8_t*)SoapStringStartAddress == 0xA && *(uint8_t*)(SoapStringStartAddress - 1) == 0x2)
		{// that is the start of Soapstring
			return SoapStringStartAddress + 1;
		}
		SoapStringStartAddress--;
	}
	return SoapStringStartAddress + 1;
}
uint32_t FindNextValuePairStartAddress()
{
	int IndexOffsetPoiner = SOAP_STRING_FLASH_SIZE/2;//start by pointing to the middle of the flash segment
	uint32_t SoapStringstartPoint = SOAP_STRING_BASE_ADDRESS;//start at the beginning
	uint8_t c = *(uint8_t*)SoapStringstartPoint;
	if (*(uint8_t*)SoapStringstartPoint == 0xff) return SoapStringstartPoint;//empty flash so return the starting address
	IndexOffsetPoiner >>= 1; //divide by 2 please
	SoapStringstartPoint += IndexOffsetPoiner;
	uint8_t IsFindoutAddress = 0;
	//for (int bitcounter = 0; bitcounter < FlasHSizeInBits; bitcounter++)
	while (IndexOffsetPoiner >= SOAP_STRING_BLOCK_SIZE) 
	{
		if(*(uint8_t*)SoapStringstartPoint == 0xff)
		{
			//we have empty memory, so now we will check to see if the byte BEFORE is empty or not
			if(*(uint8_t*)(SoapStringstartPoint-1) != 0xff)
			{
				return SoapStringstartPoint;//the byte before is not empty,so this is the first empty byte
			}
			else
			{
				SoapStringstartPoint -= IndexOffsetPoiner;
			}
		}
		else
		{
			SoapStringstartPoint += IndexOffsetPoiner;
		}
		IndexOffsetPoiner >>= 1;//divide by 2 please
	}
	return SoapStringstartPoint;

}


uint32_t FindFirst0xffInSoapString()
{
	uint32_t returnAddress = SOAP_STRING_BASE_ADDRESS;
	uint32_t halfWaypoint = SOAP_STRING_FLASH_SIZE>>1;
	if (*(uint8_t*)returnAddress == 0xff)return returnAddress;//empty memory return the start of sector
	while (halfWaypoint > 0)
	{
		halfWaypoint >>= 1; //divide by 2 for next check
		if (*(uint8_t*)returnAddress == 0xff)  //still empty memory
		{  
			if (*(uint8_t*)(returnAddress - 1) != 0xff)break;//bingo found it
			returnAddress -= halfWaypoint; //adjust the 
			continue;//try again			
		}		
		returnAddress += halfWaypoint; //adjust the 
	}
	return returnAddress;//bingo found it
}


