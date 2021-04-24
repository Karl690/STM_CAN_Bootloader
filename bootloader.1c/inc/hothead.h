////////////////////////////////////////////////////////////////////////////////
//
// File:	bootloader.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	include for public bootloader related defines and functions
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define BOOTLOADER
#define SOFTWARE_MAJOR_REVISION	1
#define SOFTWARE_MINOR_REVISION	'c'

//#define UNOPTIMIZED_BOOTLOADER		// if defined, will assume BL takes 3K instead of 1K (CODE_BASE_ADDR moves)
#define DIRECT_IO_INITIALIZATION		// will directly write 6 registers to set up I/O

#define DEBUG_NO_MAIN_BOARD					0
#define DEBUG_USING_LIMIT2					0
#define DEBUG_USING_LIMIT2_FOR_HEATER		0
#define DEBUG_USING_LIMIT2_FOR_FAN			0
#define DEBUG_USING_LIMIT2_FOR_HEARTBEAT	0
#define DEBUG_USING_LIMIT2_FOR_STEP			0
#define DEBUG_USING_LIMIT2_FOR_PROCESSING	0

#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "hyrel.h"
#include "hyrel_can.h"
#include "hothead_hist.h"
#include "hothead_gpio.h"

#define HEARTBEAT_BIT						(LED_7)

#define LED_STATUS_MOTOR_STEP			    (LED_0)
#define LED_STATUS_MOTOR_EN				    (LED_1)
#define LED_STATUS_FAN_SWITCH				(LED_2)
#define LED_STATUS_HEATER_SWITCH			(LED_3)
#define LED_STATUS_ERROR					(LED_4)
#define LED_STATUS_CAN_RX					(LED_5)
#define LED_STATUS_CAN_TX					(LED_6)
#define LED_STATUS_HEARTBEAT			    (LED_7)

#define LED_PROGRESS_INITIALIZED			(LED_0)
#define LED_PROGRESS_TABLES_OK				(LED_1)
#define LED_PROGRESS_FOUND_ID				(LED_2)
#define LED_PROGRESS_ANNOUNCE_SENT			(LED_3)
#define LED_PROGRESS_REGISTERED				(LED_4)
#define LED_PROGRESS_CAN_RX					(LED_5)
#define LED_PROGRESS_CAN_TX					(LED_6)
#define LED_PROGRESS_HEARTBEAT			    (LED_7)

#define FLASH_PAGE_SIZE						HH_FLASH_PAGE_SIZE
#define WORKING_BUFFER_SIZE					(FLASH_PAGE_SIZE / 4)
 
#define INFO_MEMORY_ADDR					0x1FFFF800
#define UNIQUE_ID_REG_ADDR					0x1FFFF7E8
#define FLASH_SIZE_REG_ADDR					0x1FFFF7E0

#define INFO_MEM_DEV_TYPE_OFFSET			HH_INFO_MEM_DEV_TYPE_OFFSET
#define INFO_MEM_DEV_REV_OFFSET				HH_INFO_MEM_DEV_REV_OFFSET
#define INFO_MEM_DEV_KEY_OFFSET				HH_INFO_MEM_DEV_KEY_OFFSET

#define INFO_MEM_DEV_TYPE_INDEX				HH_INFO_MEM_DEV_TYPE_INDEX
#define INFO_MEM_DEV_REV_INDEX				HH_INFO_MEM_DEV_REV_INDEX
#define INFO_MEM_DEV_KEY_INDEX				HH_INFO_MEM_DEV_KEY_INDEX
 
#define FLASH_BASE_ADDR						FLASH_BASE	// 0x08000000
#define BOOTLOADER_BASE_ADDR				FLASH_BASE_ADDR

#ifdef UNOPTIMIZED_BOOTLOADER	// to allow debugginf with non optimized code
#define CODE_BASE_ADDR						0x08003000
#else
#define CODE_BASE_ADDR						0x08001000
#endif

#define TABLE_BASE_ADDR						(_gs._pageAddrDef[TABLE_PAGE_INDEX])
#define SOAPBOX_BASE_ADDR					(_gs._pageAddrDef[SOAPBOX_PAGE_INDEX])
#define HISTORY0_BASE_ADDR					(_gs._pageAddrDef[HISTORY0_PAGE_INDEX])
#define HISTORY1_BASE_ADDR					(_gs._pageAddrDef[HISTORY1_PAGE_INDEX])

