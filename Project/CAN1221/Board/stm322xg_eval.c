/**	****************************************************************************
	* @file		stm322xg_eval.c
	* @author	MCD Application Team
	* @version V5.0.3
	* @date		09-March-2012
	* @brief	 This file provides
	*			- set of firmware functions to manage Leds, push-button and COM
	*			- low level initialization functions for SD card (on SDIO) and
	*			  ports serial EEPROM (sEE)
	*			available on STM322xG-EVAL evaluation board(MB786) RevA and RevB 
	*			from STMicroelectronics.
	****************************************************************************
	* @attention
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
	****************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include "stm322xg_eval.h"
#include "stm32f2xx_sdio.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_i2c.h"
		
/** * @defgroup STM322xG_EVAL_LOW_LEVEL 
	* @brief This file provides firmware functions to manage Leds, push-buttons, 
	*		 COM ports, SD card on SDIO and serial EEPROM (sEE) available on 
	*		 STM322xG-EVAL evaluation board from STMicroelectronics.
	*/
LED_PARAM LEDs[] = {
	{GPIOA, RCC_AHB1Periph_GPIOA, GPIO_Pin_4},
	{GPIOA, RCC_AHB1Periph_GPIOA, GPIO_Pin_5},
	{GPIOA, RCC_AHB1Periph_GPIOA, GPIO_Pin_6},
	{GPIOA, RCC_AHB1Periph_GPIOA, GPIO_Pin_7},
	{NULL}
};
BTN_PARAM BTNs[] = {
	{GPIOA, RCC_AHB1Periph_GPIOA, GPIO_Pin_0,  EXTI_Line0,  EXTI_PortSourceGPIOA, EXTI_PinSource0,  1, EXTI0_IRQn},	//Unknown push-button
	{GPIOF, RCC_AHB1Periph_GPIOF, GPIO_Pin_13, EXTI_Line13, EXTI_PortSourceGPIOF, EXTI_PinSource13, 1, 0x7F},		//Wakeup push-button
	{GPIOF, RCC_AHB1Periph_GPIOF, GPIO_Pin_14, EXTI_Line14, EXTI_PortSourceGPIOF, EXTI_PinSource14, 1, 0x7F},		//Tamper push-button
	{GPIOF, RCC_AHB1Periph_GPIOF, GPIO_Pin_15, EXTI_Line15, EXTI_PortSourceGPIOF, EXTI_PinSource15, 1, 0x7F},		//Key push-button
	{NULL}
};
BTN_PARAM JOYs[] = {
	{GPIOE, RCC_AHB1Periph_GPIOE, GPIO_Pin_11, EXTI_Line11, EXTI_PortSourceGPIOE, EXTI_PinSource11, 0, EXTI15_10_IRQn},
	{GPIOE, RCC_AHB1Periph_GPIOE, GPIO_Pin_12, EXTI_Line12, EXTI_PortSourceGPIOE, EXTI_PinSource12, 0, EXTI15_10_IRQn},
	{GPIOE, RCC_AHB1Periph_GPIOE, GPIO_Pin_13, EXTI_Line13, EXTI_PortSourceGPIOE, EXTI_PinSource13, 0, EXTI15_10_IRQn},
	{GPIOE, RCC_AHB1Periph_GPIOE, GPIO_Pin_14, EXTI_Line14, EXTI_PortSourceGPIOE, EXTI_PinSource14, 0, EXTI15_10_IRQn},
	{GPIOE, RCC_AHB1Periph_GPIOE, GPIO_Pin_15, EXTI_Line15, EXTI_PortSourceGPIOE, EXTI_PinSource15, 0, EXTI15_10_IRQn},
	{NULL}
};
COM_PARAM COMs[] = {
	{USART1, RCC_APB2Periph_USART1, GPIO_AF_USART1, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9,  GPIO_PinSource9,
													RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_PinSource10, USART1_IRQn},
	{USART2, RCC_APB1Periph_USART2, GPIO_AF_USART1, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9,  GPIO_PinSource9,
													RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_PinSource10, USART2_IRQn},
	{USART3, RCC_APB1Periph_USART3, GPIO_AF_USART1, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9,  GPIO_PinSource9,
													RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_PinSource10, USART3_IRQn},
	{UART4,  RCC_APB1Periph_UART4,  GPIO_AF_USART1, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9,  GPIO_PinSource9,
													RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_PinSource10, UART4_IRQn},
	{UART5,  RCC_APB1Periph_UART5,  GPIO_AF_USART1, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9,  GPIO_PinSource9,
													RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_PinSource10, UART5_IRQn},
	{USART6, RCC_APB2Periph_USART6, GPIO_AF_USART1, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9,  GPIO_PinSource9,
													RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_PinSource10, USART6_IRQn},
	{NULL}
};
CAN_PARAM CANs[] = {
	//CAN1: PA11+PA12,PB8+PB9,PD0+PD1,PI9+PH13. CAN2: PB5+PB6,PB12+PB13.(DFU only use PB5+PB13.) The sample pos =(1 + BS1) / (1 + BS1 + BS2),
	//it should be 75% when Baudrate > 500kbps, 90% when Baudrate < 300kbps. APB1=30MHz, Baudrate = 30M/((1+16+3)*5) = 300k.
	{CAN1, RCC_APB1Periph_CAN1, GPIO_AF_CAN1, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_0, GPIO_PinSource0, GPIO_Pin_1, GPIO_PinSource1, 
		CAN1_RX0_IRQn, CAN1_RX1_IRQn, CAN_BS1_16tq, CAN_BS1_3tq, 5},
	{CAN2, RCC_APB1Periph_CAN2, GPIO_AF_CAN2, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_5, GPIO_PinSource5, GPIO_Pin_6, GPIO_PinSource6, 
		CAN2_RX0_IRQn, CAN2_RX1_IRQn, CAN_BS1_16tq, CAN_BS1_3tq, 5},
	{NULL}
};
TIM_PARAM TIMs[] = {
	{TIM1, RCC_APB2Periph_TIM1, 1200, TIM_CounterMode_Up, 0x95F, TIM_OCMode_Timing, TIM_FLAG_Update, TIM_IT_Update, TIM1_CC_IRQn},
	{TIM2, RCC_APB1Periph_TIM2, 2400, TIM_CounterMode_Up, 0x95F, TIM_OCMode_Timing, TIM_FLAG_Update, TIM_IT_Update, TIM2_IRQn},
	{NULL}
};
/*/
GPIO_TypeDef*	LED_PORT[]			= {LED1_GPIO_PORT, 	LED2_GPIO_PORT,	LED3_GPIO_PORT,	LED4_GPIO_PORT};
const uint16_t	LED_PIN[]			= {LED1_PIN, 		LED2_PIN, 		LED3_PIN, 		LED4_PIN};
const uint32_t	LED_CLK[]			= {LED1_GPIO_CLK, 	LED2_GPIO_CLK, 	LED3_GPIO_CLK, 	LED4_GPIO_CLK};
//
GPIO_TypeDef*	BTN_PORT[]			= {BTN_WAKEUP_GPIO_PORT, 	BTN_TEMPER_GPIO_PORT, 	BTN_USER_GPIO_PORT}; 
const uint16_t	BTN_PIN[]			= {BTN_WAKEUP_PIN, 			BTN_TEMPER_PIN, 		BTN_USER_PIN}; 
const uint32_t	BTN_CLK[]			= {BTN_WAKEUP_GPIO_CLK, 	BTN_TEMPER_GPIO_CLK, 	BTN_USER_GPIO_CLK};
const uint16_t	BTN_EXTI_LINE[]		= {BTN_WAKEUP_EXTI_LINE, 		BTN_TEMPER_EXTI_LINE, 			BTN_USER_EXTI_LINE};
const uint8_t	BTN_PORT_SOURCE[]	= {BTN_WAKEUP_EXTI_PORT_SOURCE,	BTN_TEMPER_EXTI_PORT_SOURCE,	BTN_USER_EXTI_PORT_SOURCE};
const uint8_t	BTN_PIN_SOURCE[]	= {BTN_WAKEUP_EXTI_PIN_SOURCE,	BTN_TEMPER_EXTI_PIN_SOURCE,		BTN_USER_EXTI_PIN_SOURCE}; 
const uint8_t	BTN_IRQn[]			= {BTN_WAKEUP_EXTI_IRQn, 		BTN_TEMPER_EXTI_IRQn, 			BTN_USER_EXTI_IRQn};
//
GPIO_TypeDef*	JOY_PORT[]			= {JOY_SELECT_GPIO_PORT, JOY_UP_GPIO_PORT,	JOY_DOWN_GPIO_PORT, JOY_LEFT_GPIO_PORT,	JOY_RIGHT_GPIO_PORT};
const uint16_t	JOY_PIN[]			= {JOY_SELECT_PIN, 		 JOY_UP_PIN, 		JOY_DOWN_PIN, 		JOY_LEFT_PIN, 		JOY_RIGHT_PIN}; 
const uint32_t	JOY_CLK[]			= {JOY_SELECT_GPIO_CLK,  JOY_UP_GPIO_CLK, 	JOY_DOWN_GPIO_CLK, 	JOY_LEFT_GPIO_CLK, 	JOY_RIGHT_GPIO_CLK};
const uint16_t	JOY_EXTI_LINE[]		= {JOY_SELECT_EXTI_LINE, 		JOY_UP_EXTI_LINE, 		 JOY_DOWN_EXTI_LINE, 		JOY_LEFT_EXTI_LINE, 		JOY_RIGHT_EXTI_LINE};
const uint8_t	JOY_PORT_SOURCE[]	= {JOY_SELECT_EXTI_PORT_SOURCE,	JOY_UP_EXTI_PORT_SOURCE, JOY_DOWN_EXTI_PORT_SOURCE,	JOY_LEFT_EXTI_PORT_SOURCE,	JOY_RIGHT_EXTI_PORT_SOURCE};
const uint8_t	JOY_PIN_SOURCE[]	= {JOY_SELECT_EXTI_PIN_SOURCE,	JOY_UP_EXTI_PIN_SOURCE,	 JOY_DOWN_EXTI_PIN_SOURCE,	JOY_LEFT_EXTI_PIN_SOURCE,	JOY_RIGHT_EXTI_PIN_SOURCE}; 
const uint8_t	JOY_IRQn[]			= {JOY_SELECT_EXTI_IRQn, 		JOY_UP_EXTI_IRQn, 		 JOY_DOWN_EXTI_IRQn, 		JOY_LEFT_EXTI_IRQn, 		JOY_RIGHT_EXTI_IRQn};
//
CAN_TypeDef* 	CAN_PORT[]			= {CAN1, 			CAN2}; 
GPIO_TypeDef*	CAN_GPIO_PORT[]		= {CAN1_GPIO_PORT,	CAN2_GPIO_PORT};
const uint32_t	CAN_CLK[] 			= {CAN1_CLK,		CAN2_CLK};
const uint16_t	CAN_RX_PIN[] 		= {CAN1_RX_PIN,		CAN2_RX_PIN};
const uint16_t	CAN_TX_PIN[] 		= {CAN1_TX_PIN,		CAN2_TX_PIN};
const uint32_t	CAN_GPIO_CLK[]		= {CAN1_GPIO_CLK,	CAN2_GPIO_CLK};
const uint32_t	CAN_AF_PORT[]		= {CAN1_AF_PORT,	CAN2_AF_PORT};
const uint32_t	CAN_RX_SOURCE[]		= {CAN1_RX_SOURCE,	CAN2_RX_SOURCE};
const uint32_t	CAN_TX_SOURCE[]		= {CAN1_TX_SOURCE,	CAN2_TX_SOURCE};
const uint32_t	CAN_RX0_IRQ[]		= {CAN1_RX0_IRQn,	CAN2_RX0_IRQn};
const uint32_t	CAN_RX1_IRQ[]		= {CAN1_RX1_IRQn,	CAN2_RX1_IRQn};
//
USART_TypeDef* 	COMx[]				= {COM1, COM2, COM3, 	COM4, COM5, COM6}; 
GPIO_TypeDef* 	COM_TX_PORT[]		= {COM1_TX_GPIO_PORT, 	COM2_TX_GPIO_PORT, COM3_TX_GPIO_PORT, COM4_TX_GPIO_PORT, COM5_TX_GPIO_PORT, COM6_TX_GPIO_PORT}; 
GPIO_TypeDef* 	COM_RX_PORT[]		= {COM1_RX_GPIO_PORT,	COM2_RX_GPIO_PORT, COM3_RX_GPIO_PORT, COM4_RX_GPIO_PORT, COM5_RX_GPIO_PORT, COM6_RX_GPIO_PORT};
const uint32_t	COM_USART_CLK[] 	= {COM1_CLK, COM2_CLK,	COM3_CLK,	COM4_CLK,	COM5_CLK,	COM6_CLK};
const uint32_t	COM_TX_PORT_CLK[] 	= {COM1_TX_GPIO_CLK,	COM2_TX_GPIO_CLK, COM3_TX_GPIO_CLK, COM4_TX_GPIO_CLK, COM5_TX_GPIO_CLK, COM6_TX_GPIO_CLK}; 
const uint32_t	COM_RX_PORT_CLK[] 	= {COM1_RX_GPIO_CLK,	COM2_RX_GPIO_CLK, COM3_RX_GPIO_CLK, COM4_RX_GPIO_CLK, COM5_RX_GPIO_CLK, COM6_RX_GPIO_CLK};
const uint16_t	COM_TX_PIN[] 		= {COM1_TX_PIN,	COM2_TX_PIN, COM3_TX_PIN, COM4_TX_PIN, COM5_TX_PIN, COM6_TX_PIN};
const uint16_t	COM_RX_PIN[] 		= {COM1_RX_PIN,	COM2_RX_PIN, COM3_RX_PIN, COM4_RX_PIN, COM5_RX_PIN, COM6_RX_PIN}; 
const uint8_t	COM_TX_PIN_SOURCE[] = {COM1_TX_SOURCE,	COM2_TX_SOURCE,	COM3_TX_SOURCE,	COM4_TX_SOURCE,	COM5_TX_SOURCE, COM6_TX_SOURCE};
const uint8_t	COM_RX_PIN_SOURCE[] = {COM1_RX_SOURCE,	COM2_RX_SOURCE,	COM3_RX_SOURCE,	COM4_RX_SOURCE,	COM5_RX_SOURCE, COM6_RX_SOURCE}; 
const uint8_t	COM_TX_AF[] 		= {COM1_TX_AF,	COM2_TX_AF,	COM3_TX_AF,	COM4_TX_AF,	COM5_TX_AF,	COM6_TX_AF}; 
const uint8_t	COM_RX_AF[] 		= {COM1_RX_AF,	COM2_RX_AF,	COM3_RX_AF,	COM4_RX_AF,	COM5_RX_AF,	COM6_RX_AF};
const uint32_t	COM_USART_IRQ[]		= {COM1_IRQn,	COM2_IRQn,	COM3_IRQn,	COM4_IRQn,	COM5_IRQn,	COM6_IRQn};
*/

