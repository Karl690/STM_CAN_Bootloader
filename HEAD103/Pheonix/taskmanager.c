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
#include "taskmanager.h"
#include "GPIO/pinout.h"
#include "ADC/adc.h"
#include "CAN/can.h"
#include "FLASH/flash.h"
uint32_t PWMSubCounter=0;
uint8_t  ch = 0;

//uint16_t                    	ExtendedSliceTimeNeeded = 0;   // down counter of extra slice times needed
//uint32_t                    	Seconds = 0;               // needed for heartbeat (number of seconds since boot)
//uint16_t                    	SliceCnt = 0;              // current slice being processed


uint8_t  SoapString[0x400] = {0};
uint32_t HeartBeat = 0;
uint32_t Seconds = 0;               // needed for heartbeat (number of seconds since boot)
uint16_t SliceCnt = 0;              // current slice being processed
uint16_t TaskTime[32]={0};			// last execution time
uint16_t MaxTaskTime[32]={0};       // Maximum execution time
uint16_t SliceOffset=0;
uint16_t ADC_Value[8] = {0};

uint16_t SmallTaskType = TASK_IDLE;
uint32_t SmallTaskCount = 0xFFFF;
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
		ProcessCanTxMessage,
		Spare,
		Spare, //USBTxProcessor,

};

const PFUNC F10HZ[NUM_10HZ] =
{
		Spare,//ReportXYZLocation,
		Spare,            // can't use slice 0 and this is time slot to execute the next slower slice
		Spare,//loop_10Hz_simple_work,  // keep as last call in this array
		SmallTask,//soapstringController,
		Spare,//sendUpdateToHost,
		Spare,//checkBlockingWaits,
		Spare, //Sequencer,,//EdgeTriggerSendResults, // move into simple_work if space needed
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


void SmallTask(void)
{
	switch(SmallTaskType) {
	case TASK_CAN_SEND_SOAPSTRING:
		if((SmallTaskCount + 1) * 6 > FLASH_SOAP_SIZE) {
			SmallTaskType = TASK_IDLE;
			break;
		}
		//in this case , TaskCoutUp means soap string's offset address.
		//2bytes are address
		CanMessageBuffer[0] = SmallTaskCount & 0xFF;
		CanMessageBuffer[1] = (SmallTaskCount & 0xFF00) >> 8;
		//6bytes are data
		uint8_t size = 0;
		for(uint8_t i = 0; i < 6; i ++){
			CanMessageBuffer[i+2] = SoapString[SmallTaskCount * 6 + i];
			if(CanMessageBuffer[i+2] == 0) {
				break;
			}
			size ++;
		}

		CanAddTxBuffer(CAN_DEV_HOST, CAN_READ, CAN_MSG_SOAP_STRING, 0, 0, CanMessageBuffer, size + 2);
		if(size < 6) {
			SmallTaskType = TASK_IDLE;
		}
		break;
	case TASK_FLASH_WRITE_SOAPSTRING:
		if(SmallTaskCount == 0) {
			if(EraseFlash(FLASH_SOAP_START_ADDRESS, FLASH_PAGE_SIZE) == ERROR) {
				SmallTaskType = TASK_IDLE;
				break;
			}
		}
		if(SmallTaskCount * 8 >= FLASH_SOAP_SIZE) {
			SmallTaskType = TASK_IDLE;
			break;
		}
		if(WriteFlash8Bytes(FLASH_SOAP_START_ADDRESS + SmallTaskCount * 8, SoapString + SmallTaskCount * 8) == ERROR) {
			SmallTaskType = TASK_IDLE;
			break;
		}
		break;
	}
	SmallTaskCount ++;
}