#define POSITION_UNPLUGGED					HH_POSITION_UNPLUGGED
#define POSITION_ENTRIES					HH_POSITION_ENTRIES
#define RTD_ENTRIES							HH_RTD_ENTRIES
#define TEMP_SENSOR_ENTRIES					HH_TEMP_SENSOR_ENTRIES
#define TEMP_2_LED_ENTRIES					HH_TEMP_2_LED_ENTRIES
#define PULSE_TRAIN_ENTRIES					HH_PULSE_TRAIN_ENTRIES

#define POSITION_TABLE_OFFSET				0x0000
#define RTD1_TABLE_OFFSET					0x0080
#define RTD2_TABLE_OFFSET					0x0100
#define RTD3_TABLE_OFFSET					0x0180
#define TEMP_SENSOR_TABLE_OFFSET			0x0200
#define TEMP_2_LED_TABLE_OFFSET				0x0228
#define PULSE_TRAIN_TABLE_OFFSET			0x0268

#define POSITION_TABLE_ADDR					(TABLE_BASE_ADDR + POSITION_TABLE_OFFSET)
#define RTD1_TABLE_ADDR						(TABLE_BASE_ADDR + RTD1_TABLE_OFFSET)
#define RTD2_TABLE_ADDR						(TABLE_BASE_ADDR + RTD2_TABLE_OFFSET)
#define RTD3_TABLE_ADDR						(TABLE_BASE_ADDR + RTD3_TABLE_OFFSET)
#define TEMP_SENSOR_TABLE_ADDR				(TABLE_BASE_ADDR + TEMP_SENSOR_TABLE_OFFSET)
#define TEMP_2_LED_ADDR						(TABLE_BASE_ADDR + TEMP_2_LED_TABLE_OFFSET)
#define PULSE_TRAIN_ADDR					(TABLE_BASE_ADDR + PULSE_TRAIN_TABLE_OFFSET)
 
#define NUM_EXTRUSIONS						HH_NUM_EXTRUSIONS
#define NUM_SWITCHES						HH_NUM_SWITCHES
#define NUM_LIMITS							HH_NUM_LIMITS
#define FAN									HH_FAN
#define	HEATER								HH_HEATER
#define MAX_ADC12							HH_MAX_ADC12
#define NUM_ADC_CHANNELS					HH_NUM_ADC_CHANNELS

#define ADC_CHANNEL_POS						ADC_Channel_9
#define ADC_CHANNEL_RTD1					ADC_Channel_1
#define ADC_CHANNEL_RTD2					ADC_Channel_0
#define ADC_CHANNEL_RTD3					ADC_Channel_4
#define ADC_CHANNEL_TEMP_SENSOR				ADC_Channel_16

#define NUM_STATUS_PAGES					HH_NUM_STATUS_PAGES
 			
#define HOTBED_POSITION_MIN					HH_HOTBED_POSITION_MIN
#define HOTBED_POSITION_MAX					HH_HOTBED_POSITION_MAX
#define HOTBED_POSITION						HH_HOTBED_POSITION
#define HOTBED_MAX_ALLOWABLE_TEMP			HH_HOTBED_MAX_ALLOWABLE_TEMP
#define HEATER_MAX_ALLOWABLE_TEMP			HH_HEATER_MAX_ALLOWABLE_TEMP
#define FAN_MAX_ALLOWABLE_TEMP				HH_FAN_MAX_ALLOWABLE_TEMP

#define DEVICE_IS_A_HOTBED					((_gs._devicePosition >= HOTBED_POSITION_MIN) && (_gs._devicePosition <= HOTBED_POSITION_MAX))

#define REGISTRATION_TIME_INTERVAL			10			// 10 seconds to determine device position and get registered
#define MAX_EXTRUSION_RATE					HH_MAX_EXTRUSION_RATE	
#define MAX_MANUAL_EXTRUSION_COUNT			200
#define EXTRUSION_UPDATE_RATE_BIT			0x01
#define EXTRUSION_UPDATE_STEPS_BIT			0x02

