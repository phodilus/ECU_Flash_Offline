/**
	******************************************************************************
	* @file		CAN/Networking/stm32f2xx_it.c 
	* @author	MCD Application Team
	* @version 	V1.1.0
	* @date		13-April-2012
	* @brief	 Main Interrupt Service Routines.
	*					This file provides template for all exceptions handler and 
	*					peripherals interrupt service routine.
	******************************************************************************
	* @attention
	*
	* <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
	*
	* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
	* You may not use this file except in compliance with the License.
	* You may obtain a copy of the License at:
	*
	*				http://www.st.com/software_license_agreement_liberty_v2
	*
	* Unless required by applicable law or agreed to in writing, software 
	* distributed under the License is distributed on an "AS IS" BASIS, 
	* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	* See the License for the specific language governing permissions and
	* limitations under the License.
	*
	******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include <string.h>

/**	* @brief	This function handles NMI exception. */
void NMI_Handler(void)
{
}
/**	* @brief	This function handles Hard Fault exception. */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) ;
}
/**	* @brief	This function handles Memory Manage exception. */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
		;
}
/**	* @brief	This function handles Bus Fault exception. */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
		;
}
/**	* @brief	This function handles Usage Fault exception. */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
		;
}
/**	* @brief	This function handles SVCall exception. */
void SVC_Handler(void)
{
}
/**	* @brief	This function handles Debug Monitor exception. */
void DebugMon_Handler(void)
{
}
/**	* @brief	This function handles PendSVC exception. */
void PendSV_Handler(void)
{
}
/**	* @brief	This function handles SysTick Handler. */
void SysTick_Handler(void)
{
}

/////////////////////////////////////////////////////////
void SDIO_IRQHandler(void)
{
	//Process All SDIO Interrupt Sources.
	SD_ProcessIRQSrc();
}
void SDIO_DMA_IRQHANDLER(void)
{
	//Process All SDIO Interrupt Sources.
	SD_ProcessDMAIRQ();
}
/*void TIM4_IRQHandler(void)
{
	//Clear TIM2 update interrupt. This should be down first.
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	//Toggle LED2.
	LED_Switch(LED_2, -1);
}
void CAN1_RX0_IRQHandler(void)
{
	CAN_RxMsgTypeDef msg, msg1;
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != SET)
		return;
	CAN_Receive(CAN1, CAN_FIFO0, &msg);
	CAN_FIFORelease(CAN1, CAN_FIFO0);
	if (msg.Data[0] == 0x40)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &msg1);
		if (msg1.Data[0] != msg.Data[0])
			msg1.StdId = msg.StdId;
	}
	m_uCanRecvID = msg.StdId;
	//CAN_ClearFlag();
	//CAN_ClearITPendingBit();
}*/
void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(BTNs[BTN_GPIOA3].iline) == SET)
	{
		LED_Switch(LED_3, 0);
		LED_Switch(LED_4, 0);
		if (Process() == TRUE)
		{
			LED_Switch(LED_3, 1);//OK
			LED_Switch(LED_4, 0);//NG
		}
		else
		{
			LED_Switch(LED_3, 0);//OK
			LED_Switch(LED_4, 1);//NG
		}
		//avoid user operations during process.
		EXTI_ClearITPendingBit(BTNs[BTN_GPIOA3].iline);
	}
}
