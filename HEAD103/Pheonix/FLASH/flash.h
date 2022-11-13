#pragma once
#include "configure.h"
#include "main.h"
#ifdef HH103
#include "1xx/flash_1xx.h"
#include "stm32f10x_adc.h"
#elif defined(ESR407)
#elif defined(PHEONIX750)
#include "7xx/flash_7xx.h"
#endif

#define FLASH_LENGTH							0x8000  //32kb
#define FLASH_PAGE_SIZE							0x400	//a page size is 0x400 in stm32f10x series.

#define FLASH_SOAP_SIZE						0x400
#define FLASH_SOAP_START_ADDRESS			(FLASH_BASE + FLASH_LENGTH - FLASH_SOAP_SIZE)


extern void ReadFlash8Bytes(uint32_t addr, uint8_t* data); //data is 8bytes.
extern uint32_t ReadFlash(uint32_t addr);
extern uint8_t WriteFlash8Bytes(uint32_t addr, uint8_t* data);
extern uint8_t WriteFlash(uint32_t address, uint32_t data);
extern uint8_t EraseFlash(uint32_t StartAddress, uint32_t size);
