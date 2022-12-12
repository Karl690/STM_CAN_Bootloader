#include "configure.h"
#ifdef HH103
#include "stm32f10x_can.h"
#include "global.h"
#include "CAN/can.h"

#include "taskmanager.h"
CanRxMsg 				CanRxMessage;
CanTxMsg 				CanTxMessage;

uint16_t 				CanRxLedCountDown = 0;
uint16_t 				CanTxLedCountDown = 0;

void Init_CAN()
{
	/* CAN register init*/
		//First, Deinitializes the CAN peripheral registers to their default reset values.
	RCC->APB1RSTR |= RCC_APB1Periph_CAN1;  /* Enable CAN1 reset state */
	RCC->APB1RSTR &= ~RCC_APB1Periph_CAN1; /* Release CAN1 from reset state */

	uint32_t temp = 0;
	//Initializes the CAN peripheral according to the specified         parameters in the CAN_InitStruct.(CAN_Init(CAN1, &CAN_InitStructure))
	/* Exit from sleep mode */
	CAN1->MCR &= (~(uint32_t)CAN_MCR_SLEEP);

	/* Request initialisation */
	CAN1->MCR |= CAN_MCR_INRQ;

	/* Wait the acknowledge */
	while (((CAN1->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) && (temp != INAK_TIMEOUT))
	{
		temp++;
	}
	/* Set the time triggered communication mode */
	CAN1->MCR &= ~(uint32_t)CAN_MCR_TTCM;  	//CAN_InitStructure.CAN_TTCM = DISABLE;
	/* Set the automatic bus-off management */
	CAN1->MCR &= ~(uint32_t)CAN_MCR_ABOM;	//CAN_InitStructure.CAN_ABOM = DISABLE;
	/* Set the automatic wake-up mode */
	CAN1->MCR &= ~(uint32_t)CAN_MCR_AWUM;
	/* Set the no automatic retransmission */
	CAN1->MCR &= ~(uint32_t)CAN_MCR_NART;	//CAN_InitStructure.CAN_NART = DISABLE;
	/* Set the receive FIFO locked mode */
	CAN1->MCR &= ~(uint32_t)CAN_MCR_RFLM;	//CAN_InitStructure.CAN_RFLM = DISABLE;
	/* Set the transmit FIFO priority */
	CAN1->MCR &= ~(uint32_t)CAN_MCR_TXFP;	//CAN_InitStructure.CAN_TXFP = DISABLE;

	/* Set the bit timing register */
	CAN1->BTR = (uint32_t)((uint32_t)CAN_Mode_Normal << 30) | \
				((uint32_t)CAN_SJW_1tq << 24) | \
				((uint32_t)CAN_BS1_3tq << 16) | \
				((uint32_t)CAN_BS2_5tq << 20) | \
			   ((uint32_t)8 - 1);
	/* Request leave initialisation */
	CAN1->MCR &= ~(uint32_t)CAN_MCR_INRQ;
	/* Wait the acknowledge */
	temp = 0;

	while (((CAN1->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) && (temp != INAK_TIMEOUT))
	{
		temp++;
	}
	/*End of Initialized Can Init Structure.*/

	/*Initializes the CAN peripheral according to the specified parameters in the CAN_FilterInitStruct.*/
	CAN1->FMR |= FMR_FINIT;
	CAN1->FA1R &= ~(uint32_t)1;
	CAN1->FS1R |= 1;

	 /* 32-bit identifier or First 32-bit identifier */
	CAN1->sFilterRegister[0].FR1 = ((0x0000FFFF & (uint32_t)0) << 16) | (0x0000FFFF & (uint32_t)0);
	/* 32-bit mask or Second 32-bit identifier */
	CAN1->sFilterRegister[0].FR2 = ((0x0000FFFF & (uint32_t)0) << 16) | (0x0000FFFF & (uint32_t)0);

	CAN1->FM1R &= ~(uint32_t)1;
	CAN1->FFA1R |= (uint32_t)1;
	CAN1->FA1R |= 1;
	/* Leave the initialisation mode for the filter */
	CAN1->FMR &= ~FMR_FINIT;
	/*End of Initialized Can Filter Init Structure.*/

	/*Enables the specified CAN1 interrupts.*/
	CAN1->IER |= CAN_IT_FMP1;


	/*Initializes the NVIC peripheral according to the specified parameters in the NVIC_InitStruct.*/
	NVIC->IP[CAN1_RX1_IRQn] = 0;
	/* Enable the Selected IRQ Channels --------------------------------------*/
	NVIC->ISER[CAN1_RX1_IRQn >> 0x05] = (uint32_t)0x01 << (CAN1_RX1_IRQn & (uint8_t)0x1F);
}

//////////////////////////////////////////////////
//this function is for sending message thru CAN.
//
// param:
//		target is a head number to send message.
//		funcId means the function identifier of message.
//		data is buffer to send. it must be less than 8bytes.
//		size is the length of buffer.


uint8_t SendCanMessage(uint32_t id, uint8_t* data, uint8_t size)
{
	uint8_t CanTransmitMailbox = 0;

	/* Select one empty transmit mailbox */
	if ((CAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
	{
		CanTransmitMailbox = 0;
	}
	else if ((CAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
	{
		CanTransmitMailbox = 1;
	}
	else if ((CAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
	{
		CanTransmitMailbox = 2;
	}
	else
	{
		CanTransmitMailbox = CAN_TxStatus_NoMailBox;
		return ERROR;
	}


	CAN1->sTxMailBox[CanTransmitMailbox].TIR &= TMIDxR_TXRQ;

	CAN1->sTxMailBox[CanTransmitMailbox].TIR |= ((id << 3) | \
												CAN_Id_Extended | \
													CAN_RTR_Data);

	/* Set up the DLC */

	CAN1->sTxMailBox[CanTransmitMailbox].TDTR &= (uint32_t)0xFFFFFFF0;
	CAN1->sTxMailBox[CanTransmitMailbox].TDTR |= size; //Data Size

	/* Set up the data field */
	CAN1->sTxMailBox[CanTransmitMailbox].TDLR = (((uint32_t)data[3] << 24) |
											 ((uint32_t)data[2] << 16) |
											 ((uint32_t)data[1] << 8) |
											 ((uint32_t)data[0]));
	CAN1->sTxMailBox[CanTransmitMailbox].TDHR = (((uint32_t)data[7] << 24) |
											 ((uint32_t)data[6] << 16) |
											 ((uint32_t)data[5] << 8) |
											 ((uint32_t)data[4]));
	/* Request transmission */
	CAN1->sTxMailBox[CanTransmitMailbox].TIR |= TMIDxR_TXRQ;
	CanTxLedCountDown = LED_ON_MAXCOUNT;
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//THis function is a callback for Receiving CAN Message.
void CAN1_RX1_IRQHandler(void)
{
	CAN_Receive(CAN1, CAN_FIFO1, &CanRxMessage);
	uint8_t CanRxTargetId = (CanRxMessage.ExtId >> 20) & 0xFF;
	if(CanRxTargetId == HeadPosition || CanRxTargetId == CAN_BROADCAST_ADDRESS) //if Head Address is same as Can message Head identifier.
	{
		CanRxLedCountDown = LED_ON_MAXCOUNT;
		CANMsg* pInBuffer = &CanRxMsgBuffer[CanRxInIndex];

		pInBuffer->ID = CanRxMessage.ExtId;
		memset(pInBuffer->Data, 0, 8);
		if(CanRxMessage.DLC > 0) memcpy(pInBuffer->Data, CanRxMessage.Data, CanRxMessage.DLC);
		pInBuffer->DataSize = CanRxMessage.DLC;

		pInBuffer->Immediate = CanRxMessage.ExtId & 0x10000000?1:0;
		pInBuffer->MsgType = (CanRxMessage.ExtId & 0x800)?CAN_WRITE:CAN_READ;
		pInBuffer->MsgId = ((CanRxMessage.ExtId &  0xFF0) >> 4) & 0x7F;
		pInBuffer->Page = CanRxMessage.ExtId & 0xF;
		pInBuffer->SourceAddress = (CanRxMessage.ExtId >> 12) & 0xFF;
		pInBuffer->TargetAddress = (CanRxMessage.ExtId >> 20) & 0xFF;
		CanRxInIndex ++;
		if(CanRxInIndex >= CAN_MSG_BUFFER_SIZE) CanRxInIndex = 0;
	}
	CAN1->RF1R |= CAN_RF1R_RFOM1;   // Release FIFO1
}

#endif
