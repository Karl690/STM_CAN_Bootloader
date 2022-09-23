/*
 * can.c
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */

#include "configure.h"
#include "taskmanager.h"
#include "can.h"
#include "ADC/adc.h"
#include "RCC/rcc.h"
#include "FLASH/flash.h"

CanRxMsg 				CanRxMessage;
CanTxMsg 				CanTxMessage;
uint8_t 				CanTransmitMailbox;
uint32_t 				CanTrasmitMsgWaitCounter = 0;
uint8_t					CanRxTargetId;
uint8_t					CanRxSourceId;
uint8_t					CanRxPage;
uint8_t					CanRxMsgType;
uint8_t					CanRxImmediateFlag = 0;
uint8_t 				CanMessageBuffer[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}; //this is a buffer for CAN message transmitting.

uint16_t				CanRxInIndex = 0;
uint16_t				CanRxOutIndex = 0;
CANMsg 					CanRxMsgBuffer[CAN_MSG_BUFFER_SIZE];
uint16_t				CanTxInIndex = 0;
uint16_t				CanTxOutIndex = 0;
CANMsg 					CanTxMsgBuffer[CAN_MSG_BUFFER_SIZE];
void InitCAN()
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
	CAN1->MCR |= CAN_MCR_INRQ ;

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
	NVIC->IP[CAN1_RX1_IRQn] = 2;
	/* Enable the Selected IRQ Channels --------------------------------------*/
	NVIC->ISER[CAN1_RX1_IRQn >> 0x05] = (uint32_t)0x01 << (CAN1_RX1_IRQn & (uint8_t)0x1F);
}
/**
  * @brief  Receives a message.
  * @param  CANx:       where x can be 1 or 2 to to select the CAN peripheral.
  * @param  FIFONumber: Receive FIFO number, CAN_FIFO0 or CAN_FIFO1.
  * @param  RxMessage:  pointer to a structure receive message which contains
  *                     CAN Id, CAN DLC, CAN datas and FMI number.
  * @retval None.
  */
void CAN_Receive(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg* RxMessage)
{
  /* Check the parameters */
  /* Get the Id */
  RxMessage->IDE = (uint8_t)0x04 & CANx->sFIFOMailBox[FIFONumber].RIR;
  if (RxMessage->IDE == CAN_Id_Standard)
  {
    RxMessage->StdId = (uint32_t)0x000007FF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 21);
  }
  else
  {
    RxMessage->ExtId = (uint32_t)0x1FFFFFFF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 3);
  }

  RxMessage->RTR = (uint8_t)0x02 & CANx->sFIFOMailBox[FIFONumber].RIR;
  /* Get the DLC */
  RxMessage->DLC = (uint8_t)0x0F & CANx->sFIFOMailBox[FIFONumber].RDTR;
  /* Get the FMI */
  RxMessage->FMI = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDTR >> 8);
  /* Get the data field */
  RxMessage->Data[0] = (uint8_t)0xFF & CANx->sFIFOMailBox[FIFONumber].RDLR;
  RxMessage->Data[1] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 8);
  RxMessage->Data[2] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 16);
  RxMessage->Data[3] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDLR >> 24);
  RxMessage->Data[4] = (uint8_t)0xFF & CANx->sFIFOMailBox[FIFONumber].RDHR;
  RxMessage->Data[5] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 8);
  RxMessage->Data[6] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 16);
  RxMessage->Data[7] = (uint8_t)0xFF & (CANx->sFIFOMailBox[FIFONumber].RDHR >> 24);
  /* Release the FIFO */
  /* Release FIFO0 */
  if (FIFONumber == CAN_FIFO0)
  {
    CANx->RF0R |= CAN_RF0R_RFOM0;
  }
  /* Release FIFO1 */
  else /* FIFONumber == CAN_FIFO1 */
  {
    CANx->RF1R |= CAN_RF1R_RFOM1;
  }
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

	//CanTxMessage.RTR= CAN_RTR_DATA;
	//CanTxMessage.IDE= CAN_ID_EXT;
	//CanTxMessage.DLC= size;  //the data size to send, which would be smaller than 8bytes.
	//if(size > 0) memcpy(CanTxMessage.Data, data, size); // copy the data to TxMessage's buffer

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
	return SUCCESS;
}

/**
  * @brief  Checks the transmission of a message.
  * @param  CANx:            where x can be 1 or 2 to to select the
  *                          CAN peripheral.
  * @param  TransmitMailbox: the number of the mailbox that is used for
  *                          transmission.
  * @retval CAN_TxStatus_Ok if the CAN driver transmits the message, CAN_TxStatus_Failed
  *         in an other case.
  */
