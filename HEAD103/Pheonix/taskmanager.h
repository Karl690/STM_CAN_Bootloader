/*
 * TaskManager.h
 *
 *  Created on: Apr 28, 2022
 *      Author: hyrel
 */
#pragma once
#include "main.h"
typedef void (*PFUNC)(void);

#define NUM_1000HZ              8
#define NUM_100HZ               8      // must remain 10 otherwise loop timing will not be correct
#define NUM_10HZ                8      // must remain 10 otherwise loop timing will not be correct
#define NUM_1HZ                 8     // must remain 10 otherwise loop timing will not be correct

#define SYSTICKS_PER_SECOND                 (NUM_1000HZ * NUM_100HZ * NUM_10HZ * NUM_1HZ)

#define ADC_CHANELS				8
#define ADC_MAX_COUNTDOWN 			0x2000


typedef enum {
	HZ_1000     = 0,
	HZ_100      = 1,
	HZ_10       = 2,
	HZ_1        = 3
} indexType;

enum {
	TASK_CAN_SEND_SOAPSTRING,
	TASK_FLASH_WRITE_SOAPSTRING,
	TASK_IDLE,
};

extern uint8_t  SoapString[0x400];
extern uint32_t HeartBeat;
extern uint32_t Seconds;               // needed for heartbeat (number of seconds since boot)
extern uint16_t SliceCnt;              // current slice being processed
extern uint16_t TaskTime[32];			// last execution time
extern uint16_t MaxTaskTime[32];       // Maximum execution time
extern uint16_t SliceOffset;
extern uint16_t ADC_Value[8];

extern uint16_t SmallTaskType;
extern uint32_t SmallTaskCount;

void func_SystickCallback(void);
void Spare (void);
void Execute1000HzTask(void);
void Execute100HzTask(void);
void Execute10HzTask(void);
void Execute1HzTask(void);
void BlinkHeartBeat(void);
void PWMSweeper(void);
void ClearSliceTimes(void);
void ReadAdcValue();

void SmallTask(void);


