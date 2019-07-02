/**
	******************************************************************************
	* @file	stm322xg_eval.h
	* @author	MCD Application Team
	* @version V5.0.3
	* @date	09-March-2012
	* @brief	This file contains definitions for STM322xG_EVAL's Leds, push-buttons
	*			and COM ports hardware resources.
	******************************************************************************
	* @attention
	*
	* <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
	*
	* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
	* You may not use this file except in compliance with the License.
	* You may obtain a copy of the License at:
	*
	*		http://www.st.com/software_license_agreement_liberty_v2
	*
	* Unless required by applicable law or agreed to in writing, software 
	* distributed under the License is distributed on an "AS IS" BASIS, 
	* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	* See the License for the specific language governing permissions and
	* limitations under the License.
	*
	******************************************************************************
	*/

#ifndef __STM322xG_EVAL_H
#define __STM322xG_EVAL_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f2xx.h"
#include "stm32_eval_legacy.h"
#include "integer.h"
	 
/** @defgroup STM322xG_EVAL_LOW_LEVEL_Exported_Types */
typedef struct
{
	GPIO_TypeDef*	port;	//GPIOA
	uint32_t		clock;	//RCC_AHB1Periph_GPIOA
	uint16_t 		pin;	//GPIO_Pin_4
} LED_PARAM;
typedef struct
{
	GPIO_TypeDef*	port;	//GPIOF
	uint32_t		clock;	//RCC_AHB1Periph_GPIOF
	uint16_t 		pin;	//GPIO_Pin_13
	uint32_t		iline;	//EXTI_Line13
	uint8_t			iport;	//EXTI_PortSourceGPIOF
	uint8_t			ipin;	//EXTI_PinSource13
	uint32_t		trig;	//0:falling, 1:rising, 2:both.
	uint32_t		irq;	//EXTI15_10_IRQn
} BTN_PARAM;
typedef struct
{
	USART_TypeDef*	comx;	//USART1
	uint32_t		afclk;	//RCC_APB2Periph_USART1
	uint8_t			afnum;	//GPIO_AF_USART1=7
	uint32_t		txgclk;	//RCC_AHB1Periph_GPIOA
	GPIO_TypeDef*	txport;	//GPIOA
	uint16_t 		txpin;	//GPIO_Pin_9
	uint8_t			txsrc;	//GPIO_PinSource9
	uint32_t		rxgclk;	//RCC_AHB1Periph_GPIOA
	GPIO_TypeDef*	rxport;	//GPIOA
	uint16_t 		rxpin;	//GPIO_Pin_10
	uint8_t			rxsrc;	//GPIO_PinSource10
	uint32_t		irq;	//USART1_IRQn
	uint32_t		brate;	//
} COM_PARAM;
typedef struct
{
	CAN_TypeDef*	canx;	//CAN1
	uint32_t		afclk;	//RCC_APB1Periph_CAN1
	uint8_t			afnum;	//GPIO_AF_CAN1=9
	uint32_t		gclk;	//RCC_AHB1Periph_GPIOD
	GPIO_TypeDef*	port;	//GPIOD
	uint16_t		rxpin;	//GPIO_Pin_0
	uint8_t			rxsrc;	//GPIO_PinSource0
	uint16_t		txpin;	//GPIO_Pin_1
	uint8_t			txsrc;	//GPIO_PinSource1
	uint32_t		rx0irq;	//CAN1_RX0_IRQn
	uint32_t		rx1irq;	//CAN1_RX1_IRQn
	uint8_t 		bs1;
	uint8_t			bs2;
	uint8_t			scaler;
} CAN_PARAM;
typedef struct
{
	TIM_TypeDef*	timx;	//TIM2
	uint32_t		clock;	//RCC_APB1Periph_TIM2
	uint32_t		scaler;	//1200
	uint16_t		cmode;	//TIM_CounterMode_Up
	uint32_t		period;	//2400-1
	uint16_t		ocmode;	//TIM_OCMode_Timing
	uint16_t		tflag;	//TIM_FLAG_Update
	uint16_t		isrc;	//TIM_IT_Update
	uint32_t		irq;	//TIM2_IRQn
} TIM_PARAM;

