

//////////////////////////////////////            GLOBAL Contants 							///////////////////////////
#define SYSCLK_FREQ_72MHz  72000000

#define MAX_COUNTDOWN 0x2000

#define HEAD_POSITION_ENTRIES 		27
#define HEAD_01 11
#define HEAD_02 12
#define HEAD_03 13
#define HEAD_04 14

#define ALL_DEVICE 0

#define MAX_ADC 					0x0fff
#define POSITION_UNPLUGGED			0x3F
#define CAN_DEV_ANALYST				0x3E

typedef struct {
	uint16_t	adcRaw;			// max adcValue for corresponding position
	uint8_t	position;			// the devicePosition
} HEADPOSITIONTABLE;

#define BYTE2UINT32(a,b,c,d) ((a << 24) + (b << 16) + (c << 8) + d)



//////////////////////////////////////            RCC MACRO 							///////////////////////////
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB1Periph_CAN1              ((uint32_t)0x02000000)
#define RCC_APB2Periph_ADC1              ((uint32_t)0x00000200)

#define WaitMicrosecond(us) 		({usTicks = us; while (usTicks);})

//////////////////////////////////////            GPIO MACRO 							///////////////////////////
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */


#define CAN_A_RX				GPIO_Pin_11
#define CAN_A_TX				GPIO_Pin_12
#define LED_A_HEARTBEAT 		GPIO_Pin_15



#define LED_B_CANRXERROR 		GPIO_Pin_5
#define LED_B_CANRX 			GPIO_Pin_4
#define LED_B_CANTX 			GPIO_Pin_3


#define LED_B_MOTERON 			GPIO_Pin_12
#define LED_B_MOTERMOVE 		GPIO_Pin_13
#define LED_B_FANON 			GPIO_Pin_14
#define LED_B_HEATERON 			GPIO_Pin_15

#define SWITCH_B_LEFT			GPIO_Pin_7
#define SWITCH_B_RIGHT			GPIO_Pin_8

#define LED0 					LED_A_HEARTBEAT
#define LED1 					LED_B_CANTX
#define LED2 					LED_B_CANRX
#define LED3 					LED_B_CANRXERROR
#define LED4 					LED_B_HEATERON
#define LED5 					LED_B_FANON
#define LED6 					LED_B_MOTERMOVE
#define LED7 					LED_B_MOTERON

#define LED_HEADPOS_00 			LED4
#define LED_HEADPOS_01 			LED5
#define LED_HEADPOS_02 			LED6
#define LED_HEADPOS_03 			LED7


//How to set the GPIO Control Register(CRL, CRH)?
//
//each bit has a definition of Input/output/pullup/pulldown
//the bit is configured/defined in 2 control registers
//each I/o bit has 2 bits assigned to define how the bit is used

//Mode Value is
//	00: Input Mode
//	01: Out Mode (Max speed 10MHz)
//	10: Out Mode (Max speed 2MHz)
//	11: Out Mode (Max speed 50MHz)

//CNF Value is
// if Mode value is 0(Input Mode),
//   00:Analog Mode, 01:Float Input(reset Mode), 10: Input With pull-up/pull-down, 11:Reserved
// Otherwise, 00: General purpose output push-pull, 01:General purpose output Open drain,
//				10:Alternate function output Push-pull, 11: Aleternate function output Open-drain.
// CRH and CRL is 32bit register
// CRH: [CNF15][MOD15]|[CNF14][MOD14]|[CNF13][MOD13]|........|[CNF8][MOD8]
// CRL: [CNF7][MOD7]|[CNF6][MOD6]|[CNF6][MOD6]|........|[CNF0][MOD0]


//for LED_MOTERON(PB12), LED_MOTERMOVE(PB13), LED_FANON(PB14),LED_HEATERON(PB15)
//  CNF= 00[Out Mode 2Mz] , Mode = 10[General purpose output push-pull] => 0x2
//ADC(PB1)
//	CNF= 00[Analog Mode], Mode = 00[Input Mode]=> 0x0
#define PortBControlBits8_15 			0x22220008

//LEDB_CANTX(PB3), LEDB_CANRX(PB4), LEDB_CANRXERROR(PB5),
//SWT_B_ON and SWT_B_OFF is Input pull-up/pull-down ( CNF=10, Mode=00=>0x8)
#define PortBControlBits0_7 			0x44222444

