/*
 * flash.c
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */

#ifndef FLASH_FLASH_C_
#define FLASH_FLASH_C_

#include "flash.h"

uint32_t 	i, result, temp, address, length;

FLASH_Status 			flashStatus = FLASH_COMPLETE;
__IO uint32_t 			flashTemp= 0;
//////////////////////////////////////// Flash functions ///////////////////////////////////////////

/**
  * @brief  Unlocks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function unlocks Bank1 and Bank2.
  *         - For all other devices it unlocks Bank1 and it is equivalent
  *           to FLASH_UnlockBank1 function..
  * @param  None
  * @retval None
  */
void FLASH_Unlock(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/**
  * @brief  Locks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function Locks Bank1 and Bank2.
  *         - For all other devices it Locks Bank1 and it is equivalent
  *           to FLASH_LockBank1 function.
  * @param  None
  * @retval None
  */
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank1 */
  FLASH->CR |= CR_LOCK_Set;
}


/**
  * @brief  Clears the FLASH's pending flags.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices, this function clears Bank1 or Bank2’s pending flags
  *         - For other devices, it clears Bank1’s pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
}

/**
  * @brief  Returns the FLASH Bank1 Status.
  * @note   This function can be used for all STM32F10x devices, it is equivalent
  *         to FLASH_GetStatus function.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP or FLASH_COMPLETE
  */
FLASH_Status FLASH_GetBank1Status(void)
{

  if((FLASH->SR & FLASH_FLAG_BANK1_BSY) == FLASH_FLAG_BSY)
    return FLASH_BUSY;
  if((FLASH->SR & FLASH_FLAG_BANK1_PGERR) != 0)
      return FLASH_ERROR_PG;
  if((FLASH->SR & FLASH_FLAG_BANK1_WRPRTERR) != 0 )
	  return FLASH_ERROR_WRP;
  return  FLASH_COMPLETE;
}
/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @note   This function can be used for all STM32F10x devices,
  *         it is equivalent to FLASH_WaitForLastBank1Operation.
  *         - For STM32F10X_XL devices this function waits for a Bank1 Flash operation
  *           to complete or a TIMEOUT to occur.
  *         - For all other devices it waits for a Flash operation to complete
  *           or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{
	flashStatus = FLASH_COMPLETE;

  /* Check for the Flash Status */
	flashStatus = FLASH_GetBank1Status();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
	while((flashStatus == FLASH_BUSY) && (Timeout != 0x00))
	{
		flashStatus = FLASH_GetBank1Status();
		Timeout--;
  	}
	if(Timeout == 0x00 )
  	{
  		flashStatus = FLASH_TIMEOUT;
  	}
  /* Return the operation status */
	return flashStatus;
}


/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  /* Wait for last operation to be completed */
  if(FLASH_WaitForLastOperation(EraseTimeout) != FLASH_COMPLETE) return FLASH_ERROR_PG;
  /* if the previous operation is completed, proceed to erase the page */
	FLASH->CR|= CR_PER_Set;
	FLASH->AR = Page_Address;
	FLASH->CR|= CR_STRT_Set;

	/* Wait for last operation to be completed */
	FLASH_WaitForLastOperation(EraseTimeout);
	/* Disable the PER Bit */
	FLASH->CR &= CR_PER_Reset;

  /* Return the Erase Status */
	return FLASH_COMPLETE;
}

/**
  * @brief  Programs a word at a specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  /* Wait for last operation to be completed */
  flashStatus = FLASH_WaitForLastOperation(ProgramTimeout);

  if(flashStatus != FLASH_COMPLETE) return flashStatus;

	/* if the previous operation is completed, proceed to program the new first
	half word */
	FLASH->CR |= CR_PG_Set;

	*(__IO uint16_t*)Address = (uint16_t)Data;
	/* Wait for last operation to be completed */
	flashStatus = FLASH_WaitForLastOperation(ProgramTimeout);

	if(flashStatus == FLASH_COMPLETE)
	{
	  /* if the previous operation is completed, proceed to program the new second
	  half word */
	  flashTemp = Address + 2;

	  *(__IO uint16_t*) flashTemp = Data >> 16;

	  /* Wait for last operation to be completed */
	  flashStatus = FLASH_WaitForLastOperation(ProgramTimeout);

	  /* Disable the PG Bit */
	  FLASH->CR &= CR_PG_Reset;
	}
	else
	{
	  /* Disable the PG Bit */
	  FLASH->CR &= CR_PG_Reset;
	}

  /* Return the Program Status */
  return flashStatus;
}

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

	uint16_t temp = (size / FLASH_PAGE_SIZE) + 2; // calculate the page size of erase range.
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

void WriteSoapString(uint8_t* data, uint8_t page)
{
	if(page * 8 > FLASH_SOAPSTRING_SIZE) return;
	if(page == 0) {
		if(EraseFlash(FLASH_SOAPSTRING_OFFSET_ADDRESS, FLASH_SOAPSTRING_SIZE) == ERROR) {
			//SendCanMessage(CAN_, CAN_MSG_, data, 0)
		}
	}
	WriteFlash8Bytes(FLASH_SOAPSTRING_OFFSET_ADDRESS + page * 8, data);
}

void ReadSoapString(uint8_t* data, uint8_t page)
{
	if(page * 8 > FLASH_SOAPSTRING_SIZE) return;
	ReadFlash8Bytes(FLASH_SOAPSTRING_OFFSET_ADDRESS + page * 8, data);
}
#endif /* FLASH_FLASH_C_ */