#define AUTO_STATUS_CONTROL_MASK_BIT		0x01
#define AUTO_STATUS_CONTROL_RATE_BIT		0x02

#define MAX_HEATER_DUTY_CYCLE				100
#define RTD_TEMP_DELTA 						HH_RTD_TEMP_DELTA

// defaults for temperature controls for switches .... all can be overridden via canbus on a per switch basis
#define SWITCH_POWER_FACTOR					100
#define SWITCH_MAX_OVERSHOOT				(5 << TEMP_FRAC_BITS)
#define SWITCH_DELTA_TEMPERATURE_BIAS_COEFF	5

#define TIM1_CLOCK_FREQ 					72000000

#define NUM_1000HZ							4	// not restricted....larger the number, the less time per slice
#define NUM_100HZ							10	// must remain 10 otherwise loop timing will not be correct
#define NUM_10HZ							10	// must remain 10 otherwise loop timing will not be correct
#define NUM_1HZ								10	// must remain 10 otherwise loop timing will not be correct

#define SYSTICKS_PER_SECOND					(NUM_1000HZ * NUM_100HZ * NUM_10HZ * NUM_1HZ)

#define MAX_EXTENDED_TIME					(SYSTICKS_PER_SECOND << 1) // 1 seconds of extra time

typedef enum {
	HZ_1000		= 0,
	HZ_100		= 1,
	HZ_10		= 2,
	HZ_1		= 3
} indexType;

#define POSITION_SAMPLES_DELTA_AVG			18	// about 15mv - error allowed for any sample to average for devicePosition monitoring

#define NO_ERROR    						((_gs._led.errorUnitValue == ERROR_UNIT_NONE) && (_gs._led.errorCodeValue == ERROR_CODE_NONE))
#define DEVICE_POSITION_KNOWN				(_gs._devicePosition < _gs._devicePositionUnplugged)
#define MANUAL_EXTRUDE_FORWARD				(_gs._extrusion.manualFwdCnt > 0)
#define MANUAL_EXTRUDE_REVERSE				(_gs._extrusion.manualRevCnt > 0)
#define MANUAL_EXTRUDE						(MANUAL_EXTRUDE_FORWARD || MANUAL_EXTRUDE_REVERSE)
#define MOTOR_ENABLED						(((pinPort(PIN_ENABLEB)->ODR >> pinPinNum(PIN_ENABLEB)) & 0x01) == 0) // active low

#define REGISTERED							(_gs._controlWord.bit.registered)
#define SET_REGISTERED_BIT					(_gs._controlWord.bit.registered = 1)
#define CLR_REGISTERED_BIT					(_gs._controlWord.bit.registered = 0)

#define SW_RESET							(_gs._controlWord.bit.swReset)
#define SET_SW_RESET_BIT					(_gs._controlWord.bit.swReset = 1)
#define CLR_SW_RESET_BIT					(_gs._controlWord.bit.swReset = 0)

#define GLOBAL_SYNC							(_gs._controlWord.bit.globalSync)
#define SET_GLOBAL_SYNC_BIT					(_gs._controlWord.bit.globalSync = 1)
#define CLR_GLOBAL_SYNC_BIT					(_gs._controlWord.bit.globalSync = 0)

#define STOP_EXTRUDER						(_gs._controlWord.bit.stopExtruder)
#define SET_STOP_EXTRUDER_BIT				(_gs._controlWord.bit.stopExtruder = 1)
#define CLR_STOP_EXTRUDER_BIT				(_gs._controlWord.bit.stopExtruder = 0)

#define PAUSE_EXTRUDER						(_gs._controlWord.bit.pauseExtruder)
#define SET_PAUSE_EXTRUDER_BIT				(_gs._controlWord.bit.pauseExtruder = 1)
#define CLR_PAUSE_EXTRUDER_BIT				(_gs._controlWord.bit.pauseExtruder = 0)

#define STOP_ON_ERROR						(_gs._controlWord.bit.stopOnError)
#define SET_STOP_ON_ERROR_BIT				(_gs._controlWord.bit.stopOnError = 1)
#define CLR_STOP_ON_ERROR_BIT				(_gs._controlWord.bit.stopOnError = 0)

