////////////////////////////////////////////////////////////////////////////////
//
// File:			hyrel.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:			include file for common public defines and functions.
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define CAN_500KBPS
//#define CAN_1000KBPS

#define CHECKSUM_KEY 0x5555aaaa

typedef uint8_t byte;

typedef void (*PFUNC)(void);

typedef uint8_t	byte;
typedef uint16_t pinType;

#define PASS	0
#define FAIL	1

typedef enum {
	FALSE	= 0,
	TRUE	= 1
} boolean;

typedef enum {
		OFF		= 0,
		ON		= 1,
} onOff;

typedef enum {
		IN_FLOAT	= 0,
		IN_ANALOG	= 1,
		IN_PULLDOWN	= 2,
		IN_PULLUP	= 3,
		OUT_0		= 4,
		OUT_1		= 5
} boostCtrl;

typedef enum {
	TABLE_PAGE_INDEX		= 0,
	SOAPBOX_PAGE_INDEX		= 1,
	HISTORY0_PAGE_INDEX		= 2,
	HISTORY1_PAGE_INDEX		= 3
} pageIndexType;

#define HH_NUM_HISTORY_COUNTERS	16

////////////////////////////////////////////////////////////////////////////////
// HOTHEAD ADC RELATED /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define HH_MAX_ADC12 					0x0fff
#define HH_NUM_ADC_CHANNELS				5

#define ADC_NUM_SAMPLES     			10  // 10 values saved; toss high and low to get average
#define ADC_SHIFT_FOR_AVG   			3

typedef enum {
		POSITION	= 0,
		RTD1		= 1,
		RTD2		= 2,
		RTD3		= 3,
		TEMP_SENSOR	= 4
} adcChanType;

typedef struct {
	uint16_t			adcRaw;					// last raw value read from ADC
	int16_t				convRaw;				// converted value from last rawValue read
	uint16_t			adcAvg;					// average of the last 10 read after tossing high and low
	int16_t				convAvg;				// converted value using the rawAvg

	int16_t				rawHistory[ADC_NUM_SAMPLES];	// last N reads from ADC
#ifdef STM32F10X_LD
	uint8_t				index;					// index of last valid value in the history
	uint8_t				inputChannel;			// actual adc channel input selection
	boolean				allValid;				// goes TRUE after 10 reads (so that all averages are valid)
#endif
} adcStruct;


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD LIMIT SWITCH RELATED ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define HH_NUM_LIMITS					2

typedef struct {
	byte				pinState;				// state of the input limit pin from most recent read
	boolean				initialConditionTested; // flag to indicate the initial state has been tested (ie, not first time)

	pinType				pin;					// pin connecting to limit control (switch, hall effect sensor, etc)
} limitStruct;


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD EXTRUSION RELATED ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define EXTRUDE_DIR_FORWARD				1
#define EXTRUDE_DIR_REVERSE				0

#define HH_NUM_EXTRUSIONS 5
typedef enum {
		EXT_MAIN		= 0,					// primary extrusion control
		EXT_UNPRIME		= 1,					// unprime (withdraw material) extrusion control
		EXT_PRIME		= 2,					// prime (restart material flow) extrusion control
		EXT_MANUAL		= 3,					// manual extrusion control
		EXT_DWELL		= 4						// dwell (for time matching to 407 if needed)
} extrusionIndex;

typedef struct {
	union {
		struct {
			uint32_t 	rate; 					// rate of extrusion (will be scaled)
			uint32_t 	qty;					// quantity to be extrude (will be scaled)
		} u32;
		struct {
			float 		rate; 					// rate of extrusion (will be scaled)
			float 		qty;					// quantity to be extrude (will be scaled)
		} flt;
	} data;
	union {
		uint32_t u32;
		struct {
			unsigned	updateRate				: 1;
			unsigned	updateQty				: 1;
			unsigned	direction				: 1;
			unsigned	continuousExtrusion		: 1;
			unsigned	immediateExtrusion		: 1;
			unsigned	blockStep				: 1;	// whether to block the step signal to the motor (needed for dwell)
		} bit;									// bitwise mask for control
	} control;
	extrusionIndex		index;
	uint32_t			timeMS;					// time in milliseconds 20.12 format to complete move
	uint16_t			localScale;				// 16.0
	uint16_t			calcRate;
	uint16_t			calcSteps;
} extrusionStruct;