//PA11 is RX for CAN1(0b1000=0x8 [CNF=10, MODE=00]), PA12 is TX for CAN1 (0b1011=0xB [CNF=10 , MODE=11])
//LED_HEARTBEAT(PA15)
#define PortAControlBits8_15 			0x200B8000

#define TurnOnHeartBeatLed			(GPIOA->BSRR = LED_A_HEARTBEAT)
#define TurnOffHeartBeatLed			(GPIOA->BRR = LED_A_HEARTBEAT)


#define SetPin(Port, Pin)			(Port->BSRR = Pin)
#define ClearPin(Port, Pin)			(Port->BRR = Pin)
#define TogglePin(Port, Pin)		(Port->ODR ^= Pin)
#define ReadPin(Port, Pin) 			(Port->IDR & Pin)


#define SetCanTxLed					(GPIOB->BSRR = LED_B_CANTX)
#define ClearCanTxLed				(GPIOB->BRR  = LED_B_CANTX)
#define SetCanRxLed					(GPIOB->BSRR = LED_B_CANRX)
#define ClearCanRxLed				(GPIOB->BRR  = LED_B_CANRX)
/////////////////////////////////////            CAN Contants & Structure 							////////////////////////////////

/**
  * @brief  CAN Rx message structure definition
  */

typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that
                        will be received. This parameter can be a value of
                        @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to
                        0xFF. */

  uint8_t FMI;     /*!< Specifies the index of the filter the message stored in
                        the mailbox passes through. This parameter can be a
                        value between 0 to 0xFF */
} CanRxMsg;


/**
  * @brief  CAN Tx message structure definition
  */

typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that
                        will be transmitted. This parameter can be a value
                        of @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the message that will
                        be transmitted. This parameter can be a value of
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be
                        transmitted. This parameter can be a value between
                        0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be transmitted. It ranges from 0
                        to 0xFF. */
} CanTxMsg;


/** @defgroup CAN_Private_Defines
  * @{
  */

/* CAN Master Control Register bits */

#define MCR_DBF      ((uint32_t)0x00010000) /* software master reset */

/* CAN Mailbox Transmit Request */
#define TMIDxR_TXRQ  ((uint32_t)0x00000001) /* Transmit mailbox request */

/* CAN Filter Master Register bits */
#define FMR_FINIT    ((uint32_t)0x00000001) /* Filter init mode */

/* Time out for INAK bit */
#define INAK_TIMEOUT        ((uint32_t)0x0000FFFF)
/* Time out for SLAK bit */
#define SLAK_TIMEOUT        ((uint32_t)0x0000FFFF)

/** @defgroup CAN_Mode
  * @{
  */

#define CAN_Mode_Normal             ((uint8_t)0x00)  /*!< normal mode */
#define CAN_Mode_LoopBack           ((uint8_t)0x01)  /*!< loopback mode */
#define CAN_Mode_Silent             ((uint8_t)0x02)  /*!< silent mode */
#define CAN_Mode_Silent_LoopBack    ((uint8_t)0x03)  /*!< loopback combined with silent mode */

#define IS_CAN_MODE(MODE) (((MODE) == CAN_Mode_Normal) || \
                           ((MODE) == CAN_Mode_LoopBack)|| \
                           ((MODE) == CAN_Mode_Silent) || \
                           ((MODE) == CAN_Mode_Silent_LoopBack))


/** @defgroup CAN_synchronisation_jump_width
  * @{
  */

#define CAN_SJW_1tq                 ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_SJW_2tq                 ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_SJW_3tq                 ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_SJW_4tq                 ((uint8_t)0x03)  /*!< 4 time quantum */

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq)|| \
                         ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))

/** @defgroup CAN_time_quantum_in_bit_segment_1
  * @{
  */

