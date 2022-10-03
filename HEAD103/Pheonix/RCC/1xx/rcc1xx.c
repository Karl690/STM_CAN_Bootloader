#pragma once

#include "main.h"
#include "configure.h"
#include "rcc1xx.h"
#include "taskmanager.h"
volatile uint32_t usTicks=0;


void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2RSTR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2RSTR &= ~RCC_APB2Periph;
  }
}


void RCC_Init()
{
    // Update SystemCoreClock value
	SystemCoreClockUpdate();
    // Configure the SysTick timer to overflow every 1 us
    //SysTick_Config(SYSCLK_FREQ_72MHz / 1000000);
	SysTick_Config(SystemCoreClock / 2000);//SYSTICKS_PER_SECOND);
	//SysTick->LOAD = 72000-1;
	//SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    //Enable GPIOA clock
	RCC->APB2ENR |= (RCC_APB2Periph_GPIOA);		//enable clock for peripheral
	RCC->APB2RSTR &= ~(RCC_APB2Periph_GPIOA);	//remove reset from peripheral
	//Enable GPIOB clock
	RCC->APB2ENR |= (RCC_APB2Periph_GPIOB);
	RCC->APB2RSTR &= ~(RCC_APB2Periph_GPIOB);

	//Enable AFIOEN clock
	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN);
	RCC->APB2RSTR &= ~(RCC_APB2ENR_AFIOEN);

	//Enable ADC1 clock
	RCC->APB2ENR |= (RCC_APB2Periph_ADC1);
	RCC->APB2RSTR &= ~(RCC_APB2Periph_ADC1);
	//Enable CAN1 clock

	RCC->APB1ENR |= (RCC_APB1Periph_CAN1);
	RCC->APB1RSTR &= ~(RCC_APB1Periph_CAN1);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @param  None
  * @retval None
  */
void RCC_DeInit(void)
{
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
  RCC->CFGR &= (uint32_t)0xF8FF0000;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;

}

// SysTick_Handler function will be called every 1 us
void SysTick_Handler(uint32_t us)
{
    if (usTicks != 0)
    {
        usTicks--;
    }

    func_SystickCallback();
}