typedef struct {
	uint32_t			overridePct;			// scale factor to tweak rates/quantities (4.28 format)wait
	uint32_t			stepsPerNL;				// scale factor (steps/nL in 4.28 format) to convert rate to be steps/sec
	extrusionStruct		extrusion[HH_NUM_EXTRUSIONS];
	volatile uint16_t	stepCount;				// current count of steps remain of extrusion
	uint32_t			manualFwdCnt;			// count of how long switch has been held
	uint32_t			manualRevCnt;			// count of how long switch has been held
	uint16_t			manualPrescaler;		// to adjust how often to look at switch and/or report for Z moves (for hotbed only)
	uint32_t			totalSteps;				// for debug.....
} extrusionControlStruct;


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD MOTOR RELATUED //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
	byte				microsteps;				// 2-bit microstep setting
	uint16_t			maxRate;			// generate ERROR try to run faster than this
} motorStruct;


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD HIGH SIDE SWITCH RELATED ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define HH_NUM_SWITCHES					2
#define HH_FAN							0	// SW1 on hot head connector
#define HH_HEATER						1	// SW2 on hot head connector

typedef struct {
	byte				switchNum;				// which switch
	uint16_t			period; 				// period is either 100 for duty cycle cases or arbitrary value (timebase based on prescaler)
	uint16_t			dutyCycle;				// dutyCycle (when period is 100) or arbitrary transition count for arbitrary periods
	uint16_t			prescaler;				// prescaler terminal count
	boolean				variableDutyCycle;		// flag for controlling (fixed duty or f (temp)
	boolean				invertSignal;			// flag to indicate whether to invert the outputSignal (for an activeLow signal)
	boolean				usePulseTrain;			// flag to say whether or not to use pulse train (with fixed perion only)
	int16_t				actualTemp;				// actual temp of device associate with switch
	int16_t				targetTemp;				// target/desired temp
	int16_t				maxSafeTemp;			// Absolute maximum allowable temperature as set under external program control, shutdown heater and raise ERROR/Alarm if reach
	int16_t				powerFactor;			// Karl's K-factor/Power Factor for temperature control (default 125)
	int16_t				deltaTemperatureBiasCoeff;	// used to effect a larger duty cycle when delta temp is large
	int16_t				maxOvershoot;			// amount above targetTemp before switch is off completely.

	uint16_t			currentCnt;				// current counter value while counting up to "period"
	uint16_t			prescaleCnt;			// prescaler to reduce overall frequency
	pinType				pin;					// pin connecting to switch
	byte				switchStatusLedMask;	// LED used to indicate switch on
// XXX - check the usage of these two items.
	byte				state;					//working variable for the on/off state of the switch
	boolean             isHeater;				//is this a heater, just a simple control
} switchStruct;


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD CONTROL TABLES RELATED //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
	uint16_t	adcRaw;				// max adcValue for corresponding position
	uint16_t	position;			// the devicePosition
} positionTableStruct;

typedef struct {
	uint16_t	adcRaw;				// expected adcValue for specified temp
	int16_t		temp;				// temp (s10.5 format - 1/32 degree) at specified adcValue
} rtdTableStruct;

typedef struct {
	int16_t		temp;				// temp (s10.5 format - 1/32 degree) -- used for table to convert tep to a proper LED display
	byte		ledValue;			// value to display
	byte		ledMask;			// mask to allow partial update of the bits
} ledTempTableStruct;


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD TEMPERATURE RELATED /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define TEMP_FRAC_BITS					5
#define TEMP_SCALE 						(1 << TEMP_FRAC_BITS)
#define MIN_TEMP						(int16_t)0x8000  // max negative
#define MAX_TEMP						(int16_t)0x7fff  // max positive


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD AUTOSTATUS RELATED //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
	uint32_t			pageSelect;				// which data to send
	byte				rate;					// seconds between updates

	byte				secondsCnt;				// count of numbers of seconds past
	byte				prescaleCnt;			// subcounter for number of 100 Hz time slices
	boolean				dumpingStatus;			// flag to indicate it's time to output status
} autoStatusStruct;


