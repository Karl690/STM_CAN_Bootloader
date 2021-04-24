////////////////////////////////////////////////////////////////////////////////
//
// File:	hyrel_can.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	include file for common public defines and functions of hyrel_can.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define CAN_SYSTEM_DEVICE_ID	0x00

#ifdef BOOTLOADER
#define CAN_NUM_TX_TRANSFERS_PER_SLICE	1
#else
#define CAN_NUM_TX_TRANSFERS_PER_SLICE	3
#endif

#define CAN_READ					0b00		// 0x00
#define CAN_WRITE					0b01		// 0x01
#define CAN_RETURN_DATA				0b10		// 0x02
#define CAN_WRITE_ACK				0b11		// 0x03

#define NO_PAGE						((byte)0)

#define CAN_BYTES_0					((byte)0)
#define CAN_BYTES_1					((byte)1)
#define CAN_BYTES_2					((byte)2)
#define CAN_BYTES_3					((byte)3)
#define CAN_BYTES_4					((byte)4)
#define CAN_BYTES_5					((byte)5)
#define CAN_BYTES_6					((byte)6)
#define CAN_BYTES_7					((byte)7)
#define CAN_BYTES_8					((byte)8)
#define CAN_BYTES_FF				((byte)0xff)

// WARNING:  IF A NEW COMMAND IS ADDED, UPDATE "canMsgIdToNumBytes" in hyrel_can.c

#define CAN_MSG_DEVICE_POSITION			((byte)0x00)
#define CAN_MSG_DEVICE_INFO				((byte)0x01)
#define CAN_MSG_FLASH_CONFIG			((byte)0x02)
#define CAN_MSG_UNIQUE_ID				((byte)0x03)
#define CAN_MSG_PRE_DEFINED_ALIASES		((byte)0x04)
#define CAN_MSG_USER_DEFINED_ALIASES	((byte)0x05)
#define CAN_MSG_STATUS					((byte)0x06)
#define CAN_MSG_HISTORY_RANGE_DEF		((byte)0x07)
#define CAN_MSG_HISTORY_RANGES			((byte)0x08)
#define CAN_MSG_HISTORY_COUNTERS		((byte)0x09)
#define CAN_MSG_PAGE_DEF				((byte)0x0a)
#define CAN_MSG_TABLE_START_OFFSETS		((byte)0x0b)
#define CAN_MSG_GLOBAL_STRUCT_INFO		((byte)0x0c)

#define CAN_MSG_CONTROL_WORD			((byte)0x10)
#define CAN_MSG_ADD_ALIAS				((byte)0x11)
#define CAN_MSG_REMOVE_ALIAS			((byte)0x12)
#define CAN_MSG_AUTO_STATUS_CONTROL		((byte)0x13)
#define CAN_MSG_LED_CONTROL				((byte)0x14)
#define CAN_MSG_LED_OVERRIDE			((byte)0x15)
#define CAN_MSG_EXTREMES				((byte)0x16)
#define CAN_MSG_ERROR_REPORT_INTERVAL	((byte)0x17)
#define CAN_MSG_SWITCH_DUTY				((byte)0x18)
#define CAN_MSG_SWITCH_PWM				((byte)0x19)
#define CAN_MSG_SWITCH_TEMP				((byte)0x1a)
#define CAN_MSG_KARL_FACTORS			((byte)0x1b)

#define CAN_MSG_MOTOR_ENABLE			((byte)0x20)
#define CAN_MSG_MICROSTEPS				((byte)0x21)
#define CAN_MSG_CURRENT_BOOST			((byte)0x22)
#define CAN_MSG_EXTRUSION_OVERRIDE_PCT	((byte)0x23)
#define CAN_MSG_EXTRUSION_STEPS_PER_NL	((byte)0x24)
#define CAN_MSG_EXTRUSION_MAIN			((byte)0x25)
#define CAN_MSG_EXTRUSION_UNPRIME		((byte)0x26)
#define CAN_MSG_EXTRUSION_PRIME			((byte)0x27)
#define CAN_MSG_EXTRUSION_MANUAL		((byte)0x28)
#define CAN_MSG_EXTRUSION_DWELL			((byte)0x29)
#define CAN_MSG_UNPRIME_DWELL_PRIME		((byte)0x2a)

#define CAN_MSG_FILL_BUFFER				((byte)0x30)
#define CAN_MSG_ACCESS_BUFFER			((byte)0x31)
#define CAN_MSG_COPY_ADDR_TO_BUFFER		((byte)0x32)
#define CAN_MSG_COPY_PAGE_TO_BUFFER		((byte)0x33)
#define CAN_MSG_COPY_BUFFER_TO_PAGE		((byte)0x34)
#define CAN_MSG_PAGE_CHECKSUM			((byte)0x35)
#define CAN_MSG_ACCESS_OPTION_BYTES		((byte)0x36)
#define CAN_MSG_START_PRIMARY_PROGRAM	((byte)0x37)

#define CAN_MSG_EVENT_MESSAGE			((byte)0x40)
#define CAN_MSG_LOOPBACK				((byte)0x41)
#define CAN_MSG_REPORT_ERROR			((byte)0x42)
#define CAN_MSG_AUTO_STATUS				((byte)0x43)