/** * @brief	Delay Function.	*/
void Sleep(__IO uint32_t ms)
{
	ms *= 10000;
	while(ms > 0)
		ms --;
}
/** * @brief	This function is prepared for main() to set irqs with different priorities,
	*			except btns. btns irq will be set in btn_init() with the lowest priority.*/
void NVIC_Config(uint32_t irq, uint32_t pree, uint32_t subp)
{
	NVIC_InitTypeDef nvid_init;
	nvid_init.NVIC_IRQChannel = irq;
	nvid_init.NVIC_IRQChannelPreemptionPriority = pree;
	nvid_init.NVIC_IRQChannelSubPriority = subp;
	nvid_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvid_init);
}
/** * @brief	Configures LED GPIO. */
void LED_Init(void)
{
	GPIO_InitTypeDef gpio_init;
	LONG i = 0;
	while (LEDs[i].port != NULL)
	{
		/* Enable the GPIO_LED Clock */
		RCC_AHB1PeriphClockCmd(LEDs[i].clock, ENABLE);
		/* Configure the GPIO_LED pin */
		gpio_init.GPIO_Mode 	= GPIO_Mode_OUT;
		gpio_init.GPIO_OType 	= GPIO_OType_PP;
		gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
		gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
		gpio_init.GPIO_Pin 		= LEDs[i].pin;
		GPIO_Init(LEDs[i].port, &gpio_init);
		i ++;
	}
}
/**	* @brief	Turns selected LED On/Off/Toggle.
	* @param	led: Specifies the led to be set on. 
	*/