#define CAN_BS1_1tq                 ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_BS1_2tq                 ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_BS1_3tq                 ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_BS1_4tq                 ((uint8_t)0x03)  /*!< 4 time quantum */
#define CAN_BS1_5tq                 ((uint8_t)0x04)  /*!< 5 time quantum */
#define CAN_BS1_6tq                 ((uint8_t)0x05)  /*!< 6 time quantum */
#define CAN_BS1_7tq                 ((uint8_t)0x06)  /*!< 7 time quantum */
#define CAN_BS1_8tq                 ((uint8_t)0x07)  /*!< 8 time quantum */
#define CAN_BS1_9tq                 ((uint8_t)0x08)  /*!< 9 time quantum */
#define CAN_BS1_10tq                ((uint8_t)0x09)  /*!< 10 time quantum */
#define CAN_BS1_11tq                ((uint8_t)0x0A)  /*!< 11 time quantum */
#define CAN_BS1_12tq                ((uint8_t)0x0B)  /*!< 12 time quantum */
#define CAN_BS1_13tq                ((uint8_t)0x0C)  /*!< 13 time quantum */
#define CAN_BS1_14tq                ((uint8_t)0x0D)  /*!< 14 time quantum */
#define CAN_BS1_15tq                ((uint8_t)0x0E)  /*!< 15 time quantum */
#define CAN_BS1_16tq                ((uint8_t)0x0F)  /*!< 16 time quantum */

#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16tq)

/** @defgroup CAN_time_quantum_in_bit_segment_2
  * @{
  */

#define CAN_BS2_1tq                 ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_BS2_2tq                 ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_BS2_3tq                 ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_BS2_4tq                 ((uint8_t)0x03)  /*!< 4 time quantum */
#define CAN_BS2_5tq                 ((uint8_t)0x04)  /*!< 5 time quantum */
#define CAN_BS2_6tq                 ((uint8_t)0x05)  /*!< 6 time quantum */
#define CAN_BS2_7tq                 ((uint8_t)0x06)  /*!< 7 time quantum */
#define CAN_BS2_8tq                 ((uint8_t)0x07)  /*!< 8 time quantum */

#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8tq)



/* Receive Interrupts */
#define CAN_IT_FMP0                 ((uint32_t)0x00000002) /*!< FIFO 0 message pending Interrupt*/
#define CAN_IT_FF0                  ((uint32_t)0x00000004) /*!< FIFO 0 full Interrupt*/
#define CAN_IT_FOV0                 ((uint32_t)0x00000008) /*!< FIFO 0 overrun Interrupt*/
#define CAN_IT_FMP1                 ((uint32_t)0x00000010) /*!< FIFO 1 message pending Interrupt*/
#define CAN_IT_FF1                  ((uint32_t)0x00000020) /*!< FIFO 1 full Interrupt*/
#define CAN_IT_FOV1                 ((uint32_t)0x00000040) /*!< FIFO 1 overrun Interrupt*/


/** @defgroup CAN_transmit_constants
  * @{
  */

#define CAN_TxStatus_Failed         ((uint8_t)0x00)/*!< CAN transmission failed */
#define CAN_TxStatus_Ok             ((uint8_t)0x01) /*!< CAN transmission succeeded */
#define CAN_TxStatus_Pending        ((uint8_t)0x02) /*!< CAN transmission pending */
#define CAN_TxStatus_NoMailBox      ((uint8_t)0x04) /*!< CAN cell did not provide an empty mailbox */



/** @defgroup CAN_identifier_type
  * @{
  */

#define CAN_Id_Standard             ((uint32_t)0x00000000)  /*!< Standard Id */
#define CAN_Id_Extended             ((uint32_t)0x00000004)  /*!< Extended Id */
#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_Id_Standard) || \
                               ((IDTYPE) == CAN_Id_Extended))

#define CAN_ID_EXT                  CAN_Id_Extended
#define CAN_RTR_Data                ((uint32_t)0x00000000)  /*!< Data frame */


/** @defgroup CAN_transmit_constants
  * @{
  */

#define CAN_TxStatus_Failed         ((uint8_t)0x00)/*!< CAN transmission failed */
#define CAN_TxStatus_Ok             ((uint8_t)0x01) /*!< CAN transmission succeeded */
#define CAN_TxStatus_Pending        ((uint8_t)0x02) /*!< CAN transmission pending */
#define CAN_TxStatus_NoMailBox      ((uint8_t)0x04) /*!< CAN cell did not provide an empty mailbox */


