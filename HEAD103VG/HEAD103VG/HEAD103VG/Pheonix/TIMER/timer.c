#include "timer.h"

void init_TIM1(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	//initClkAndResetAPB2(RCC_APB2Periph_TIM1);
	//Enabled TIM1 clock
	RCC->APB2ENR |= (RCC_APB2Periph_TIM1);
	RCC->APB2RSTR &= ~(RCC_APB2Periph_TIM1);

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;// TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1;
	TIM_TimeBaseInitStruct.TIM_Period = TIM1_MAX_SPEED_ARR;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	TIM_ARRPreloadConfig(TIM1, DISABLE);
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
	TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Disable);
	TIM_Cmd(TIM1, ENABLE);
}


void InitMotionTimer(TIM_TypeDef *PUlSETimer, IRQn_Type IRQn)
{
	//NVIC_InitTypeDef NVIC_InitStructure;//for the interrupts
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_Period = 100-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);	
	
	TIM_ARRPreloadConfig(PUlSETimer, DISABLE);
	
	TIM_SelectSlaveMode(PUlSETimer, TIM_SlaveMode_External1);	//sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
	TIM_SelectInputTrigger(PUlSETimer, TIM_TS_ITR0);	//sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	//HAL_TIM_SlaveConfigSynchro(htim, &sSlaveConfig);
	TIM_SelectOutputTrigger(PUlSETimer, TIM_TRGOSource_Reset); //sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	TIM_SelectMasterSlaveMode(PUlSETimer, TIM_MasterSlaveMode_Disable); //sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	PUlSETimer->EGR |= TIM_EGR_UG; // clear update flags
	TIM_ITConfig(PUlSETimer, TIM_IT_Update, ENABLE);

	NVIC_EnableIRQ(IRQn);
	TIM_Cmd(PUlSETimer, ENABLE); // start off disabled.... wait for first update
}


void TIM2_IRQHandler()
{
}
void TIM3_IRQHandler()
{
}
void TIM4_IRQHandler()
{
}
void TIM8_IRQHandler()
{
}
void TIM15_IRQHandler()
{
}