typedef enum {
	CAN_EVENT_DEVICE_ANNOUNCE			= 0,
	CAN_EVENT_BOOTLOADER_ANNOUNCE		= 1,
	CAN_EVENT_DEVICE_HEARTBEAT			= 2,
	CAN_EVENT_MANUAL_Z_MOVE				= 3,
	CAN_EVENT_PRIME_COMPLETE			= 4,
	CAN_EVENT_LIMIT_SWITCH_ON			= 5,
	CAN_EVENT_LIMIT_SWITCH_OFF			= 6
} canEventMsgType;

typedef enum {
	STATUS_PAGE_HEATER_TEMP				= 0,
	STATUS_PAGE_MOTOR_TEMP				= 1,
	STATUS_PAGE_SWITCH_FAN				= 2,
	STATUS_PAGE_SWITCH_HEATER			= 3,
	STATUS_PAGE_LIMITS					= 4,
	STATUS_PAGE_ADC_INFO_POSITION		= 5,
	STATUS_PAGE_ADC_INFO_RTD1			= 6,
	STATUS_PAGE_ADC_INFO_RTD2			= 7,
	STATUS_PAGE_ADC_INFO_RTD3			= 8,
	STATUS_PAGE_ADC_INFO_TEMP_SENSOR	= 9,
	STATUS_PAGE_ADC_RAW0_POSITION		= 10,
	STATUS_PAGE_ADC_RAW0_RTD1			= 11,
	STATUS_PAGE_ADC_RAW0_RTD2			= 12,
	STATUS_PAGE_ADC_RAW0_RTD3			= 13,
	STATUS_PAGE_ADC_RAW0_TEMP_SENSOR	= 14,
	STATUS_PAGE_ADC_RAW1_POSITION		= 15,
	STATUS_PAGE_ADC_RAW1_RTD1			= 16,
	STATUS_PAGE_ADC_RAW1_RTD2			= 17,
	STATUS_PAGE_ADC_RAW1_RTD3			= 18,
	STATUS_PAGE_ADC_RAW1_TEMP_SENSOR	= 19
} statusPageType;

#define HH_NUM_STATUS_PAGES						20

#define CAN_NUM_FILTERS							14
#define NUM_PRE_DEFINED_ALIASES					4
#define NUM_USER_DEFINED_ALIASES				8	// note, if this increases past 8, changed to hothead_can.c are needed to 
													// support more than 1 pg
													// also filters can at most support 9
#define ALIAS_PHYSICAL_FILTER_INDEX				0
#define ALIAS_BROADCAST_ALL_FILTER_INDEX		1
#define ALIAS_BROADCAST_TYPE_FILTER_INDEX		2
#define ALIAS_BROADCAST_YOKE_FILTER_INDEX		3
#define ALIAS_USER_DEFINED_START_FILTER_INDEX	4
#define ALIAS_IMMEDIATE_FILTER_INDEX			13

#define ALIAS_ALL_DEVICES				0
#define ALIAS_ALL_HOTBEDS				90
#define ALIAS_ALL_EXTRUDERS				100
#define ALIAS_UNUSED					0xff

#define FILTER_CONSTANT_VALUE       0x80000004
#define FILTER_MASK_CONSTANT        0xE0040004
#define FILTER_MASK_DEVICE          0x07F80000
#define FILTER_IMMEDIATE_BIT        0x00040000
#define FILTER_DEVICE_LSB_POSITION	19

uint32_t calculateChecksum8(uint8_t [], uint32_t);
uint32_t calculateChecksum16(uint16_t [], uint32_t);
uint32_t calculateChecksum32(uint32_t [], uint32_t);
void canInitFilter(CAN_TypeDef *, byte, byte, boolean, boolean);
boolean canPageOutOfRange(byte, byte);
byte canMsgIdToNumBytes(byte);
boolean canIsValidAlias(byte device);
void canAddUserDefinedAlias(byte, byte []);
void canRemoveUserDefinedAlias(byte, byte []);
canStruct *canGetTxQueueNextInPtr(void);
byte canPackIntoTxQueue2x32(byte, byte, byte, byte, boolean, uint32_t, uint32_t);
byte canPackIntoTxQueue4x16(byte, byte, byte, byte, boolean, uint16_t, uint16_t, uint16_t, uint16_t);
byte canPackIntoTxQueue8x8(byte, byte, byte, byte, boolean, byte []);
byte canAddToTxQueue(canSwStruct *);
byte canProcessTxQueue(void);
void canProcessTxQueueUntilEmpty();
byte canTransmit(CAN_TypeDef *, canHwStruct *);
void canGetMailboxData(CAN_FIFOMailBox_TypeDef *, canHwStruct *);
byte canReceive(CAN_TypeDef *, uint8_t, canHwStruct *);
byte canAddToImmediateRxQueue(void);
byte canAddToRxQueue(void);
void canUnpackCanHwStruct(canStruct *, canSwStruct *);
void canPrepNextRx(void);
void canAddToRxQueueNoReturn(void);
void canProcessRxQueueNoReturn(void);
void canProcessTxQueueNoReturn(void);