////////////////////////////////////////////////////////////////////////////////
// HOTHEAD LED CONTROL RELATED /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum {
	LED_SHOW_DEFAULT				= 0x00,
	LED_SHOW_DEVICE_ID				= 0x01,
	LED_SHOW_NITE_RIDER				= 0x02,
	LED_SHOW_PROGRESS				= 0x03,
	LED_SHOW_STATUS					= 0x04,
	LED_SHOW_STATUS_DEVICE_ID		= 0x05,
	LED_SHOW_STATUS_NITE_RIDER		= 0x06,
	LED_SHOW_TEMP_RANGE				= 0x07,
	LED_SHOW_TEMP_VALUE				= 0x08,
	LED_SHOW_SW_REV					= 0x09,
	LED_SHOW_STATUS_AND_TEMP		= 0x0a,
	LED_SHOW_STATUS_AND_DEVICE_ID	= 0x0b
} ledShowType;

typedef struct {
	byte				displaySelect;			// which LED value to display
	byte				dutyCycle;				// for LED pwm
	byte				systemMask;				// mask for which bits to override
	byte				systemValue;			// override value
	byte				errorDisplayTimes;		// number of times to display error code (4 seconds per interation)
	byte				currValue;				// current value being display
	byte				danceValue;				// dancing led pattern (possibly selected for display)
	byte				tempValue;				// temparture led pattern (possibly selected for display)
	byte				statusValue;			// status led pattern (possibly selected for display)
	byte				progressValue;			// progress led pattern of the boot sequence(possibly selected for display)
	byte				errorUnitValue;			// error unit pattern (possibly selected for display)
	byte				errorCodeValue;			// error code pattern (possibly selected for display)
	byte				currentCnt;				// current count of seconds for display 2-part error code
	uint32_t			errorDisplayCnt;		// has to handle values up to 255 * 4 * 128 (approximate 4 seconds; 100Hz loop)
	uint16_t			canRxLedCnt;			// counter to know when to turn off Rx can bus activity indicator
	uint16_t			canTxLedCnt;			// counter to know when to turn off Tx can bus activity indicator
	uint16_t			stepLedCnt;				// counter to know when to turn off motor step activity indicator
} ledStruct;

////////////////////////////////////////////////////////////////////////////////
// HOTHEAD ERROR STATUS RELATED ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
	union {
		uint32_t u32[2];
		struct {
			byte		unit;					// error unit (see HH Spec for definitions)
			byte		code;					// error code (see HH Spec for definitions)
			uint16_t	count;
			byte		params[4];				// 4 bytes of data (see HH Spec for definitions)
		} fields;
	} val;
} errorStruct;


////////////////////////////////////////////////////////////////////////////////
// GLOBAL CANBUS STRUCTURES ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef union {
	uint8_t		u8[8];
	int8_t		i8[8];
	uint16_t	u16[4];
	int16_t		i16[4];
	uint32_t	u32[2];
	int32_t		i32[2];
	uint64_t	u64;
	int64_t		i64;
} payloadUnion;

typedef struct {
	unsigned fixed_b0	: 1;
	unsigned RTR		: 1;
	unsigned IDE		: 1;
	unsigned page		: 8;
	unsigned msgId		: 7;
	unsigned immediate	: 1;
	unsigned device		: 8;
	unsigned msgType	: 2;
	unsigned fixed_b100	: 3;

	unsigned numBytes	: 4;
	unsigned pad		: 28;

	payloadUnion payload;
} canSwStruct;

typedef struct {
	uint32_t IR;
	uint32_t DTR;
	uint32_t DLR;
	uint32_t DHR;
} canHwStruct;

typedef struct {
	union {
		canHwStruct hw __attribute__ ((aligned (8)));		// get on double word boundry for directy copying of data
		canSwStruct sw;
	};
} canStruct;