extern LED_PARAM LEDs[];
extern BTN_PARAM BTNs[];
extern BTN_PARAM JOYs[];
extern COM_PARAM COMs[];
extern CAN_PARAM CANs[];
extern TIM_PARAM TIMs[];
extern uint32_t m_uCanRecvID;

#define BTN_GPIO_SET	Bit_RESET
#define BTN_GPIO_RST	Bit_SET
typedef enum 
{
	LED_1 	= 0,
	LED_2 	= 1,
	LED_3 	= 2,
	LED_4 	= 3,
} LED_INDEX;
typedef enum
{
	BTN_DOWN	= 0,
	BTN_UP		= 1,
} BTN_STATE;
typedef enum 
{
	BTN_GPIOA3	= 0,
	BTN_WAKEUP	= 1,
	BTN_TAMPER	= 2,
	BTN_USER	= 3,
} BTN_INDEX;
typedef enum 
{	
	BTN_MODE_GPIO = 0,
	BTN_MODE_EXTI = 1,
} BTN_MODE;
typedef enum 
{ 
	JOY_C	= 2,
	JOY_B	= 1,
	JOY_D	= 3,
	JOY_A	= 0,
	JOY_S	= 4,
	JOY_N	= 5,
} JOY_INDEX;
typedef enum 
{
	CAN_1	= 0,
	CAN_2	= 1,
} CAN_INDEX;
typedef enum 
{
	COM_1	= 0,
	COM_2	= 1,
	COM_3	= 2,
	COM_4	= 3,
	COM_5	= 4,
	COM_6	= 5,
} COM_INDEX;
typedef enum 
{
	TIM_1	= 0,
	TIM_2	= 1,
	TIM_3	= 2,
	TIM_4	= 3,
	TIM_5	= 4,
	TIM_6	= 5,
	TIM_7	= 6,
	TIM_8	= 7,
	TIM_9	= 8,
	TIM_10	= 9,
	TIM_11	= 10,
	TIM_12	= 11,
	TIM_13	= 12,
	TIM_14	= 13,
} TIM_INDEX;

/** * @brief Define for STM322xG_EVAL board */ 
//#if !defined (USE_STM322xG_EVAL)
// #define USE_STM322xG_EVAL
//#endif