/* Mailboxes definition */
#define CAN_TXMAILBOX_0                   ((uint8_t)0x00)
#define CAN_TXMAILBOX_1                   ((uint8_t)0x01)
#define CAN_TXMAILBOX_2                   ((uint8_t)0x02)

/** @defgroup CAN_receive_FIFO_number_constants
  * @{
  */

#define CAN_FIFO0                 ((uint8_t)0x00) /*!< CAN FIFO 0 used to receive */
#define CAN_FIFO1                 ((uint8_t)0x01) /*!< CAN FIFO 1 used to receive */

#define IS_CAN_FIFO(FIFO) (((FIFO) == CAN_FIFO0) || ((FIFO) == CAN_FIFO1))

/** @defgroup CAN_sleep_constants
  * @{
  */

#define CAN_InitStatus_Failed              ((uint8_t)0x00) /*!< CAN initialization failed */
#define CAN_InitStatus_Success             ((uint8_t)0x01) /*!< CAN initialization OK */



/** @defgroup CAN_filter_mode
  * @{
  */

#define CAN_FilterMode_IdMask       ((uint8_t)0x00)  /*!< identifier/mask mode */
#define CAN_FilterMode_IdList       ((uint8_t)0x01)  /*!< identifier list mode */

#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FilterMode_IdMask) || \
                                  ((MODE) == CAN_FilterMode_IdList))
/**
  * @}
  */


/** @defgroup CAN_filter_FIFO
  * @{
  */

#define CAN_Filter_FIFO0             ((uint8_t)0x00)  /*!< Filter FIFO 0 assignment for filter x */
#define CAN_Filter_FIFO1             ((uint8_t)0x01)  /*!< Filter FIFO 1 assignment for filter x */
#define IS_CAN_FILTER_FIFO(FIFO) (((FIFO) == CAN_FilterFIFO0) || \
                                  ((FIFO) == CAN_FilterFIFO1))

/** @defgroup CAN_filter_scale
  * @{
  */

#define CAN_FilterScale_16bit       ((uint8_t)0x00) /*!< Two 16-bit filters */
#define CAN_FilterScale_32bit       ((uint8_t)0x01) /*!< One 32-bit filter */

#define IS_CAN_FILTER_SCALE(SCALE) (((SCALE) == CAN_FilterScale_16bit) || \
                                    ((SCALE) == CAN_FilterScale_32bit))

/**
  * @brief  CAN init structure definition
  */

typedef struct
{
  uint16_t CAN_Prescaler;   /*!< Specifies the length of a time quantum.
                                 It ranges from 1 to 1024. */

  uint8_t CAN_Mode;         /*!< Specifies the CAN operating mode.
                                 This parameter can be a value of
                                @ref CAN_operating_mode */

  uint8_t CAN_SJW;          /*!< Specifies the maximum number of time quanta
                                 the CAN hardware is allowed to lengthen or
                                 shorten a bit to perform resynchronization.
                                 This parameter can be a value of
                                 @ref CAN_synchronisation_jump_width */

  uint8_t CAN_BS1;          /*!< Specifies the number of time quanta in Bit
                                 Segment 1. This parameter can be a value of
                                 @ref CAN_time_quantum_in_bit_segment_1 */

  uint8_t CAN_BS2;          /*!< Specifies the number of time quanta in Bit
                                 Segment 2.
                                 This parameter can be a value of
                                 @ref CAN_time_quantum_in_bit_segment_2 */

  FunctionalState CAN_TTCM; /*!< Enable or disable the time triggered
                                 communication mode. This parameter can be set
                                 either to ENABLE or DISABLE. */

  FunctionalState CAN_ABOM;  /*!< Enable or disable the automatic bus-off
                                  management. This parameter can be set either
                                  to ENABLE or DISABLE. */

  FunctionalState CAN_AWUM;  /*!< Enable or disable the automatic wake-up mode.
                                  This parameter can be set either to ENABLE or
                                  DISABLE. */

  FunctionalState CAN_NART;  /*!< Enable or disable the no-automatic
                                  retransmission mode. This parameter can be
                                  set either to ENABLE or DISABLE. */

  FunctionalState CAN_RFLM;  /*!< Enable or disable the Receive FIFO Locked mode.
                                  This parameter can be set either to ENABLE
                                  or DISABLE. */

  FunctionalState CAN_TXFP;  /*!< Enable or disable the transmit FIFO priority.
                                  This parameter can be set either to ENABLE
                                  or DISABLE. */
} CAN_InitTypeDef;

