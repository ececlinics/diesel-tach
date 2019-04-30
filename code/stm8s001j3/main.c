/**
  ******************************************************************************
  * @file    main.c 
  * @author  Damon Boorstein
  * @date    24-Mar-2019
  * @brief   Main source file
   ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "iostm8s001.h"
#include "binary_filt.h"

/* Private defines -----------------------------------------------------------*/
#define _GPIO 1
#define _TIM2 1
#define INITIAL_PULSE 0 // --- Damon 4/16/19: What's a good default PWM width? --- Damon 4/29/19: Using 0 for now
#define RELOAD_VALUE 0	// --- Damon 4/29/19: What's a good reload value for the watchdog?

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

//uint8_t i = 0;
uint8_t period;

void main(void)
{
  disableInterrupts();
	/* ----- Damon 4/16/19: Need to enable/reset Independent Watchdog (IWDG) periph in this loop ----- */
	IWDG_SetReload();
	IWDG_ReloadCounter();
	
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
	/* ----- Damon 4/16/19: This code was used for software PWM, we need HARDWARE PWM ------------------------
	TIM2_ITConfig(TIM2_IT_CC1, ENABLE);
	TIM2_SetCompare1(500);
	*/
	TIM2_PrescalerConfig(TIM2_PRESCALER_8, TIM2_PSCRELOADMODE_UPDATE);
	TIM2_PWMIConfig(TIM2_CHANNEL_1,							// Config PWM input on Channel 1
									TIM2_ICPOLARITY_RISING,			// polarity = rising edge
									TIM2_ICSELECTION_DIRECTTI,	// ----- Damon 4/29/19: I'm not sure about what type of polarity,
									TIM2_ICPSC_DIV8,						//					whether we need direct, or indirect, what divider we
									0);						//						//					need. I don't think we need a filter (I put 0 for that arg)
	TIM2_OC3Init( TIM2_OCMODE_PWM1,					// Initialize output on channel 3
								TIM2_OUTPUTSTATE_ENABLE,
								INITIAL_PULSE,						// INITIAL_PULSE = 0
								TIM2_OCPOLARITY_LOW);
	TIM2_SelectOCxM(TIM2_CHANNEL_3, TIM2_OCMODE_PWM1); // Select output compare mode PWM1 on channel 3 (pin 5)
	TIM2_CCxCmd(TIM2_CHANNEL_3, ENABLE); // Enable capture compare on channel 3
	TIM2_Cmd(ENABLE); // Enable TIM2
	/* ^--------------------------------------TIM2 hardware PWM----------------------------------------------^ */
	
	enableInterrupts();
	IWDG_Enable(); // enable IWDG
	
	/* Infinite loop */
  while (1)
  {
		// --- Damon 4/29/19: After interrupt, return here, set new pwm pulse width, enable TIM2 CH3
		// -----------------------------------------------------------------------------------------
		TIM2_CCxCmd(TIM2_CHANNEL_3, DISABLE); // disable channel 3
		TIM2_OC3Init( TIM2_OCMODE_PWM1,					// Initialize output on channel 3
									TIM2_OUTPUTSTATE_ENABLE,
									period,										// update pwm period with new period
									TIM2_OCPOLARITY_LOW);
		TIM2_CCxCmd(TIM2_CHANNEL_3, ENABLE); // re-enable CC on channel 3
		// ^---------------------------------------------------------------------------------------^
		
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

// ------ Damon 4/16/19: interrupt handler for TIM2 PWM --------
INTERRUPT_HANDLER(TIM2_OVR_IQRHandler,TIM2_OVR_UIF_vector)
{
	/*int*/ period = calc_period(dyn_window_filt(PA_ODR & GPIO_PIN_0));
	//int i;
	
	// software PWM
/*if(i >= period >> 1)
		GPIO_WriteLow(GPIOA, GPIO_PIN_1);
	else
		GPIO_WriteHigh(GPIOA, GPIO_PIN_1); */

	// increment i
	if(i > period - 1)
		i = 0;
	else
		i++;
		
	// --- Damon 4/29/19: clear TIM2 interupt flag here
}

/*****END OF FILE****/
