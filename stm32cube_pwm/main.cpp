#include "main.h"

#define  PERIOD_VALUE       (uint32_t)(666 - 1)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE)        /* Capture Compare 1 Value  */




/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;

static void SystemClock_Config(void);
static void Error_Handler(void);

static GPIO_InitTypeDef  GPIO_InitStruct;
  
  

int main(void)  
{
	//must be included to initially configure the library
	HAL_Init();
	  
	/* Configure the system clock to 100 MHz */
	SystemClock_Config();
	
	//enable the led clock
	 __HAL_RCC_GPIOA_CLK_ENABLE();
		
	
	GPIO_InitTypeDef GPIO_InitStruct;
 

	__TIM2_CLK_ENABLE();
  
	/**TIM2 GPIO Configuration    
	PA5     ------> TIM2_CH1 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  

	  
	/* Compute the prescaler value to have TIM8 counter clock equal to 16000000 Hz */
	uhPrescalerValue = (uint32_t)(SystemCoreClock / 16000000) - 1;
  
	/* Initialize TIMx peripheral as follows:
		+ Prescaler = (SystemCoreClock / 16000000) - 1
		+ Period = (666 - 1)
		+ ClockDivision = 0
		+ Counter direction = Up
	*/
	TimHandle.Instance = TIMx;

	TimHandle.Init.Prescaler         = uhPrescalerValue;
	TimHandle.Init.Period            = PERIOD_VALUE;
	TimHandle.Init.ClockDivision     = 0;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	
	if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
	{
		
		Error_Handler();
	}

	/*##-2- Configure the PWM channels ##*/
	/* Common configuration for all channels */
	sConfig.OCMode        = TIM_OCMODE_PWM1;
	sConfig.OCPolarity     = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode  = TIM_OCFAST_DISABLE;
	sConfig.OCNPolarity   = TIM_OCNPOLARITY_HIGH;
	sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

	/* Set the pulse value for channel 1 */
	sConfig.Pulse = PULSE1_VALUE;
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	
	/*##-3- Start PWM signals generation ##*/
	/* Start channel 1 */
	if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
	{
		
		Error_Handler();
	}



	while (1)
	{
		
		HAL_Delay(500);
		sConfig.Pulse = PULSE1_VALUE / 4;
		HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) ;
		HAL_Delay(500);
		sConfig.Pulse = PULSE1_VALUE;
		HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) ;
		
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