void LED_Switch(LED_INDEX led, int32_t value)
{
	switch (value)
	{
		case 1:	LEDs[led].port->BSRRL = LEDs[led].pin; 	break;
		case 0:	LEDs[led].port->BSRRH = LEDs[led].pin;	break;
		case -1:LEDs[led].port->ODR  ^= LEDs[led].pin;	break;
	}
}
/**	* @brief	Configures Button GPIO and EXTI Line. */
void BTN_Init(void)
{
	GPIO_InitTypeDef gpio_init;
	EXTI_InitTypeDef exti_init;
	NVIC_InitTypeDef nvic_init;
	LONG i = 0;

	while (BTNs[i].port != NULL)
	{
		/* Enable the Button Clock */
		RCC_AHB1PeriphClockCmd(BTNs[i].clock, ENABLE);
		/* Configure Button pin as input */
		gpio_init.GPIO_Mode	= GPIO_Mode_IN;
		gpio_init.GPIO_PuPd	= GPIO_PuPd_UP;
		gpio_init.GPIO_Pin	= BTNs[i].pin;
		GPIO_Init(BTNs[i].port, &gpio_init);
		if (BTNs[i].irq != 0x7F)
		{
			/* Enable SYSCFG clock */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
			/* Connect Button EXTI Line to Button GPIO Pin */
			SYSCFG_EXTILineConfig(BTNs[i].iport, BTNs[i].ipin);
			/* Configure Button EXTI line */
			exti_init.EXTI_LineCmd	= ENABLE;
			exti_init.EXTI_Mode		= EXTI_Mode_Interrupt;
			exti_init.EXTI_Line		= BTNs[i].iline;
			if(BTNs[i].trig == 0)
				exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
			else if (BTNs[i].trig == 1)
				exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
			else
				exti_init.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			EXTI_Init(&exti_init);
			/* Enable and set Button EXTI Interrupt to the lowest priority */
			nvic_init.NVIC_IRQChannelCmd = ENABLE;
			nvic_init.NVIC_IRQChannelPreemptionPriority = 0x0F;
			nvic_init.NVIC_IRQChannelSubPriority = 0x0F;
			nvic_init.NVIC_IRQChannel = BTNs[i].irq;
			NVIC_Init(&nvic_init); 
		}
		i ++;
	}
}
/**	* @brief	Configures Button GPIO and EXTI Line. */
void JOY_Init(void)
{
	GPIO_InitTypeDef gpio_init;
	EXTI_InitTypeDef exti_init;
	NVIC_InitTypeDef nvic_init;
	LONG i = 0;

	while (JOYs[i].port != NULL)
	{
		/* Enable the Button Clock */
		RCC_AHB1PeriphClockCmd(JOYs[i].clock, ENABLE);
		/* Configure Button pin as input */
		gpio_init.GPIO_Mode	= GPIO_Mode_IN;
		gpio_init.GPIO_PuPd	= GPIO_PuPd_UP;
		gpio_init.GPIO_Pin	= JOYs[i].pin;
		GPIO_Init(JOYs[i].port, &gpio_init);
		if (JOYs[i].irq != 0xFF)
		{
			/* Enable SYSCFG clock */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
			/* Connect Button EXTI Line to Button GPIO Pin */
			SYSCFG_EXTILineConfig(JOYs[i].iport, JOYs[i].ipin);
			/* Configure Button EXTI Line */
			exti_init.EXTI_LineCmd	= ENABLE;
			exti_init.EXTI_Mode		= EXTI_Mode_Interrupt;
			exti_init.EXTI_Line		= JOYs[i].iline;
			if(JOYs[i].trig == 0)
				exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
			else if (JOYs[i].trig == 1)
				exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
			else
				exti_init.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			EXTI_Init(&exti_init);
			/* Enable and set Button EXTI Interrupt to the lowest priority */
			nvic_init.NVIC_IRQChannelCmd = ENABLE;
			nvic_init.NVIC_IRQChannelPreemptionPriority = 0x0F;
			nvic_init.NVIC_IRQChannelSubPriority = 0x0F;
			nvic_init.NVIC_IRQChannel = JOYs[i].irq;
			NVIC_Init(&nvic_init); 
		}
		i ++;
	}
}
/**	* @brief	Returns the selected Button state.
	* @retval	The Button GPIO pin value.
	*/
