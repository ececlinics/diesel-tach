/**
  ******************************************************************************
  * @file    src/main.c 
  * @author  Damon
  * @date    24-Mar-2019
  * @brief   Main source file
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
	// WWDG_CR &= ~WWDG_CR_WDGA; // disable window watchdog
	
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

/*****END OF FILE****/