#define SAVE_HISTORY						(_gs._controlWord.bit.saveHistory)
#define SET_SAVE_HISTORY_BIT				(_gs._controlWord.bit.saveHistory = 1)
#define CLR_SAVE_HISTORY_BIT				(_gs._controlWord.bit.saveHistory = 0)

#define CLEAR_ERROR							(_gs._controlWord.bit.clearError)
#define SET_CLEAR_ERROR_BIT					(_gs._controlWord.bit.clearError = 1)
#define CLR_CLEAR_ERROR_BIT					(_gs._controlWord.bit.clearError = 0)

#define ACK_ON_WRITE						(_gs._controlWord.bit.ackOnWrite)
#define SET_ACK_ON_WRITE_BIT				(_gs._controlWord.bit.ackOnWrite = 1)
#define CLR_ACK_ON_WRITE_BIT				(_gs._controlWord.bit.ackOnWrite = 0)

#define INVERT_DIRECTION					(_gs._controlWord.bit.invertDirection)
#define SET_INVERT_DIRECTION_BIT			(_gs._controlWord.bit.invertDirection = 1)
#define CLR_INVERT_DIRECTION_BIT			(_gs._controlWord.bit.invertDirection = 0)

#define ENABLE_MOTOR						(_gs._controlWord.bit.enableMotor)
#define SET_ENABLE_MOTOR_BIT				(_gs._controlWord.bit.enableMotor = 1)
#define CLR_ENABLE_MOTOR_BIT				(_gs._controlWord.bit.enableMotor = 0)

#define DISABLE_MOTOR						(_gs._controlWord.bit.disableMotor)
#define SET_DISABLE_MOTOR_BIT				(_gs._controlWord.bit.disableMotor = 1)
#define CLR_DISABLE_MOTOR_BIT				(_gs._controlWord.bit.disableMotor = 0)

#define IGNORE_RTD1							(_gs._controlWord.bit.ignoreRTD1)
#define SET_IGNORE_RTD1_BIT					(_gs._controlWord.bit.ignoreRTD1 = 1)
#define CLR_IGNORE_RTD1_BIT					(_gs._controlWord.bit.ignoreRTD1 = 0)

#define IGNORE_RTD2							(_gs._controlWord.bit.ignoreRTD2)
#define SET_IGNORE_RTD2_BIT					(_gs._controlWord.bit.ignoreRTD2 = 1)
#define CLR_IGNORE_RTD2_BIT					(_gs._controlWord.bit.ignoreRTD2 = 0)

#define IGNORE_RTD3							(_gs._controlWord.bit.ignoreRTD3)
#define SET_IGNORE_RTD3_BIT					(_gs._controlWord.bit.ignoreRTD3 = 1)
#define CLR_IGNORE_RTD3_BIT					(_gs._controlWord.bit.ignoreRTD3 = 0)

#define BLOCK_FLASH_WRITES					(_gs._controlWord.bit.blockFlashWrites)
#define SET_BLOCK_FLASH_WRITES_BIT			(_gs._controlWord.bit.blockFlashWrites = 1)
#define CLR_BLOCK_FLASH_WRITES_BIT			(_gs._controlWord.bit.blockFlashWrites = 0)

#define PRIME_EXTRUDER						(_gs._controlWord.bit.primeExtruder)
#define SET_PRIME_EXTRUDER_BIT				(_gs._controlWord.bit.primeExtruder = 1)
#define CLR_PRIME_EXTRUDER_BIT				(_gs._controlWord.bit.primeExtruder = 0)

#define UNPRIME_EXTRUDER					(_gs._controlWord.bit.unprimeExtruder)
#define SET_UNPRIME_EXTRUDER_BIT			(_gs._controlWord.bit.unprimeExtruder = 1)
#define CLR_UNPRIME_EXTRUDER_BIT			(_gs._controlWord.bit.unprimeExtruder = 0)

#define GATE_EXTRUDER						(_gs._controlWord.bit.gateExtruder)
#define SET_GATE_EXTRUDER_BIT				(_gs._controlWord.bit.gateExtruder = 1)
#define CLR_GATE_EXTRUDER_BIT				(_gs._controlWord.bit.gateExtruder = 0)