#ifdef STM32F10X_LD
#ifdef BOOTLOADER
#define CAN_TX_QUEUE_SIZE 256
#define CAN_RX_QUEUE_SIZE 1
#else
#define CAN_TX_QUEUE_SIZE 192
#define CAN_RX_QUEUE_SIZE 256
#endif
#endif
#ifdef STM32F4XX
#define CAN_TX_QUEUE_SIZE 512
#define CAN_RX_QUEUE_SIZE 512
#endif

typedef struct {
	canStruct	Q[CAN_TX_QUEUE_SIZE];
	uint16_t	nextIn;
	uint16_t	nextOut;
	uint16_t	numMsg;
} txQueueStruct;

typedef struct {
	canStruct	Q[CAN_RX_QUEUE_SIZE];
	uint16_t				nextIn;
	uint16_t				nextOut;
	uint16_t				numMsg;
} rxQueueStruct;

////////////////////////////////////////////////////////////////////////////////
// HOTHEAD FLASH RELATED ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define ERASED_WORD32				   ((uint32_t)0xffffffff)
#define ERASED_WORD16				   ((uint16_t)0xffff)

#define HH_FLASH_PAGE_SIZE				0x400

#define HH_POSITION_UNPLUGGED			0xFE
#define HH_POSITION_ENTRIES				32  // (32*4 = 128 bytes)
#define HH_RTD_ENTRIES					32  // (32*4 = 128 bytes)
#define HH_TEMP_SENSOR_ENTRIES			10  // (10*4 =  40 bytes)
#define HH_TEMP_2_LED_ENTRIES			16  // (16*4 =  64 bytes)
#define HH_PULSE_TRAIN_ENTRIES			51	// (covers 0 to 50 --> to cover 0 to 100 in 2% increments)  (51*8 = 408 bytes)

#define HH_GROUP_POSITION_MIN			101
#define HH_GROUP_POSITION_MAX			(HH_GROUP_POSITION_MIN + CAN_NUM_GROUPS)
#define HH_HOTBED_POSITION_MIN			91
#define HH_HOTBED_POSITION_MAX			99
#define HH_HOTBED_POSITION				91
#define HH_HOTBED_MAX_ALLOWABLE_TEMP	(100 << TEMP_FRAC_BITS)		// for HOTBED (devicePosition == 91 to 99)
#define HH_HEATER_MAX_ALLOWABLE_TEMP	(270 << TEMP_FRAC_BITS)		// for NON HOTBED
#define HH_FAN_MAX_ALLOWABLE_TEMP		(70 << TEMP_FRAC_BITS)
#define HH_MAX_EXTRUSION_RATE			10000						// 3200 is 1 rev/sec
#define HH_RTD_TEMP_DELTA				(15 << TEMP_FRAC_BITS)		// 15 per Karl - max number of degrees allowed 
																			// between RTD1 and RTD2 for fusion chamber (8 degrees)

#define HH_INFO_MEM_DEV_TYPE_INDEX						2
#define HH_INFO_MEM_DEV_REV_INDEX						1
#define HH_INFO_MEM_DEV_KEY_INDEX						3
#define HH_INFO_MEM_FLASH_PAGE_WRITE_PROTECTION_INDEX	4

#define HH_INFO_MEM_DEV_TYPE_OFFSET						(HH_INFO_MEM_DEV_TYPE_INDEX << 1)
#define HH_INFO_MEM_DEV_REV_OFFSET						(HH_INFO_MEM_DEV_REV_INDEX << 1)
#define HH_INFO_MEM_DEV_KEY_OFFSET						(HH_INFO_MEM_DEV_KEY_INDEX << 1)
#define	HH_INFO_MEM_FLASH_PAGE_WRITE_PROTECTION_OFFSET	)HH_INFO_MEM_FLASH_PAGE_WRITE_PROTECTION_INDEX << 1)

