/*
 * hyrel.c
 *
 *  Created on: Apr 28, 2022
 *      Author: hyrel
 *      this is the roundrobing scehdular
 *      to set the basic int interval search for
 *      HAL_SYSTICK_Config(SystemCoreClock
 *      it is in stm32h7xx_hal.c
 *      in the current version we set the value to 8000
 */
#include <stdlib.h>
#include "main.h"
#include "stdint.h"
#include "taskmanager.h"
#include "GPIO/pinout.h"
#include "ADC/adc.h"
#include "CAN/can.h"
uint32_t PWMSubCounter=0;
uint8_t  ch = 0;

//uint16_t                    	ExtendedSliceTimeNeeded = 0;   // down counter of extra slice times needed
//uint32_t                    	Seconds = 0;               // needed for heartbeat (number of seconds since boot)
//uint16_t                    	SliceCnt = 0;              // current slice being processed

uint32_t HeartBeat = 0;
uint32_t Seconds = 0;               // needed for heartbeat (number of seconds since boot)
uint16_t SliceCnt = 0;              // current slice being processed
uint16_t TaskTime[32]={0};			// last execution time
uint16_t MaxTaskTime[32]={0};       // Maximum execution time
uint16_t SliceOffset=0;
uint16_t ADC_Value[8] = {0};
unsigned 									GlobalSync = 1;//causes all timers to reset and synchronize, makes all hot heads on canbus time toghether
#define GLOBAL_SYNC                         (GlobalSync)
#define SET_GLOBAL_SYNC_BIT                 (GlobalSync = 1)
#define CLR_GLOBAL_SYNC_BIT                 (GlobalSync = 0)


void processEverySlice(void)
{   // all code in this method will be executed EVERY SYSTICK and therefore comes out of the
	// time budget for EVERY slice.   Only put code here that cannot exist in an actual slice

}


const PFUNC F1000HZ[NUM_1000HZ] =
{
		Spare,    // keep as last call in this array
		Spare, //ParseIncomingLineIntoGCodeArgs,//canProcessTxQueueNoReturn,
		Spare, //ConvertArgsToFloat,
		Spare, //CheckForUart6TxRx,//serialProcessor,
};

const PFUNC F100HZ[NUM_100HZ] =
{
		Spare,//loop_100Hz_simple_work, // keep as last call in this array
		Spare,//ParseIncomingGcodeLine,             // can't use slice 0 and this is time slot to execute the next slower slice
		Spare, //Sequencer,
		Spare, //CheckCanMsgWaitingFifo1,
		ProcessCanRxMessage,
		ProcessCanTxMessage, //Sequencer,
		Spare,
		Spare, //USBTxProcessor,

};

const PFUNC F10HZ[NUM_10HZ] =
{
		Spare,//ReportXYZLocation,
		Spare,            // can't use slice 0 and this is time slot to execute the next slower slice
		Spare,//loop_10Hz_simple_work,  // keep as last call in this array
		Spare,//soapstringController,
		Spare,//sendUpdateToHost,
		Spare,//checkBlockingWaits,
		Spare,//EdgeTriggerSendResults, // move into simple_work if space needed
		Spare, //ReadAdcValue,//BlinkLed,//checkForCompletedAbort,
};

const PFUNC F1HZ[NUM_1HZ] =
{
		Spare,
		Spare,//BlinkLed,
		Spare, //TestVCP,
		BlinkHeartBeat,
		Spare,
		Spare,
		Spare,
		Spare,
};
/*
 * to make the slice scedular more effecient, we are going to use a binary counter, not base 10
 *
 */
//void TaskManager()
void func_SystickCallback()
{
	if(!Initialized) return; //if it is not initialized, it should do nothing
	SliceCnt++;
	SliceOffset=SliceCnt&0x0003;  //precalculate the slice index into the jump table

	if(SliceOffset)
	{
		F1000HZ[SliceOffset]();//run the task
//		if(TIM6->CNT > TaskTime[SliceOffset])
//		{
//			TaskTime[SliceOffset]=TIM6->CNT;
//		}
		return;//toggle pin so we can see on Oscillosclope and exit
	}

	if(SliceCnt&0x001C)
	{
		SliceOffset=(SliceCnt>>2) & 0x0007;
		F100HZ[SliceOffset]();
		SliceOffset+=8;
//		if(TIM6->CNT > TaskTime[SliceOffset]) TaskTime[SliceOffset]=TIM6->CNT;
		return;//toggle pin so we can see on Oscillosclope and exit
	}



	if(SliceCnt&0x00E0)
	{
		SliceOffset=(SliceCnt>>5)  & 0x0007;
		F10HZ[SliceOffset]();
		SliceOffset+=16;
//		if(TIM6->CNT > TaskTime[SliceOffset]) TaskTime[SliceOffset]=TIM6->CNT;
		return;//toggle pin so we can see on Oscillosclope and exit
	}
	//return;
	SliceOffset=(SliceCnt>>8)  & 0x0007;
	F1HZ[SliceOffset]();
	SliceOffset+=24;
//	if(TIM6->CNT > TaskTime[SliceOffset])
//	{
//		TaskTime[SliceOffset]=TIM6->CNT;
//	}
	return;//toggle pin so we can see on Oscillosclope and exit

}

void Spare (void)
{
	// placeholder call for empty slice
}
void BlinkHeartBeat()
{

	HeartBeat++;
	if(HeartBeat&1)
	{ LED_HEARTBEAT_ON;}
	else
	{ LED_HEARTBEAT_OFF;}

}
void ClearSliceTimes()
{
	for(size_t i = 0; i < sizeof TaskTime; ++i)
	{
		TaskTime[i] = 0;
		MaxTaskTime[i]=0;
	}
}