#define RETRACT_EXTRUDER					(_gs._controlWord.bit.retractExtruder)
#define SET_RETRACT_EXTRUDER_BIT			(_gs._controlWord.bit.retractExtruder = 1)
#define CLR_RETRACT_EXTRUDER_BIT			(_gs._controlWord.bit.retractExtruder = 0)

#define CAN_INITIALIZED						(_gs._controlWord.bit.canInitialized)
#define SET_CAN_INITIALIZED_BIT				(_gs._controlWord.bit.canInitialized = 1)
#define CLR_CAN_INITIALIZED_BIT				(_gs._controlWord.bit.canInitialized = 0)

#define SENT_ANNOUNCE						(_gs._controlWord.bit.sentAnnounce)
#define SET_SENT_ANNOUNCE_BIT				(_gs._controlWord.bit.sentAnnounce = 1)
#define CLR_SENT_ANNOUNCE_BIT				(_gs._controlWord.bit.sentAnnounce = 0)

#define MATERIAL_OVER_TEMP					(_gs._controlWord.bit.materialOverTemp)
#define SET_MATERIAL_OVER_TEMP_BIT			(_gs._controlWord.bit.materialOverTemp = 1)
#define CLR_MATERIAL_OVER_TEMP_BIT			(_gs._controlWord.bit.materialOverTemp = 0)

#define MOTOR_OVER_TEMP						(_gs._controlWord.bit.motorOverTemp)
#define SET_MOTOR_OVER_TEMP_BIT				(_gs._controlWord.bit.motorOverTemp = 1)
#define CLR_MOTOR_OVER_TEMP_BIT				(_gs._controlWord.bit.motorOverTemp = 0)

#define EXTRUDING							(_gs._controlWord.bit.extruding)
#define SET_EXTRUDING_BIT					(_gs._controlWord.bit.extruding = 1)
#define CLR_EXTRUDING_BIT					(_gs._controlWord.bit.extruding = 0)

#define PROCESSING							(_gs._controlWord.bit.processing)
#define SET_PROCESSING_BIT					(_gs._controlWord.bit.processing = 1)
#define CLR_PROCESSING_BIT					(_gs._controlWord.bit.processing = 0)

#define UPDATE_BOOTLOADER					(_gs._controlWord.bit.updateBootloader)
#define SET_UPDATE_BOOTLOADER_BIT			(_gs._controlWord.bit.updateBootloader = 1)
#define CLR_UPDATE_BOOTLOADER_BIT			(_gs._controlWord.bit.updateBootloader = 0)

//#define XXX								(_gs._controlWord.bit.XXX)
//#define SET_XXX_BIT						(_gs._controlWord.bit.XXX = 1)
//#define CLR_XXX_BIT						(_gs._controlWord.bit.XXX = 0)

#define CONTINUOUS_EXTRUDE					(_gs._currentExtrusion->control.bit.continuousExtrusion)

typedef union {
	uint64_t u64;
	struct {
		//unsigned SWITCH_MSG_TYPE				: 1;
		//unsigned SWITCH_MSG_ID					: 1;
		//unsigned BAD_PAGE_NUM					: 1;
		//unsigned NUM_BYTES_MISMATCH				: 1;
		unsigned RX_QUEUE_FULL					: 1;
		unsigned TX_QUEUE_FULL					: 1;
		//unsigned ILLEGAL_ACCESS_ADDRESS			: 1;
		//unsigned TOO_MANY_ALIASES				: 1;
		//unsigned BAD_CHECKSUM					: 1;
		//unsigned ADC_NOISE_PART1				: 1;
		//unsigned ADC_NOISE_PART2				: 1;
		//unsigned POSITION_CHANGE				: 1;
		//unsigned LOST_POSITION_RESISTOR			: 1;
		//unsigned CANT_DETERMINE_POSITION		: 1;
		//unsigned RTD1_RTD2_TEMP_DELTA			: 1;
		//unsigned EXCEEDED_TABLE_LENGTH			: 1;
		//unsigned ERROR_PRIMARY_PROGRAM_CORRUPT	: 1;
		//unsigned ERASE_FAILED					: 1;
		//unsigned MISSING_TABLES					: 1;
		//unsigned BAD_VECTOR						: 1;
		//unsigned SLICE_OVERRUN					: 1;
		unsigned MATERIAL_IS_OVER_TEMP			: 1;
		unsigned MOTOR_IS_OVER_TEMP				: 1;
		unsigned MAX_EXTRUSION_RATE_EXCEEDED	: 1;
		unsigned EXTRUSION_RATE_OVERFLOW		: 1;
		unsigned EXTRUSION_STEPS_OVERFLOW		: 1;
		//unsigned BAD_NEW_BANK_ADDR				: 1;
		//unsigned BAD_INIT						: 1;
	} bit;
} errorFlagsUnion;

