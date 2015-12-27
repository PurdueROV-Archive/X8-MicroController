#include "main.h"


#define  PERIOD_VALUE       (uint32_t)(666 - 1)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE)        /* Capture Compare 1 Value  */


static void SystemClock_Config(void);
static void Error_Handler(void);

/*structures used in initializing the pins */

static GPIO_InitTypeDef  GPIO_InitStruct;
TIM_OC_InitTypeDef sConfig;/* Timer Output Compare (technical name for PWM) */
TIM_HandleTypeDef    TimHandle;/* Timer handler declaration */

/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;




/*anything that does not have a comment next to it can be ignored
 * it means that it is not important enough or will never change
 * so there is no reason to put a comment next to it */

int main(void)  
{
	//must be included to initially configure the library
	HAL_Init();
	  
	/* Configure the system clock to 100 MHz */
	SystemClock_Config();

	//enable the led clock
	 __HAL_RCC_GPIOA_CLK_ENABLE();
		

	//enables the clock used by the pwm timer
	__TIM2_CLK_ENABLE();


	/*this initializes the pin A5 to be act as its alternate functions
	 * which is pwm (GPIO_AF1_TIM2) */
	GPIO_InitStruct.Pin = GPIO_PIN_5; //pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  //puts it in Alterate Fucntion mode (AF mode)
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;  //puts it in AF1_TIM2 mode (pwm mode)
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin as an alternate function
											//you still have to initialize the pwm portion
	  

	  
	/* Compute the prescaler value to have TIM8 counter clock equal to 16000000 Hz */
	uhPrescalerValue = (uint32_t)(SystemCoreClock / 16000000) - 1;
  
	/*Many of the variables such as TIMx are constants defined in main.h
	 * anything variable capitalized is a #define and it is located in
	 * the main.h file*/
	TimHandle.Instance = TIMx;

	TimHandle.Init.Prescaler         = uhPrescalerValue;  //scales down the master clock to get the clock that the pwm is using
	TimHandle.Init.Period            = PERIOD_VALUE;  //creates the period of the pwm cycle (the period is in clock ticks one clock tick is an increment of one on the period
	TimHandle.Init.ClockDivision     = 0;  //no clock division after the prescaling
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;  //the pwm's counter will count up to the maximum period value
	TimHandle.Init.RepetitionCounter = 0;

	//if this portion does not initialize correctly, go into an error handling state
	if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
	{
		Error_Handler();
	}

	/*configures how the pwm settings and it's duty cycle */
	sConfig.OCMode        = TIM_OCMODE_PWM1;  //puts it into generic pwm mode
	sConfig.OCPolarity     = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode  = TIM_OCFAST_DISABLE;
	sConfig.OCNPolarity   = TIM_OCNPOLARITY_HIGH;
	sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;


	sConfig.Pulse = PULSE1_VALUE; /* Set the pulse value for channel 1 */

	//checks if these settings initialize correctly
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	
	/*checks if the pwm starts correctly*/
	if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}


	while (1)
	{
		/*this code puts a new value as the pulse length (the value in the pwm period when the line switches from high
		 * to low and then reconfigures the pwm and restarts it to apply the settings
		 */

		sConfig.Pulse = PULSE1_VALUE / 4;  //sets the brightness to 1/4
		HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) ;
		HAL_Delay(500);

		sConfig.Pulse = PULSE1_VALUE;  //sets the brightness to full
		HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) ;
		HAL_Delay(500);
	}
}

//configures the system clcok
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

static void Error_Handler(void)
{
  while(1)
  {
	  
  }
}
