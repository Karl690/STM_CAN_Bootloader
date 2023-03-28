#include "flash.h"

void checkAndClearFlashStatus(void)
{
	if ((FLASH->SR & 0xc0) != 0)
	{
		// if prior to this, there was a bad access to flash, it will be caught, reported, and cleared here ...
		// otherwise, future attempts to erase/write flash will fail
		FLASH->SR = 0x0c; // clear error flags in order to proceed
	}
}

uint8_t erase_flash()
{	
	uint8_t result = SUCCESS;
	FLASH_Unlock();

	uint16_t temp = (FLASH_LAST_SIZE / FLASH_PAGE_SIZE); // calculate the page size of erase range.
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	//FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPTERR | FLASH_FLAG_WRPRTERR);
	for (uint16_t i = 0; i < temp; i++)
	{
		if (FLASH_ErasePage(FLASH_LAST_ADDRESS +  i * FLASH_PAGE_SIZE) != FLASH_COMPLETE)
		{
			result = ERROR;
			break;
		}
	}
	FLASH_Lock();
	return result;	
}


uint32_t read_memory(uint32_t addr, uint8_t* data, uint32_t len) {
	uint32_t readBytes = 0;
	for (uint16_t i = 0; i < len; i++) {
		data[i] = *(__IO uint8_t*)(addr  + i);
		if (data[i] == 0xff) {			
			break;
		}
		else if (data[i] == 0x0)
		{
			readBytes++;
			break;
		}
		readBytes++;
	}
	return readBytes;
}

uint32_t write_memory(uint32_t addr, uint8_t* data, uint32_t len)
{
	checkAndClearFlashStatus();
	FLASH_Unlock();
	uint32_t writeBytes = 0;
	for (uint8_t i = 0; i < len; i++)
	{
		if (FLASH_ProgramOptionByteData(addr + i, data[i]) != FLASH_COMPLETE)
		{
			break;
		}
		writeBytes++;
	}
	
	FLASH_Lock();	
	return writeBytes;
}