////////////////////////////////////////////////////////////////////////////////
// HOTHEAD CONTROL WORD ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Hot Head Control Word
// set via CAN with no params
typedef union {
	uint32_t	u32;
	struct {
		unsigned registered			: 1;
		unsigned swReset			: 1;
		unsigned globalSync			: 1;
		unsigned stopExtruder		: 1;
		unsigned pauseExtruder		: 1;
		unsigned stopOnError		: 1;
		unsigned saveHistory		: 1;
		unsigned clearError			: 1;
		unsigned ackOnWrite			: 1;
		unsigned invertDirection	: 1;
		unsigned enableMotor		: 1;
		unsigned disableMotor		: 1;
		unsigned ignoreRTD1			: 1;
		unsigned ignoreRTD2			: 1;
		unsigned ignoreRTD3			: 1;
		unsigned blockFlashWrites	: 1;
		unsigned primeExtruder		: 1;
		unsigned unprimeExtruder	: 1;
		unsigned gateExtruder		: 1;
		unsigned retractExtruder	: 1;
		unsigned canInitialized		: 1;
		unsigned sentAnnounce		: 1;
		unsigned materialOverTemp	: 1;
		unsigned motorOverTemp		: 1;
		unsigned extruding			: 1;
		unsigned processing			: 1;
		unsigned updateBootloader	: 1;
	} bit;
} controlWordUnion;

// the following must align to the controlWord bitfield definition in the HH.

#define HH_REGISTERED_BIT						((uint32_t)0x00000001)		// 		marks that head has been registered w/ system
#define HH_SW_RESET_BIT							((uint32_t)0x00000002)		//		tells software to reset itself
#define HH_GLOBAL_SYNC_BIT						((uint32_t)0x00000004)		//		tells software to reset loop counters to 0
#define HH_STOP_EXTRUDER_BIT					((uint32_t)0x00000008)		// 		tells software to enter a hw safe state

#define HH_PAUSE_EXTRUDER_BIT					((uint32_t)0x00000010)		// 		tells software to stop extrusion (no steps)
#define HH_STOP_ON_ERROR_BIT					((uint32_t)0x00000020)		//	  	tells software to go to a safe state on an error
#define HH_SAVE_HISTORY_BIT						((uint32_t)0x00000040)		// 		tells software to update history to flash
#define HH_CLEAR_ERROR_BIT						((uint32_t)0x00000080)		//		tells software to clean current error and proceed

#define HH_ACK_ON_WRITE_BIT						((uint32_t)0x00000100)		// 		tells software to send & expect) ACK packets on writes
#define HH_INVERT_DIRECTION_BIT					((uint32_t)0x00000200)		// 		tells software to invert the direction value
#define HH_ENABLE_MOTOR_BIT						((uint32_t)0x00000400)		// 		tells software to turn on the motor's enable pin
#define HH_DISABLE_MOTOR_BIT					((uint32_t)0x00000800)		// 		tells software to turn off the motor's enable pin

#define HH_IGNORE_RTD1_BIT						((uint32_t)0x00001000)		// 		tells software to ignore RTD1
#define HH_IGNORE_RTD2_BIT						((uint32_t)0x00002000)		// 		tells software to ignore RTD2
#define HH_IGNORE_RTD3_BIT						((uint32_t)0x00004000)		// 		tells software to ignore RTD3
#define HH_BLOCK_FLASH_WRITES_BIT 				((uint32_t)0x00008000)		//		tells software to block all writes to flash (ie, for a pending shutdown)

#define HH_PRIME_EXTRUDER_BIT					((uint32_t)0x00010000)		//		tells software to prime extruder with prime setting
#define HH_UNPRIME_EXTRUDER_BIT					((uint32_t)0x00020000)		//		tells software to unprime extruder with unprime setting
#define HH_GATE_EXTRUDER_BIT					((uint32_t)0x00040000)		// XXXX tells software to close extruder gate
#define HH_RETRACT_EXTRUDER_BIT					((uint32_t)0x00080000)		// XXXX tells software to retract a retractable head

// internal status
#define HH_CAN_INITIALIZED_BIT					((uint32_t)0x00100000)		// 		marks that the CAN unit is initialized
#define HH_SENT_ANNOUNCE_BIT					((uint32_t)0x00200000)		// 		marks that an Announce message has been sent
#define HH_MATERIAL_OVER_TEMP_BIT				((uint32_t)0x00400000)		// 		marks that the fusion chabmber is over the max allowed
#define HH_MOTOR_OVER_TEMP_BIT					((uint32_t)0x00800000)		// 		marks that the motor temp is over the max allowed