/*
//
#define LEDn						4
//
#define LED1_PIN					GPIO_Pin_4
#define LED1_GPIO_PORT				GPIOA
#define LED1_GPIO_CLK				RCC_AHB1Periph_GPIOA	
//
#define LED2_PIN					GPIO_Pin_5
#define LED2_GPIO_PORT				GPIOA
#define LED2_GPIO_CLK				RCC_AHB1Periph_GPIOA	
//
#define LED3_PIN					GPIO_Pin_6
#define LED3_GPIO_PORT				GPIOA
#define LED3_GPIO_CLK				RCC_AHB1Periph_GPIOA 
//
#define LED4_PIN					GPIO_Pin_7
#define LED4_GPIO_PORT				GPIOA
#define LED4_GPIO_CLK				RCC_AHB1Periph_GPIOA

//Joystick pins are connected to an IO Expander (accessible through I2C1 interface)
#define JOYn						5
#define JOY_SELECT_GPIO_CLK			RCC_AHB1Periph_GPIOE
#define JOY_SELECT_GPIO_PORT		GPIOE
#define JOY_SELECT_PIN				GPIO_Pin_11
#define JOY_SELECT_EXTI_LINE		EXTI_Line11
#define JOY_SELECT_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOE
#define JOY_SELECT_EXTI_PIN_SOURCE	EXTI_PinSource12
#define JOY_SELECT_EXTI_IRQn		EXTI15_10_IRQn 
#define JOY_UP_GPIO_CLK				RCC_AHB1Periph_GPIOE
#define JOY_UP_GPIO_PORT			GPIOE
#define JOY_UP_PIN					GPIO_Pin_12
#define JOY_UP_EXTI_LINE			EXTI_Line12
#define JOY_UP_EXTI_PORT_SOURCE		EXTI_PortSourceGPIOE
#define JOY_UP_EXTI_PIN_SOURCE		EXTI_PinSource12
#define JOY_UP_EXTI_IRQn			EXTI15_10_IRQn 
#define JOY_DOWN_GPIO_CLK			RCC_AHB1Periph_GPIOE
#define JOY_DOWN_GPIO_PORT			GPIOE
#define JOY_DOWN_PIN				GPIO_Pin_13
#define JOY_DOWN_EXTI_LINE			EXTI_Line13
#define JOY_DOWN_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOE
#define JOY_DOWN_EXTI_PIN_SOURCE	EXTI_PinSource13
#define JOY_DOWN_EXTI_IRQn			EXTI15_10_IRQn 
#define JOY_LEFT_GPIO_CLK			RCC_AHB1Periph_GPIOE
#define JOY_LEFT_GPIO_PORT			GPIOE
#define JOY_LEFT_PIN				GPIO_Pin_14
#define JOY_LEFT_EXTI_LINE			EXTI_Line14
#define JOY_LEFT_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOE
#define JOY_LEFT_EXTI_PIN_SOURCE	EXTI_PinSource14
#define JOY_LEFT_EXTI_IRQn			EXTI15_10_IRQn 
#define JOY_RIGHT_GPIO_CLK			RCC_AHB1Periph_GPIOE
#define JOY_RIGHT_GPIO_PORT			GPIOE
#define JOY_RIGHT_PIN				GPIO_Pin_15
#define JOY_RIGHT_EXTI_LINE			EXTI_Line15
#define JOY_RIGHT_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOE
#define JOY_RIGHT_EXTI_PIN_SOURCE	EXTI_PinSource15
#define JOY_RIGHT_EXTI_IRQn			EXTI15_10_IRQn 

//
#define BTNn						3
//Unknown push-button
#define BTN_GPIOA0_PIN				GPIO_Pin_0
#define BTN_GPIOA0_GPIO_PORT		GPIOA
#define BTN_GPIOA0_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define BTN_GPIOA0_EXTI_LINE		EXTI_Line0
#define BTN_GPIOA0_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOA
#define BTN_GPIOA0_EXTI_PIN_SOURCE	EXTI_PinSource0
#define BTN_GPIOA0_EXTI_IRQn		EXTI0_IRQn
//Wakeup push-button
#define BTN_WAKEUP_PIN				GPIO_Pin_13
#define BTN_WAKEUP_GPIO_PORT		GPIOF
#define BTN_WAKEUP_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define BTN_WAKEUP_EXTI_LINE		EXTI_Line13
#define BTN_WAKEUP_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOF
#define BTN_WAKEUP_EXTI_PIN_SOURCE	EXTI_PinSource13
#define BTN_WAKEUP_EXTI_IRQn		EXTI15_10_IRQn 
//Tamper push-button
#define BTN_TEMPER_PIN				GPIO_Pin_14
#define BTN_TEMPER_GPIO_PORT		GPIOF
#define BTN_TEMPER_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define BTN_TEMPER_EXTI_LINE		EXTI_Line14
#define BTN_TEMPER_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOF
#define BTN_TEMPER_EXTI_PIN_SOURCE	EXTI_PinSource14
#define BTN_TEMPER_EXTI_IRQn		EXTI15_10_IRQn 
//User push-button
#define BTN_USER_PIN				GPIO_Pin_15
#define BTN_USER_GPIO_PORT			GPIOF
#define BTN_USER_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define BTN_USER_EXTI_LINE			EXTI_Line15
#define BTN_USER_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOF
#define BTN_USER_EXTI_PIN_SOURCE	EXTI_PinSource15
#define BTN_USER_EXTI_IRQn			EXTI15_10_IRQn

//Definition for CAN port
#define CANn					2
//CAN1: PA11+PA12,PB8+PB9,PD0+PD1,PI9+PH13.
#define CAN1_CLK				RCC_APB1Periph_CAN1
#define CAN1_RX_PIN				GPIO_Pin_0
#define CAN1_TX_PIN				GPIO_Pin_1
#define CAN1_GPIO_PORT			GPIOD
#define CAN1_GPIO_CLK			RCC_AHB1Periph_GPIOD
#define CAN1_AF_PORT			GPIO_AF_CAN1
#define CAN1_RX_SOURCE			GPIO_PinSource0
#define CAN1_TX_SOURCE			GPIO_PinSource1
//CAN2: PB5+PB6,PB12+PB13.(DFU only use PB5+PB13)
#define CAN2_CLK				(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
#define CAN2_RX_PIN				GPIO_Pin_5
#define CAN2_TX_PIN				GPIO_Pin_6
#define CAN2_GPIO_PORT			GPIOB
#define CAN2_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define CAN2_AF_PORT			GPIO_AF_CAN2
#define CAN2_RX_SOURCE			GPIO_PinSource5
#define CAN2_TX_SOURCE			GPIO_PinSource13

//Definition for COM port
#define COMn					6
//
#define COM1					USART1
#define COM1_CLK				RCC_APB2Periph_USART1
#define COM1_TX_PIN				GPIO_Pin_9
#define COM1_TX_GPIO_PORT		GPIOA
#define COM1_TX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM1_TX_SOURCE			GPIO_PinSource9
#define COM1_TX_AF				GPIO_AF_USART1
#define COM1_RX_PIN				GPIO_Pin_10
#define COM1_RX_GPIO_PORT		GPIOA
#define COM1_RX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM1_RX_SOURCE			GPIO_PinSource10
#define COM1_RX_AF				GPIO_AF_USART1
#define COM1_IRQn				USART1_IRQn
//
#define COM2					USART2
#define COM2_CLK				RCC_APB1Periph_USART2
#define COM2_TX_PIN				GPIO_Pin_9
#define COM2_TX_GPIO_PORT		GPIOA
#define COM2_TX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM2_TX_SOURCE			GPIO_PinSource9
#define COM2_TX_AF				GPIO_AF_USART1
#define COM2_RX_PIN				GPIO_Pin_10
#define COM2_RX_GPIO_PORT		GPIOA
#define COM2_RX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM2_RX_SOURCE			GPIO_PinSource10
#define COM2_RX_AF				GPIO_AF_USART1
#define COM2_IRQn				USART2_IRQn
//
#define COM3					USART3
#define COM3_CLK				RCC_APB1Periph_USART3
#define COM3_TX_PIN				GPIO_Pin_9
#define COM3_TX_GPIO_PORT		GPIOA
#define COM3_TX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM3_TX_SOURCE			GPIO_PinSource9
#define COM3_TX_AF				GPIO_AF_USART1
#define COM3_RX_PIN				GPIO_Pin_10
#define COM3_RX_GPIO_PORT		GPIOA
#define COM3_RX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM3_RX_SOURCE			GPIO_PinSource10
#define COM3_RX_AF				GPIO_AF_USART1
#define COM3_IRQn				USART3_IRQn
//
#define COM4					UART4
#define COM4_CLK				RCC_APB1Periph_UART4
#define COM4_TX_PIN				GPIO_Pin_9
#define COM4_TX_GPIO_PORT		GPIOA
#define COM4_TX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM4_TX_SOURCE			GPIO_PinSource9
#define COM4_TX_AF				GPIO_AF_USART1
#define COM4_RX_PIN				GPIO_Pin_10
#define COM4_RX_GPIO_PORT		GPIOA
#define COM4_RX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM4_RX_SOURCE			GPIO_PinSource10
#define COM4_RX_AF				GPIO_AF_USART1
#define COM4_IRQn				UART4_IRQn
//
#define COM5					UART5
#define COM5_CLK				RCC_APB1Periph_UART5
#define COM5_TX_PIN				GPIO_Pin_9
#define COM5_TX_GPIO_PORT		GPIOA
#define COM5_TX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM5_TX_SOURCE			GPIO_PinSource9
#define COM5_TX_AF				GPIO_AF_USART1
#define COM5_RX_PIN				GPIO_Pin_10
#define COM5_RX_GPIO_PORT		GPIOA
#define COM5_RX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM5_RX_SOURCE			GPIO_PinSource10
#define COM5_RX_AF				GPIO_AF_USART1
#define COM5_IRQn				UART5_IRQn
//
#define COM6					USART6
#define COM6_CLK				RCC_APB2Periph_USART6
#define COM6_TX_PIN				GPIO_Pin_9
#define COM6_TX_GPIO_PORT		GPIOA
#define COM6_TX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM6_TX_SOURCE			GPIO_PinSource9
#define COM6_TX_AF				GPIO_AF_USART1
#define COM6_RX_PIN				GPIO_Pin_10
#define COM6_RX_GPIO_PORT		GPIOA
#define COM6_RX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define COM6_RX_SOURCE			GPIO_PinSource10
#define COM6_RX_AF				GPIO_AF_USART1
#define COM6_IRQn				USART6_IRQn
*/

