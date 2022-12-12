/*
 * flash.h
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */


#pragma once
#include "configure.h"
#include "main.h"

void ReadFlash8Bytes(uint32_t addr, uint8_t* data); //data is 8bytes.
uint32_t ReadFlash(uint32_t addr);
uint8_t WriteFlash8Bytes(uint32_t addr, uint8_t* data);
uint8_t WriteFlash(uint32_t address, uint32_t data);
uint8_t EraseFlash(uint32_t StartAddress, uint32_t size);

void ReadFlashData(uint32_t addr, uint32_t size, uint8_t* data);