typedef struct {
	volatile controlWordUnion		_controlWord;				// collection of single bit flags/status. control portion can be modified
														// via CANbus (value + mask)

	byte					_ctrlIndex[4];				// index values for each of the control loops
	uint16_t				_sliceCnt; 					// current slice being processed
	int32_t					_seconds;					// number of seconds since powerup (more accurately, 

	byte					_devicePositionUnplugged;	// used initially to mark the position of an unplugged HH (no position resistor)
	byte					_devicePosition;			// actual device position
	byte					_registrationAttempts;		// number of attempts (one per second) of trying to register with the host

	adcChanType				_adcCurrentChannel;			// 5 ADC channels are processed in round robin fashion. index of current chan
	adcStruct 				_adcValues[NUM_ADC_CHANNELS];
	limitStruct 			_limits[NUM_LIMITS];
	boolean					_ignoreLimitState;			// used to for looking at limit pin value and not pin change
	extrusionControlStruct	_extrusion;
	motorStruct				_motor;
	byte					_currentUpdateSwitchPWM;		// which switch currently being updating PWM
	byte					_currentUpdateSwitchDutyCycle;	// which switch currently updating duty cycle
	switchStruct			_switchControl[NUM_SWITCHES];
	ledStruct				_led;
	autoStatusStruct		_autoStatusControl;
	errorStruct				_error;
	errorFlagsUnion			_sticky;
	int16_t					_rtdTempDelta;
	
	byte					_preDefinedAliases[NUM_PRE_DEFINED_ALIASES];	// current list of aliases
	byte					_userDefinedAliases[NUM_USER_DEFINED_ALIASES];		// current list of aliases
	byte					*_accessAddr;				// address for direct canbus access to flash (R or W) or ram (R only)

	canStruct				_canImmediateRx;			// buffer to hold next immedaite mode can message to process
	boolean					_canImmediateRxIsAvail;
	canStruct				_canRx;						// buffer to hold next can message to process
	boolean					_canRxIsAvail;
	rxQueueStruct			_canRxQ;
	txQueueStruct			_canTxQ;
	uint32_t				_workingBuffer[WORKING_BUFFER_SIZE];	// local storage ofr page data to/from canbus
	historyStruct			_history;
	extrusionStruct			*_currentExtrusion;
	uint16_t				_errorReportingInterval;		// updateable interval between the report of the same error message
	uint16_t				_errorReportingSecondsCnt;	// count for the interval
	uint16_t				_extendedSliceTimeNeeded;	// downcount of extra slices needed
	boolean					_flashUnlocked;
	uint32_t				_codeCheckSum;
	uint16_t				_pageNumDef[4];
	uint32_t				_pageAddrDef[4];
	uint32_t				_lastMainExtrusionRate;
	uint16_t 				_historyValue; 				// needed to bridge time as this logical routine takes too long for a single slice time
#ifdef BOOTLOADER
	uint32_t		_ledDisplay;
	uint32_t 		_canTrafficCount;
#endif
} globalStruct;

extern globalStruct _gs;
void reportError4x8(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void reportError2x16(uint8_t, uint8_t, uint16_t, uint16_t);
void reportError1x32(uint8_t, uint8_t, uint32_t);
void reportErrorNoParams(uint8_t, uint8_t);
void reportErrorCan(uint8_t, uint8_t, canSwStruct *);
void getStatusPage(byte, payloadUnion *);
uint16_t abs_int16(int16_t value);
void setMotorEnable(byte);
void setupExtrusion(extrusionStruct *);
FLASH_Status erasePage(uint16_t);

#ifdef BOOTLOADER
typedef void (*pFunction)(void);
void applicationMain(void);
#endif