/**
  * @brief  CAN filter init structure definition
  */

typedef struct
{
  uint16_t CAN_FilterIdHigh;         /*!< Specifies the filter identification number (MSBs for a 32-bit
                                              configuration, first one for a 16-bit configuration).
                                              This parameter can be a value between 0x0000 and 0xFFFF */

  uint16_t CAN_FilterIdLow;          /*!< Specifies the filter identification number (LSBs for a 32-bit
                                              configuration, second one for a 16-bit configuration).
                                              This parameter can be a value between 0x0000 and 0xFFFF */

  uint16_t CAN_FilterMaskIdHigh;     /*!< Specifies the filter mask number or identification number,
                                              according to the mode (MSBs for a 32-bit configuration,
                                              first one for a 16-bit configuration).
                                              This parameter can be a value between 0x0000 and 0xFFFF */

  uint16_t CAN_FilterMaskIdLow;      /*!< Specifies the filter mask number or identification number,
                                              according to the mode (LSBs for a 32-bit configuration,
                                              second one for a 16-bit configuration).
                                              This parameter can be a value between 0x0000 and 0xFFFF */

  uint16_t CAN_FilterFIFOAssignment; /*!< Specifies the FIFO (0 or 1) which will be assigned to the filter.
                                              This parameter can be a value of @ref CAN_filter_FIFO */

  uint8_t CAN_FilterNumber;          /*!< Specifies the filter which will be initialized. It ranges from 0 to 13. */

  uint8_t CAN_FilterMode;            /*!< Specifies the filter mode to be initialized.
                                              This parameter can be a value of @ref CAN_filter_mode */

  uint8_t CAN_FilterScale;           /*!< Specifies the filter scale.
                                              This parameter can be a value of @ref CAN_filter_scale */

  FunctionalState CAN_FilterActivation; /*!< Enable or disable the filter.
                                              This parameter can be set either to ENABLE or DISABLE. */
} CAN_FilterInitTypeDef;

typedef struct
{
  uint8_t NVIC_IRQChannel;                    /*!< Specifies the IRQ channel to be enabled or disabled.
                                                   This parameter can be a value of @ref IRQn_Type
                                                   (For the complete STM32 Devices IRQ Channels list, please
                                                    refer to stm32f10x.h file) */

  uint8_t NVIC_IRQChannelPreemptionPriority;  /*!< Specifies the pre-emption priority for the IRQ channel
                                                   specified in NVIC_IRQChannel. This parameter can be a value
                                                   between 0 and 15 as described in the table @ref NVIC_Priority_Table */

  uint8_t NVIC_IRQChannelSubPriority;         /*!< Specifies the subpriority level for the IRQ channel specified
                                                   in NVIC_IRQChannel. This parameter can be a value
                                                   between 0 and 15 as described in the table @ref NVIC_Priority_Table */

  FunctionalState NVIC_IRQChannelCmd;         /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                                   will be enabled or disabled.
                                                   This parameter can be set either to ENABLE or DISABLE */
} NVIC_InitTypeDef;



///////////////////////////////////              ADC MACRO 			/////////////////////////////////////////

//Define the Macro for ADC
#define CFGR_ADCPRE_Reset_Mask   	 ((uint32_t)0xFFFF3FFF)
#define CR1_CLEAR_Mask              ((uint32_t)0xFFF0FEFF)
#define CR2_CLEAR_Mask              ((uint32_t)0xFFF1F7FD)
#define SQR1_CLEAR_Mask             ((uint32_t)0xFF0FFFFF)
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_RSTCAL_Set              ((uint32_t)0x00000008)
#define CR2_CAL_Set                 ((uint32_t)0x00000004)
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)


#define ADC_ExternalTrigConv_None                  ((uint32_t)0x000E0000) /*!< For ADC1, ADC2 and ADC3 */

/** @defgroup ADC_clock_source
  * @{
  */

