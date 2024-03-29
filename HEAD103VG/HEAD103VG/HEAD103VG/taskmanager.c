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
#include "configure.h"
#include "global.h"
#include "taskmanager.h"
#include "ADC/adc.h"
#include "ADC/AD_Definitions.h"
#include "CAN/can.h"
#include "FLASH/flash.h"
#include "SETTINGS/settings.h"
uint32_t PWMSubCounter=0;
uint8_t  ch = 0;

uint8_t  SoapString[0x400] = {0};
uint32_t HeartBeat = 0;
uint32_t Seconds = 0;               // needed for heartbeat (number of seconds since boot)
uint16_t SliceCnt = 0;              // current slice being processed
uint16_t TaskTime[32]={0};			// last execution time
uint16_t MaxTaskTime[32]={0};       // Maximum execution time
uint16_t SliceOffset=0;
uint16_t ADC_Value[8] = {0};

uint16_t CanMsgProcessorType = TASK_IDLE;
uint32_t CanMsgProcessorCount = 0xFFFF;
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
		ProcessCanRxMessage, //ProcessCanRxMessage,
		ProcessCanTxMessage, //ProcessCanTxMessage,
};

const PFUNC F100HZ[NUM_100HZ] =
{
		Spare,//loop_100Hz_simple_work, // keep as last call in this array
		Spare,//ParseIncomingGcodeLine,             // can't use slice 0 and this is time slot to execute the next slower slice
		Spare, //Sequencer,
		Spare, //CheckCanMsgWaitingFifo1,
		CanBusExtendedMessageProcessor,
		ProcessRawADC_Data,
		UpdateLeds,
		SetDutyCycle,

};

const PFUNC F10HZ[NUM_10HZ] =
{
		Spare,//ReportXYZLocation,
		SmoothDutyCycleControl,            // can't use slice 0 and this is time slot to execute the next slower slice
		Spare,//loop_10Hz_simple_work,  // keep as last call in this array
		Spare,//soapstringController,
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
	if(HeartBeat&1)	{ LED_HEARTBEAT_ON;}
	else { LED_HEARTBEAT_OFF;}

	CanMessageBuffer[0] = (uint8_t)(ActualTemperature & 0xff);
	CanMessageBuffer[1] = (uint8_t)(ActualTemperature >> 8);
	CanMessageBuffer[2] = (uint8_t)(ActualHeaterDutyCycle & 0xff);
	CanMessageBuffer[3] = (uint8_t)(ActualHeaterDutyCycle >> 8);
	CanMessageBuffer[4] = (uint8_t)(ActualFanDutyCycle & 0xff);
	CanMessageBuffer[5] = (uint8_t)(ActualFanDutyCycle >> 8);
	CanMessageBuffer[5] = (uint8_t)(HeadAuxAnalog & 0xff);
	CanMessageBuffer[7] = (uint8_t)(HeadAuxAnalog >> 8);

	CanAddTxBuffer(CAN_DEV_HOST, CAN_READ, CAN_MSG_HEARTBEAT, HeartBeat & 0x3, 0, CanMessageBuffer, 8);

}
void ClearSliceTimes()
{
	for(size_t i = 0; i < sizeof TaskTime; ++i)
	{
		TaskTime[i] = 0;
		MaxTaskTime[i]=0;
	}
}

void UpdateLeds()
{
	if(CanRxLedCountDown) {
		CanRxLedCountDown --;  LED_CAN_RX_ON;
	}
	else LED_CAN_RX_OFF;
	if(CanTxLedCountDown) { CanTxLedCountDown --;  LED_CAN_TX_ON;	}
	else LED_CAN_TX_OFF;
}
void CanBusExtendedMessageProcessor(void)
{
	switch(CanMsgProcessorType) {
	case TASK_CAN_READ_SOAPSTRING:
		if((CanMsgProcessorCount + 1) * 8 > SOAP_STRING_FLASH_SIZE) {
			CanMsgProcessorType = TASK_IDLE;
			break;
		}
		uint8_t readBytes = ReadSoapString(SOAP_STRING_BASE_ADDRESS + CanMsgProcessorCount * 8, CanMessageBuffer);

		CanAddTxBuffer(CAN_DEV_HOST, CAN_READ, CAN_MSG_READ_SOAPSTRING, 0, 0, CanMessageBuffer, readBytes);
		if (readBytes < 8) {//completed reading soap string
			CanMsgProcessorType = TASK_IDLE;
		}
		break;
	
	}
	CanMsgProcessorCount ++;
	if(CanMsgProcessorCount >= TASK_MAXCOUNT) CanMsgProcessorCount = 0;;
}


void SetHeaterDutyCycle()
{

}

uint8_t DutyCycleCounter = 0;//range is 0-7
#define HEATER_TRANSITION_RANGE 5.0f
#define HEATER_MAX_DUTYCYCLE	50
void SmoothDutyCycleControl()
{
	if(ActualFanDutyCycle != DesiredFanDutyCycle) {
		if(ActualFanDutyCycle > DesiredFanDutyCycle) ActualFanDutyCycle --;
		else ActualFanDutyCycle ++;
	}
	if(DesiredTemperature == 0) ActualHeaterDutyCycle = 0; //that is when the heater is off
	else if(ActualTemperature != MAX_TEMP &&  ActualTemperature != DesiredTemperature) {
		if (ActualTemperature > DesiredTemperature) ActualHeaterDutyCycle = 0;
		else if (ActualTemperature > DesiredTemperature - HEATER_TRANSITION_RANGE)
		{
			ActualHeaterDutyCycle = HEATER_MAX_DUTYCYCLE * (DesiredTemperature - ActualTemperature) / HEATER_TRANSITION_RANGE; //Duty Cycle is linearly reduced.
		}		
		else ActualHeaterDutyCycle = 50;
	}
}
void SetDutyCycle()
{
	DutyCycleCounter ++;
	DutyCycleCounter&=0x7;//if(DutyCycleCounter > 8) DutyCycleCounter = 0;

	uint8_t onOff = (uint8_t)(ActualFanDutyCycle && (DutyCycleCounter <= (ActualFanDutyCycle>>5))? 1: 0);
	BB_HSS1_FAN = onOff;
	BB_LED_HSS1_FAN = onOff;
	
	onOff = (uint8_t)(ActualHeaterDutyCycle && (DutyCycleCounter <= (ActualHeaterDutyCycle>>5))? 1: 0);	
	BB_HSS2_HEATER = onOff;
	BB_LED_HSS2_HEATER = onOff;
}
