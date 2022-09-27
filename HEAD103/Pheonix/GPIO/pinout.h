#pragma once
#include "configure.h"
#ifdef HH103
#include "1xx/pinoutHH103.h"

#elif defined(ESR407)
#include "4xx/pinout.h"

#elif defined(PHEONIX750)
#include "7xx/pinout.h"
#endif