BTN_STATE BTN_GetState(BTN_INDEX btn)
{
	if (GPIO_ReadInputDataBit(BTNs[btn].port, BTNs[btn].pin) == BTN_GPIO_RST)
		return BTN_UP;
	else
		return BTN_DOWN;
}
/**	* @brief  Returns the current Joystick status.
	* @param  None
	* @retval The index of the Joystick key: 
	*   @arg  JOY_N
	*   @arg  JOY_S
	*   @arg  JOY_A
	*   @arg  JOY_B
	*   @arg  JOY_C
	*   @arg  JOY_D
	*/
JOY_INDEX JOY_GetState(void)
{
	JOY_INDEX i = (JOY_INDEX)0;
	while (i < JOY_N && GPIO_ReadInputDataBit(JOYs[i].port, JOYs[i].pin) != BTN_GPIO_SET)
		i ++;
	return i;
}
/**	* @brief	Configures COM port.
	* @param	COM: Specifies the COM port to be configured.
	*/
void USER_COM_Init(COM_INDEX com)
{
	GPIO_InitTypeDef gpio_init;
	USART_InitTypeDef uart_init; 

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(COMs[com].rxgclk | COMs[com].txgclk, ENABLE);

	/* Enable USART clock */
	if (com == COM_1 || com == COM_6)
		RCC_APB2PeriphClockCmd(COMs[com].afclk, ENABLE);
	else	
		RCC_APB1PeriphClockCmd(COMs[com].afclk, ENABLE);

	/* Connect PXx to USARTx_Tx/USARTx_Rx */
	GPIO_PinAFConfig(COMs[com].rxport, COMs[com].rxsrc, COMs[com].afnum);
	GPIO_PinAFConfig(COMs[com].txport, COMs[com].txsrc, COMs[com].afnum);

	gpio_init.GPIO_Mode		= GPIO_Mode_AF;
	gpio_init.GPIO_Speed	= GPIO_Speed_50MHz;
	/* Configure USART Tx as alternate function	*/
	gpio_init.GPIO_OType	= GPIO_OType_PP;
	gpio_init.GPIO_PuPd		= GPIO_PuPd_UP;
	gpio_init.GPIO_Pin		= COMs[com].txpin;
	GPIO_Init(COMs[com].txport, &gpio_init);
	/* Configure USART Rx as alternate function	*/
	gpio_init.GPIO_OType	= GPIO_OType_OD;
	gpio_init.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	gpio_init.GPIO_Pin		= COMs[com].rxpin;
	GPIO_Init(COMs[com].rxport, &gpio_init);

	/* USART configuration */
	uart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_init.USART_BaudRate 	= COMs[com].brate;
	uart_init.USART_WordLength	= USART_WordLength_8b;
	uart_init.USART_StopBits	= USART_StopBits_1;
	uart_init.USART_Parity		= USART_Parity_No;
	uart_init.USART_Mode		= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COMs[com].comx, &uart_init);
	/* Enable USART */
	USART_Cmd(COMs[com].comx, ENABLE);
}
/** * @brief  Configures the used Timers. */
void USER_TIM_Init(TIM_INDEX tim)
{ 
	TIM_TimeBaseInitTypeDef tim_init;
	TIM_OCInitTypeDef  		tim_oc;
	
	/* Enable TIMx clocks */
	RCC_APB1PeriphClockCmd(TIMs[tim].clock, ENABLE);

	/* TIMx configuration */        
	tim_init.TIM_Prescaler = ((SystemCoreClock / TIMs[tim].scaler) - 1);
	tim_init.TIM_ClockDivision = 0x0;    
	tim_init.TIM_CounterMode = TIMs[tim].cmode;  
	tim_init.TIM_Period = TIMs[tim].period;  
	TIM_TimeBaseInit(TIMs[tim].timx, &tim_init);
	
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCStructInit(&tim_oc);
	tim_oc.TIM_OCMode = TIMs[tim].ocmode;
	tim_oc.TIM_Pulse = 0x0;
	TIM_OC1Init(TIMs[tim].timx, &tim_oc);
	
	/* Immediate load of Precaler values */
	TIM_PrescalerConfig(TIMs[tim].timx, ((SystemCoreClock / TIMs[tim].scaler) - 1), TIM_PSCReloadMode_Immediate);
	/* Clear TIMx update pending flags */
	TIM_ClearFlag(TIMs[tim].timx, TIMs[tim].tflag);

	/* Enable the TIMx Interrupt */
	//nvic_init.NVIC_IRQChannel = TIMs[tim].irq;
	//nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	//nvic_init.NVIC_IRQChannelSubPriority = 0;
	//nvic_init.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&nvic_init);

	/* Enable TIMx Update interrupts */
	TIM_ITConfig(TIMs[tim].timx, TIMs[tim].isrc, ENABLE);
	/* TIMx enable counters */
	TIM_Cmd(TIMs[tim].timx, ENABLE);
}
/**	* @brief	Configures the CAN.
	* @param	None
	* @retval 	None
	* @note		The sample pos =(1 + BS1) / (1 + BS1 + BS2),
	*			it should be 75% when Baudrate > 500kbps, 90% when Baudrate < 300kbps.
	*/
