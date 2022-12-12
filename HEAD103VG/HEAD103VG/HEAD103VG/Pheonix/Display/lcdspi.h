#pragma once
#include "configure.h"
#ifdef USE_LCD

#ifdef HH103
#include "1xx/lcdspi_1xx.h"
#elif defined(PHEONIX750)
#include "7xx/lcdspi_7xx.h"
#elif defined(ESR407)
#include "4xx/lcdspi_4xx.h"
#endif

#endif //USE_LCD
