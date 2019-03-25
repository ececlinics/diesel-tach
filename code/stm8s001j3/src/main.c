/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
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
#include "stm8s.h"
#include "iostm8s001.h"
#include "binary_filt.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void resetGPIO(void);

/* Private functions ---------------------------------------------------------*/
void resetGPIO(void)
{
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	GPIO_DeInit(GPIOF);
}

uint8_t i = 0;
uint8_t period;

void main(void)
{
  disableInterrupts();
	WWDG_CR &= ~WWDG_CR_WDGA; // disable window watchdog
	
	// GPIO
	resetGPIO();
	PA_DDR |= GPIO_PIN_1;
	PA_DDR &= ~GPIO_PIN_0;
	
	// HSI (16MHz) CLK
	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, 
												CLK_SOURCE_HSI, 
												DISABLE, 
												CLK_CURRENTCLOCKSTATE_DISABLE);
	//CLK_HSICmd(ENABLE);
	
	// TIM2
	TIM2_DeInit();
	TIM2_ITConfig(TIM2_IT_CC1, ENABLE);
	TIM2_PrescalerConfig(TIM2_PRESCALER_8, TIM2_PSCRELOADMODE_UPDATE);
	TIM2_SetCompare1(500);
	
	enableInterrupts();
	
	/* Infinite loop */
  while (1)
  {
		wfi();
  }
  
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

//#pragma vector = TIM2_OVR_UIF_vector
//__interrupt void TIM2_OVR_IQRHandler( void )
INTERRUPT_HANDLER(TIM2_OVR_IQRHandler,TIM2_OVR_UIF_vector)
{
	int period = calc_period(dyn_window_filt(PA_ODR & GPIO_PIN_0));
	int i;
	
	// software PWM
	if(i >= period >> 1)
		GPIO_WriteLow(GPIOA, GPIO_PIN_1);
	else
		GPIO_WriteHigh(GPIOA, GPIO_PIN_1);

	// increment i
	if(i > period - 1)
		i = 0;
	else
		i++;
	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
