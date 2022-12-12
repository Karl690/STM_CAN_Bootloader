#include "configure.h"
#if defined(ST7735) && defined(USE_LCD)
#include "main.h"
#include <stddef.h>

void ST7735_Clear(uint16_t color);
void ST7735_FillRGBRect(uint32_t Xpos, uint32_t Ypos, uint16_t *pData, uint32_t Width, uint32_t Height);
#endif