//SD FLASH SDIO Interface
#define SDIO_FIFO_ADDRESS			((uint32_t)0x40012C80)
#define SDIO_INIT_CLK_DIV			((uint8_t)0x76)	//SDIO Intialization Frequency (400KHz max)
#define SDIO_TRANSFER_CLK_DIV		((uint8_t)0x00)	//SDIO Data Transfer Frequency (25MHz max)

#define SD_DETECT_PIN				GPIO_Pin_7
#define SD_DETECT_GPIO_PORT			GPIOC
#define SD_DETECT_GPIO_CLK			RCC_AHB1Periph_GPIOC

#define SD_SDIO_DMA					DMA2
#define SDIO_DMA_CLK				RCC_AHB1Periph_DMA2
#define USE_SDIO_STREAM3
#ifdef USE_SDIO_STREAM3
	#define SDIO_DMA_STREAM			DMA2_Stream3
	#define SDIO_DMA_CHANNEL		DMA_Channel_4
	#define SDIO_DMA_FLAG_FEIF		DMA_FLAG_FEIF3
	#define SDIO_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF3
	#define SDIO_DMA_FLAG_TEIF		DMA_FLAG_TEIF3
	#define SDIO_DMA_FLAG_HTIF		DMA_FLAG_HTIF3
	#define SDIO_DMA_FLAG_TCIF		DMA_FLAG_TCIF3
	#define SDIO_DMA_IRQn			DMA2_Stream3_IRQn
	#define SDIO_DMA_IRQHANDLER		DMA2_Stream3_IRQHandler	
