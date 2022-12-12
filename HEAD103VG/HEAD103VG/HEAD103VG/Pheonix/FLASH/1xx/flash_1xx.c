#include "configure.h"

#ifdef HH103
#include "FLASH/flash.h"

#include "taskmanager.h"
uint32_t 	i, result, temp, address, length;

FLASH_Status 			flashStatus = FLASH_COMPLETE;
__IO uint32_t 			flashTemp= 0;



//Read the 8bytes data at the address in Flash.
void ReadFlash8Bytes(uint32_t addr, uint8_t* data) //data is 8bytes.
{
	for(i = 0; i < 8; i ++)
		data[i] = *(__IO uint8_t*)(addr+i);
}
//Read the 32bit data at the address in Flash
uint32_t ReadFlash(uint32_t addr)
{
	return  *(__IO uint32_t*)(addr);
}

//Write the 8bytes data at the address in Flash.
uint8_t WriteFlash8Bytes(uint32_t addr, uint8_t* data)
{
	result = SUCCESS;
	temp = BYTE2UINT32(data[3], data[2], data[1], data[0]);

	FLASH_Unlock();

	if(FLASH_ProgramWord(addr, temp) != FLASH_COMPLETE)
	{
		result = ERROR;
	}else {
		temp = BYTE2UINT32(data[7], data[6], data[5], data[4]);
		if(FLASH_ProgramWord(addr+ 4, temp) != FLASH_COMPLETE)
		{
			result = ERROR;
		}
	}

	FLASH_Lock();
	return result;
}
//Write the 32bit data at the address in Flash.
uint8_t WriteFlash(uint32_t address, uint32_t data)
{
	uint8_t result = SUCCESS;
	FLASH_Unlock();

	if(FLASH_ProgramWord(address, data) != FLASH_COMPLETE)
	{
		result = SUCCESS;
	}

	FLASH_Lock();
	return result;
}

// Erase the flash memory with StartAddress and length.
// 		StartAddress : the start Address of flash to erase
//		length : the length of range
//

uint8_t EraseFlash(uint32_t StartAddress, uint32_t size)
{
	uint8_t result = SUCCESS;
	FLASH_Unlock();

	uint16_t temp = (size / FLASH_PAGE_SIZE); // calculate the page size of erase range.
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	//FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPTERR | FLASH_FLAG_WRPRTERR);
	for(uint16_t i = 0; i < temp; i ++)
	{
		if(FLASH_ErasePage(StartAddress +  i * FLASH_PAGE_SIZE) != FLASH_COMPLETE)
		{
			result = ERROR;
			break;
		}
	}
	FLASH_Lock();
	return result;
}

void WriteFlashData(uint32_t addr, uint8_t* data, uint8_t size)
{
	FLASH_Unlock();
	if(FLASH_ProgramWord(addr, temp) != FLASH_COMPLETE)
	{
		result = ERROR;
	}else {
		temp = BYTE2UINT32(data[7], data[6], data[5], data[4]);
		if(FLASH_ProgramWord(addr+ 4, temp) != FLASH_COMPLETE)
		{
			result = ERROR;
		}
	}

	FLASH_Lock();
}

void ReadFlashData(uint32_t addr, uint32_t size, uint8_t* data)
{
	for(uint16_t i = 0; i < size; i ++) {
		data[i] = *(__IO uint8_t*)(addr  + i);
		if(data[i] == 0 || data[i] == 0xff) {
			data[i] = 0;
			break;
		}
	}
}
#endif