uint8_t CAN_TransmitStatus(CAN_TypeDef* CANx, uint8_t TransmitMailbox)
{
  uint32_t state = 0;

  switch (TransmitMailbox)
  {
    case (CAN_TXMAILBOX_0):
      state =   CANx->TSR &  (CAN_TSR_RQCP0 | CAN_TSR_TXOK0 | CAN_TSR_TME0);
      break;
    case (CAN_TXMAILBOX_1):
      state =   CANx->TSR &  (CAN_TSR_RQCP1 | CAN_TSR_TXOK1 | CAN_TSR_TME1);
      break;
    case (CAN_TXMAILBOX_2):
      state =   CANx->TSR &  (CAN_TSR_RQCP2 | CAN_TSR_TXOK2 | CAN_TSR_TME2);
      break;
    default:
      state = CAN_TxStatus_Failed;
      break;
  }
  switch (state)
  {
      /* transmit pending  */
    case (0x0): state = CAN_TxStatus_Pending;
      break;
      /* transmit failed  */
     case (CAN_TSR_RQCP0 | CAN_TSR_TME0): state = CAN_TxStatus_Failed;
      break;
     case (CAN_TSR_RQCP1 | CAN_TSR_TME1): state = CAN_TxStatus_Failed;
      break;
     case (CAN_TSR_RQCP2 | CAN_TSR_TME2): state = CAN_TxStatus_Failed;
      break;
      /* transmit succeeded  */
    case (CAN_TSR_RQCP0 | CAN_TSR_TXOK0 | CAN_TSR_TME0):state = CAN_TxStatus_Ok;
      break;
    case (CAN_TSR_RQCP1 | CAN_TSR_TXOK1 | CAN_TSR_TME1):state = CAN_TxStatus_Ok;
      break;
    case (CAN_TSR_RQCP2 | CAN_TSR_TXOK2 | CAN_TSR_TME2):state = CAN_TxStatus_Ok;
      break;
    default: state = CAN_TxStatus_Failed;
      break;
  }
  return (uint8_t) state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//THis function is a callback for Receiving CAN Message.
void CAN1_RX1_IRQHandler(void)
{
	CAN_Receive(CAN1, CAN_FIFO1, &CanRxMessage);
	CanRxTargetId = (CanRxMessage.ExtId >> 20) & 0xFF;
	if(CanRxTargetId == CurrentHeadCanAddress) //if Head Address is same as Can message Head identifier.
	{
		CANMsg* pInBuffer = &CanRxMsgBuffer[CanRxInIndex];

		pInBuffer->ID = CanRxMessage.ExtId;
		memcpy(pInBuffer->Data, CanRxMessage.Data, 8);
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
	//CAN1->RF1R |= CAN_RF1R_RFOM1;   // Release FIFO1
}

uint32_t GenerateFrameID(uint16_t target, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate)
{
	return (uint32_t)((immediate << 28) + (target << 20) + ((uint32_t)CurrentHeadCanAddress << 12) + (msgType << 11) + (msgId << 4) + page);
}

void CanAddTxBuffer(uint16_t target, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate,  uint8_t* data, uint8_t size)
{
	CANMsg* pOutBuffer = &CanTxMsgBuffer[CanTxOutIndex];
	pOutBuffer->ID = GenerateFrameID(target, msgType, msgId, page, immediate);

	if(data && size > 0) memcpy(pOutBuffer->Data, data, size);
	pOutBuffer->DataSize = size;

	pOutBuffer->Immediate = immediate;
	pOutBuffer->MsgType = msgType;
	pOutBuffer->MsgId = msgId;
	pOutBuffer->Page = page;
	pOutBuffer->SourceAddress = CurrentHeadCanAddress;
	pOutBuffer->TargetAddress = target;

	CanTxInIndex ++;
	CanTxInIndex &= CAN_MSG_BUFFER_SIZE;
}

void ProcessCanRxMessage(void)
{
	if(CanRxInIndex == CanRxOutIndex) return;
	CANMsg* pOutBuffer = &CanRxMsgBuffer[CanRxOutIndex];
	uint16_t address = 0;
	switch(pOutBuffer->MsgType)
	{
	case CAN_WRITE:
		switch(pOutBuffer->MsgId)
		{
		case CAN_MSG_SOAP_STRING:
			address = (uint16_t)(pOutBuffer->Data[0] + (pOutBuffer->Data[1] << 8));
			for(uint8_t i = 2; i < pOutBuffer->DataSize; i ++)
			{
				SoapString[address + i -2] = pOutBuffer->Data[i];
				if(SoapString[address + i -2] == 0) {
					SmallTaskCount = 0;
					SmallTaskType = TASK_FLASH_WRITE_SOAPSTRING; //Start the task to write SoapString to Flash 0x8007C00 address
					break;
				}
			}
			break;
		}
		break;
	case CAN_READ:
		switch(pOutBuffer->MsgId)
		{
		case CAN_MSG_SOAP_STRING:
			//                   Address                            Length                           Out buffer
			ReadFlashData(FLASH_SOAP_START_ADDRESS + BYTES2UINT32(&pOutBuffer->Data[0]), BYTES2UINT32(&pOutBuffer->Data[4]), SoapString); //Read the soap string from Flash
			SmallTaskCount = 0;
			SmallTaskType = TASK_CAN_SEND_SOAPSTRING;
			break;
		}
		break;
	}
	CanRxOutIndex ++;
	if(CanRxOutIndex >= CAN_MSG_BUFFER_SIZE) CanRxOutIndex =0;
}
void ProcessCanTxMessage(void)
{
	if(CanTxInIndex == CanTxOutIndex) return;
	CANMsg* pOutBuffer = &CanTxMsgBuffer[CanTxOutIndex];
	if(SendCanMessage(pOutBuffer->ID, pOutBuffer->Data, pOutBuffer->DataSize) == SUCCESS) {
		CanTxOutIndex ++;
		if(CanTxOutIndex >= CAN_MSG_BUFFER_SIZE) CanTxOutIndex = 0;
	}
}
