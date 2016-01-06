/*This folder will be used to hold all code used to initialize pins */

#include "main.h"


GPIO_InitTypeDef  GPIO_InitStruct;
TIM_OC_InitTypeDef sConfig;/* Timer Output Compare (technical name for PWM) */
TIM_HandleTypeDef    TimHandle;/* Timer handler*/

void initPwm(void)
{
  __TIM2_CLK_ENABLE();

  /* Computes the prescaler */
  uint32_t uhPrescalerValue = (uint32_t)(SystemCoreClock / 1000000);

  /*TIMx defined in main.h*/
  TimHandle.Instance = TIMx;

  TimHandle.Init.Prescaler         = uhPrescalerValue;  //scales down the master clock
  TimHandle.Init.Period            = 20000;  //creates the period of the pwm cycle in clock ticks
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;

  //initializes part of the pwm
  HAL_TIM_PWM_Init(&TimHandle);

  /*configures pwm settings */
  sConfig.OCMode        = TIM_OCMODE_PWM1;
  sConfig.OCPolarity     = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode  = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity   = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

  //IMPORTANT!! sets the duty cycle
  sConfig.Pulse = 50;

  //initializes the pwm
  HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
}

//configures the system clcok
void SystemClock_Config(void)
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

void Error_Handler(void)
{
  while(1)
  {
	  
  }
}