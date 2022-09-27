#pragma once
#include "configure.h"
#ifdef HH103
#include "1xx/pins1xx.h"

#elif defined(ESR407)
#include "4xx/pins4xx.h"

#elif defined(PHEONIX750)
#include "7xx/pins7xx.h"
#endif

extern void GPIO_Init(void);
extern void Leds_Init(void);