#define RCC_PCLK2_Div2                   ((uint32_t)0x00000000)
#define RCC_PCLK2_Div4                   ((uint32_t)0x00004000)
#define RCC_PCLK2_Div6                   ((uint32_t)0x00008000)
#define RCC_PCLK2_Div8                   ((uint32_t)0x0000C000)
#define IS_RCC_ADCCLK(ADCCLK) (((ADCCLK) == RCC_PCLK2_Div2) || ((ADCCLK) == RCC_PCLK2_Div4) || \
                               ((ADCCLK) == RCC_PCLK2_Div6) || ((ADCCLK) == RCC_PCLK2_Div8))


//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////CAN FUNCTION LIST/////////////////////////////////////////
#define CAN_FUNCTION_PING 					0x10
#define CAN_FUNCTION_BLINK_HEARTBEAT 		0x20
#define CAN_FUNCTION_JUMP_TO_APPLICATION 	0x30
#define CAN_FUNCTION_CRC_REGION 			0x40
#define CAN_FUNCTION_ERASE_FLASH 			0x50
#define CAN_FUNCTION_WRITE_FLASH 			0x60
#define CAN_FUNCTION_READ_FLASH 			0x70
#define CAN_NORMAL_MESSAGE					0x00

#define CAN_APPLICATION_UPLOADDOWNLOAD_ERROR					0x08
#define CAN_APPLICATION_UPLOADDOWNLOAD_SUCCESS					0x09

#define CAN_UPLOAD_START_APPLICATION			0x1A
#define CAN_UPLOADING_APPLICATION				0x1B
#define CAN_UPLOAD_END_APPLICATION				0x1C


#define CAN_DOWNLOAD_START_APPLICATION			0x1D
#define CAN_DOWNLOADING_APPLICATION			0x1E
#define CAN_DOWNLOAD_END_APPLICATION			0x1F

#define CAN_REQUEST_PING					0x10
#define CAN_RESPONSE_PING					0x80


///////////////////////FLASH MACRO////////////////////////////////////////////////
/**
  * @brief  FLASH Status
  */

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;
/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE)
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

/* Delay definition */
#define EraseTimeout          ((uint32_t)0x000B0000)
#define ProgramTimeout        ((uint32_t)0x00002000)


#define FLASH_FLAG_BSY                 ((uint32_t)0x00000001)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000020)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PGERR               ((uint32_t)0x00000004)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR              ((uint32_t)0x00000001)  /*!< FLASH Option Byte error flag */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 Program error flag */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 Write protected error flag */


#define FLASH_PAGE_SIZE					0x400	//a page size is 0x400 in stm32f10x series.

#ifdef OPTIMIZE
#define FLASH_APPLICATION_OFFSET_ADDRESS		0x2000
#else
#define FLASH_APPLICATION_OFFSET_ADDRESS 		0x4000
#endif
#define FLASH_APPLICATION_START_ADDRESS			FLASH_BASE + FLASH_APPLICATION_OFFSET_ADDRESS
#define FLASH_APPLICATION_SIZE_ADDRESS			FLASH_APPLICATION_START_ADDRESS - FLASH_PAGE_SIZE//it would need to upload and download .

///////////////////////////////////// Application Jump ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#define __HAL_RCC_AFIO_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_AFIOEN))
#define __HAL_RCC_GPIOA_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN))
#define __HAL_RCC_GPIOB_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPBEN))
#define __HAL_RCC_GPIOC_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPCEN))
#define __HAL_RCC_GPIOD_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPDEN))
#define __HAL_RCC_ADC1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC1EN))

#define __HAL_RCC_TIM1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_TIM1EN))
#define __HAL_RCC_SPI1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_SPI1EN))
#define __HAL_RCC_USART1_CLK_DISABLE()    (RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN))


#define __HAL_RCC_APB1_FORCE_RESET()       (RCC->APB2RSTR = 0xFFFFFFFFU)
#define __HAL_RCC_APB1_RELEASE_RESET()      (RCC->APB1RSTR = 0x00)
#define __HAL_RCC_APB2_FORCE_RESET()       (RCC->APB2RSTR = 0xFFFFFFFFU)
#define __HAL_RCC_APB2_RELEASE_RESET()      (RCC->APB2RSTR = 0x00)


typedef void (application_t)(void);

typedef struct
{
    uint32_t		stack_addr;     // Stack Pointer
    application_t*	func_p;        // Program Counter
} JumpStruct;





