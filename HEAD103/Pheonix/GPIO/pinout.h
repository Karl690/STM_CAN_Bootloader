#pragma once
#include "configure.h"
#ifdef HH103
#include "1xx/pinoutHH103.h"

#elif defined(ESR407)
#include "4xx/pinout.h"

#elif defined(PHEONIX750)
#include "7xx/pinout.h"
#endif

#define PAout(n)   BIT_ADDR(GPIOA_ODR_ADDR,n)
#define PAin(n)    BIT_ADDR(GPIOA_IDR_ADDR,n)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_ADDR,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_ADDR,n)