void USER_CAN_Init(CAN_INDEX can)
{
	GPIO_InitTypeDef		gpio_init;
	CAN_InitTypeDef			can_init;
	CAN_FilterInitTypeDef 	can_filter;

	//Enable GPIO clock
	RCC_AHB1PeriphClockCmd(CANs[can].gclk, ENABLE);
	//Connect CAN pins to AF9
	GPIO_PinAFConfig(CANs[can].port, CANs[can].rxsrc, CANs[can].afnum);
	GPIO_PinAFConfig(CANs[can].port, CANs[can].txsrc, CANs[can].afnum); 
	
	//Configure CAN RX and TX pins
	gpio_init.GPIO_Pin 		= CANs[can].rxpin | CANs[can].txpin;
	gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
	gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
	gpio_init.GPIO_OType 	= GPIO_OType_PP;
	gpio_init.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_Init(CANs[can].port, &gpio_init);
	
	//Enable CAN clock
	RCC_APB1PeriphClockCmd(CANs[can].afclk, ENABLE);
	//Reset CAN registers
	CAN_DeInit(CANs[can].canx);

	//CAN cell init
	can_init.CAN_TTCM 	= DISABLE;	//time trigger mode
	can_init.CAN_ABOM 	= DISABLE;	//auto offline
	can_init.CAN_AWUM 	= DISABLE;	//auto wakeup
	can_init.CAN_NART 	= DISABLE;	//no auto retransmit
	can_init.CAN_RFLM 	= DISABLE;	//receive fifo lock when overrun
	can_init.CAN_TXFP 	= DISABLE;	//use transmit fifo priority
	can_init.CAN_MODE 	= CAN_Mode_Normal;
	can_init.CAN_SJW 	= CAN_SJW_1tq;
	can_init.CAN_BS1 	= CANs[can].bs1;
	can_init.CAN_BS2 	= CANs[can].bs2;
	can_init.CAN_Prescaler = CANs[can].scaler;
	CAN_Init(CANs[can].canx, &can_init);

	//CAN filter init
	can_filter.CAN_FilterMode 		= CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale 		= CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh 	= 0x0000;
	can_filter.CAN_FilterIdLow 		= 0x0000;
	can_filter.CAN_FilterMaskIdHigh = 0x0000;
	can_filter.CAN_FilterMaskIdLow 	= 0x0000;
	can_filter.CAN_FilterActivation = ENABLE;
	can_filter.CAN_FilterFIFOAssignment = 0;
	if (can == CAN_1)
		can_filter.CAN_FilterNumber = 0;
	else
		can_filter.CAN_FilterNumber = 14;
	CAN_FilterInit(&can_filter);
	
	//Enable FIFO 0 message pending Interrupt
	CAN_ITConfig(CANs[can].canx, CAN_IT_FMP0, ENABLE);
}
void USER_CAN_Transmit(CAN_INDEX can, uint16_t val)
{
	uint8_t  mbox;
	//send the message & wait until complete.
	CAN_TxMsgTypeDef msg;
	msg.StdId	= val & 0x7FF;
	msg.ExtId 	= 0x01;
	msg.RTR		= CAN_RTR_DATA;
	msg.IDE		= CAN_ID_STD;
	msg.DLC		= 2;
	msg.Data[0]	= val;
	msg.Data[1]	= (val>>8);	 
	mbox = CAN_Transmit(CANs[can].canx, &msg);
	while ((CAN_TransmitStatus(CANs[can].canx, mbox) != CANTXOK))
		;
}