#elif defined USE_SDIO_STREAM6
	#define SDIO_DMA_STREAM			DMA2_Stream6
	#define SDIO_DMA_CHANNEL		DMA_Channel_4
	#define SDIO_DMA_FLAG_FEIF		DMA_FLAG_FEIF6
	#define SDIO_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF6
	#define SDIO_DMA_FLAG_TEIF		DMA_FLAG_TEIF6
	#define SDIO_DMA_FLAG_HTIF		DMA_FLAG_HTIF6
	#define SDIO_DMA_FLAG_TCIF		DMA_FLAG_TCIF6
	#define SDIO_DMA_IRQn			DMA2_Stream6_IRQn
	#define SDIO_DMA_IRQHANDLER		DMA2_Stream6_IRQHandler	
#endif //USE_SDIO_STREAM3

#define sEE_I2C						I2C1
#define sEE_I2C_CLK					RCC_APB1Periph_I2C1
#define sEE_I2C_SCL_PIN				GPIO_Pin_6					/* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT		GPIOB						/* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define sEE_I2C_SCL_SOURCE			GPIO_PinSource6
#define sEE_I2C_SCL_AF				GPIO_AF_I2C1
#define sEE_I2C_SDA_PIN				GPIO_Pin_9					/* PB.09 */
#define sEE_I2C_SDA_GPIO_PORT		GPIOB						/* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define sEE_I2C_SDA_SOURCE			GPIO_PinSource9
#define sEE_I2C_SDA_AF				GPIO_AF_I2C1
#define sEE_M24C64_32

