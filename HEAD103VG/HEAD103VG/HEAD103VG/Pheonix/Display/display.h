/*
 * display.h
 *
 *  Created on: May 6, 2022
 *      Author: hyrel
 */
#pragma once
#ifdef USE_LCD

#include <stdint.h>
#include "lcdspi.h"
#include "displaylist.h"

extern void Init_Display(SPI_LCD_HandleTypeDef* LcdHandler, uint8_t SpiIndex, uint8_t colorMode);

void Format_Int(uint8_t row, void* info, uint16_t*);
void Format_Int16(uint8_t row, void* info, uint16_t*);
void Format_Hex8(uint8_t row, void* info, uint16_t*);
void Format_Hex16(uint8_t row, void* info, uint16_t*);
void Format_Hex32(uint8_t row, void* info, uint16_t*);
void Format_Ascii(uint8_t row, void* info, uint16_t*);
void Format_String(uint8_t row, void* info, uint16_t*);
void Format_Float3_3(uint8_t row, void* info, uint16_t*);
void Format_MemoryDumpAscii(uint8_t row, void* info, uint16_t*);
void Format_MemoryDumpHex(uint8_t row, void* info, uint16_t*);
void Format_Boolean(uint8_t row, void* info, uint16_t*);
void Format_Bar(uint8_t row, void* info, uint16_t*);
void Format_MemoryDumpAscii(uint8_t row, void* info, uint16_t*);

void WriteGuiBuffer();
void UpdateScreen(SPI_LCD_HandleTypeDef*,LcdVariableInfo*);

#endif /* DISPLAY_H_ */
