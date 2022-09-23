/*
 * flash.h
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */


#pragma once
#include "configure.h"
#include "main.h"


///////////////////////FLASH MACRO////////////////////////////////////////////////
/**
  * @brief  FLASH Status
  */

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;
/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE)
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

/* Delay definition */
#define EraseTimeout          ((uint32_t)0x000B0000)
#define ProgramTimeout        ((uint32_t)0x00002000)


#define FLASH_FLAG_BSY                 ((uint32_t)0x00000001)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000020)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PGERR               ((uint32_t)0x00000004)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR              ((uint32_t)0x00000001)  /*!< FLASH Option Byte error flag */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 Program error flag */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 Write protected error flag */

#define FLASH_LENGTH							0x20000  //128kb
#define FLASH_PAGE_SIZE							0x400	//a page size is 0x400 in stm32f10x series.

#define FLASH_SOAP_SIZE						0x400
#define FLASH_SOAP_START_ADDRESS			FLASH_BASE + 31 * FLASH_PAGE_SIZE //0x8007C00

#define BYTE2UINT32(a,b,c,d) 			((a << 24) + (b << 16) + (c << 8) + d)
#define BYTES2UINT32(a)					(((*(uint8_t*)(a+3)) << 24) + ((*(uint8_t*)(a+2)) << 16) + ((*(uint8_t*)(a+1)) << 8) + ((*(uint8_t*)(a+0)) << 0))


void FLASH_Unlock(void);
void FLASH_Lock(void);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetBank1Status(void);
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);

void ReadFlash8Bytes(uint32_t addr, uint8_t* data); //data is 8bytes.
uint32_t ReadFlash(uint32_t addr);
uint8_t WriteFlash8Bytes(uint32_t addr, uint8_t* data);
uint8_t WriteFlash(uint32_t address, uint32_t data);
uint8_t EraseFlash(uint32_t StartAddress, uint32_t size);
