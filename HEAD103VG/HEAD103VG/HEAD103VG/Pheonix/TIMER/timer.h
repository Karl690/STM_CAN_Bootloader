#pragma once
#include "configure.h"
#include "main.h"

#define TIM1_MAX_SPEED_ARR 105-1


void init_TIM1(void);
void InitMotionTimer(TIM_TypeDef *PUlSETimer, IRQn_Type IRQn);