#define sEE_I2C_DMA					DMA1	
#define sEE_I2C_DMA_CHANNEL			DMA_Channel_1
#define sEE_I2C_DMA_STREAM_TX		DMA1_Stream6
#define sEE_I2C_DMA_STREAM_RX		DMA1_Stream0	
#define sEE_I2C_DMA_CLK				RCC_AHB1Periph_DMA1
#define sEE_I2C_DR_Address			((uint32_t)0x40005410)
#define sEE_USE_DMA
	
#define sEE_I2C_DMA_TX_IRQn			DMA1_Stream6_IRQn
#define sEE_I2C_DMA_RX_IRQn			DMA1_Stream0_IRQn
#define sEE_I2C_DMA_TX_IRQHandler	DMA1_Stream6_IRQHandler
#define sEE_I2C_DMA_RX_IRQHandler	DMA1_Stream0_IRQHandler	
#define sEE_I2C_DMA_PREPRIO			0
#define sEE_I2C_DMA_SUBPRIO			0	
	
#define sEE_TX_DMA_FLAG_FEIF		DMA_FLAG_FEIF6
#define sEE_TX_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF6
#define sEE_TX_DMA_FLAG_TEIF		DMA_FLAG_TEIF6
#define sEE_TX_DMA_FLAG_HTIF		DMA_FLAG_HTIF6
#define sEE_TX_DMA_FLAG_TCIF		DMA_FLAG_TCIF6
#define sEE_RX_DMA_FLAG_FEIF		DMA_FLAG_FEIF0
#define sEE_RX_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF0
#define sEE_RX_DMA_FLAG_TEIF		DMA_FLAG_TEIF0
#define sEE_RX_DMA_FLAG_HTIF		DMA_FLAG_HTIF0
#define sEE_RX_DMA_FLAG_TCIF		DMA_FLAG_TCIF0
	
#define sEE_DIRECTION_TX			0
#define sEE_DIRECTION_RX			1	

/* Time constant for the delay caclulation allowing to have a millisecond 
	incrementing counter. This value should be equal to (System Clock / 1000).
	ie. if system clock = 120MHz then sEE_TIME_CONST should be 120. */
#define sEE_TIME_CONST					120 

void Sleep(__IO uint32_t ms);
void NVIC_Config(uint32_t irq, uint32_t pree, uint32_t subp);
void LED_Init(void);
void LED_Switch(LED_INDEX led, int32_t value);
void BTN_Init(void);
BTN_STATE BTN_GetState(BTN_INDEX btn);
void JOY_Init(void);
JOY_INDEX JOY_GetState(void);
void USER_COM_Init(COM_INDEX com);
void USER_TIM_Init(void);
void USER_TIM_Enable(TIM_INDEX tim, BOOL enable);
void USER_CAN_Init(CAN_INDEX can);
void USER_CAN_Transmit(CAN_INDEX can, uint32_t id, uint32_t len, uint8_t* data);
LONG USER_CAN_Receive(CAN_INDEX can, uint32_t id, uint8_t* data, int32_t tmout);
	
#ifdef __cplusplus
}
#endif
#endif /* __STM322xG_EVAL_H */
