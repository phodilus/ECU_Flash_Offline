/*
	******************************************************************************
	* @file		CAN/Networking/main.c 
	* @author	MCD Application Team
	* @version V1.1.0
	* @date		13-April-2012
	* @brief	 Main program body
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


//-------------------------------------------------------------------
#include "stm32f2xx.h"
#include "main.h"
#include "stm322xg_eval.h"

void LED_Display(uint8_t status)
{
	//Turn off all leds
	LED_Switch(LED_1, 0);
	LED_Switch(LED_2, 0);
	LED_Switch(LED_3, 0);
	LED_Switch(LED_4, 0);
	
	switch(status)
	{
		case(1): LED_Switch(LED_1, 1);	break;
	 	case(2): LED_Switch(LED_2, 1);	break;
 		case(3): LED_Switch(LED_3, 1);	break;
		case(4): LED_Switch(LED_4, 1);	break;
		default: break;
	}
}
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//4:0
	LED_Init();
	LED_Switch(LED_1, 0);
	LED_Switch(LED_2, 0);
	LED_Switch(LED_3, 1);//OK
	LED_Switch(LED_4, 0);//NG
	BTN_Init();
	USER_CAN_Init(CAN_1);
	//NVIC_Config(CANs[CAN_1].rx0irq, 0, 0);
	NVIC_Config(SDIO_IRQn, 1, 1);
	#ifdef _SD_WAIT_DMA
	NVIC_Config(SDIO_DMA_IRQn, 2, 2);
	#endif
	while (1);
}
//The end ---------------------------------------------------------*/
#ifdef	USE_FULL_ASSERT
/**	* @brief	Reports the name of the source file and the source line number
	*			where the assert_param error has occurred.
	* @param	file: pointer to the source file name
	* @param	line: assert_param error line source number
	* @retval	None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	//User can add his own implementation to report the file name and line number,
	//ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)

	//Infinite loop
	while (1)
	{
	}
}
#endif