#define HH_EXTRUDING_BIT						((uint32_t)0x01000000)		// 		marks that a "real" extrude is underway
#define HH_PROCESSING_BIT						((uint32_t)0x02000000)		// 		marks that a slice is currently underway
#define HH_UPDATE_BOOTLOADER_BIT				((uint32_t)0x04000000)		//		tells software to allow bootloader pages to be written

#define XXX_BIT							((uint32_t)0x00000000)		// XXXX t

// ERROR UNIT range 0x01 to 0x3F to allow display on HOTHEAD LEDs (will always to on to indicate ERROR)
// ERROR_UNITS start at 0xC1 (for LED display...upper 2 LEDs plus one other must be on to show error over regular funtion
// ERROR and WARNING values also used as return codes for functions.

////////////////////////////////////////////////////////////////////////////////
// HOTHEAD ERROR CODES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ERROR_NONE									0x00

#define ERROR_UNIT_NONE								0x00
#define 			ERROR_CODE_NONE							0x00
#define ERROR_UNIT_CAN								0xC1
#define 			CAN_TX_OK								0x00
#define				CAN_RX_OK								0x00
#define 			WARNING_NOT_INITIALIZED					0x01
#define 			WARNING_RX_NO_MSG_WAITING				0x02
#define 			WARNING_TX_NOT_SENT						0x03
#define				WARNING_TX_WAITING_FOR_ACK				0x04
#define				WARNING_TX_NO_DEVICE_ID					0x05
#define				WARNING_TX_MAILBOX_FULL					0x06
#define				ERROR_SWITCH_MSG_TYPE					0x07
#define				ERROR_SWITCH_MSG_ID						0x08
#define 			ERROR_BAD_PAGE_NUM		 				0x09
#define 			ERROR_NUM_BYTES_MISMATCH	 			0x0A
#define				ERROR_RX_QUEUE_FULL						0x0B
#define				ERROR_TX_QUEUE_FULL						0x0C
#define				ERROR_ILLEGAL_ACCESS_ADDRESS			0x0D
#define				ERROR_TOO_MANY_ALIASES					0x0E
#define				ERROR_BAD_CHECKSUM						0x0F
#define				ERROR_PAGE_WRITE_FAILED					0x10
#define ERROR_UNIT_ADC								0xC2
#define 			ERROR_ADC_NOISE_PART1					0x01
#define 			ERROR_ADC_NOISE_PART2					0x02
#define 			ERROR_POSITION_CHANGE					0x03
#define				ERROR_LOST_POSITION_RESISTOR			0x04
#define				ERROR_CANT_DETERMINE_POSITION			0x05
#define				ERROR_RTD1_RTD2_TEMP_DELTA				0x06
#define				ERROR_EXCEEDED_TABLE_LENGTH				0x07
#define ERROR_UNIT_FLASH							0xC3
#define				ERROR_OPTION_BYTE_PROGRAMMING_FAILED	0x01
#define 			ERROR_ERASE_FAILED						0x02
#define 			ERROR_MISSING_TABLES					0x03
#define				ERROR_PRIMARY_PROGRAM_CORRUPT			0x04
#define ERROR_UNIT_LOOP 							0xC4
#define 			ERROR_BAD_VECTOR						0x01
#define 			ERROR_SLICE_OVERRUN						0x02
#define ERROR_UNIT_HEATER							0xC6
#define 			ERROR_MATERIAL_IS_OVER_TEMP				0x01
#define ERROR_UNIT_MOTION							0xC7
#define 			ERROR_MOTOR_IS_OVER_TEMP				0x01
#define 			ERROR_MAX_EXTRUSION_RATE_EXCEEDED	 	0x02
#define 			ERROR_EXTRUSION_RATE_OVERFLOW		 	0x03
#define 			ERROR_EXTRUSION_STEPS_OVERFLOW		 	0x04
#define ERROR_UNIT_HISTORY							0xC8
#define 			ERROR_BAD_NEW_BANK_ADDR					0x01
#define 			ERROR_BAD_INIT							0x02
