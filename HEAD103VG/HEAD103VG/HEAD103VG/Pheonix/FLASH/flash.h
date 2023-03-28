#pragma once
#include "configure.h"

#define FLASH_LENGTH							0x8000  //32kb
#define FLASH_PAGE_SIZE							0x400	//a page size is 0x400 in stm32f10x series.

#define FLASH_LAST_SIZE						0x400
#define FLASH_LAST_ADDRESS			(FLASH_BASE + FLASH_LENGTH - FLASH_LAST_SIZE)

//
//extern void ReadFlash8Bytes(uint32_t addr, uint8_t* data); //data is 8bytes.
//extern uint32_t ReadFlash(uint32_t addr);
//extern uint8_t WriteFlash8Bytes(uint32_t addr, uint8_t* data);
//extern uint8_t WriteFlash(uint32_t address, uint32_t data);
//extern uint8_t EraseFlash(uint32_t StartAddress, uint32_t size);


uint8_t erase_flash();
void checkAndClearFlashStatus();
uint32_t read_memory(uint32_t addr, uint8_t* data, uint32_t len);
uint32_t write_memory(uint32_t addr, uint8_t* data, uint32_t len);
