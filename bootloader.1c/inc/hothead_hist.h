////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_hist.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	include files for history related defines and public functions.
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define NUM_HISTORY_COUNTERS			HH_NUM_HISTORY_COUNTERS
#define HIST_CNT_ERASE_CYCLES	 		0
#define HIST_CNT_EXTRUDE_TEMP_BEL_063	1
#define HIST_CNT_EXTRUDE_TEMP_064_095	2
#define HIST_CNT_EXTRUDE_TEMP_096_127	3
#define HIST_CNT_EXTRUDE_TEMP_128_159	4
#define HIST_CNT_EXTRUDE_TEMP_160_191	5
#define HIST_CNT_EXTRUDE_TEMP_192_223	6
#define HIST_CNT_EXTRUDE_TEMP_224_255	7
#define HIST_CNT_EXTRUDE_TEMP_256_287	8
#define HIST_CNT_EXTRUDE_TEMP_288_319	9
#define HIST_CNT_EXTRUDE_TEMP_320_UP 	10
#define HIST_CNT_MOTOR_TEMP_BEL_031		11
#define HIST_CNT_MOTOR_TEMP_032_063		12
#define HIST_CNT_MOTOR_TEMP_064_095		13
#define HIST_CNT_MOTOR_TEMP_096_UP 		14
#define HIST_CNT_MOTOR_STEPS	 		15

#define HIST_MASK_BITS					5
#define HIST_MASK						(0xFFFFFFFF << HIST_MASK_BITS)	// ignore lower N bits of each local counter

#define HIST_NUM_HEATER_TEMP_RANGES		(HIST_CNT_EXTRUDE_TEMP_320_UP - HIST_CNT_EXTRUDE_TEMP_BEL_063 + 1)
#define HIST_NUM_MOTOR_TEMP_RANGES		(HIST_CNT_MOTOR_TEMP_096_UP - HIST_CNT_MOTOR_TEMP_BEL_031 + 1)

#define HIST_FLASH_ERASE_START_INDEX	HIST_CNT_ERASE_CYCLES 
#define HIST_HEATER_TEMP_START_INDEX	HIST_CNT_EXTRUDE_TEMP_BEL_063
#define HIST_MOTOR_TEMP_START_INDEX		HIST_CNT_MOTOR_TEMP_BEL_031
#define HIST_MOTOR_STEPS_START_INDEX	HIST_CNT_MOTOR_STEPS

#define HIST_MOTOR_STEPS_PER_SEC 		MAX_EXTRUSION_RATE

typedef struct {
	uint32_t	longTermCount[NUM_HISTORY_COUNTERS];
	byte		shortTermCount[NUM_HISTORY_COUNTERS];
	int16_t		tempRange[NUM_HISTORY_COUNTERS];
	byte		*bankAddr;
	uint16_t	bankOffset;
	uint32_t	motorStepCount;
	boolean		initOkay;
} historyStruct;
	

void historyInit(historyStruct *);
void historyHousekeeping(void);
void historyCommitToFlashPart1(void);
void historyCommitToFlashPart2(void);
