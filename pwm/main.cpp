#include "main.h"


/*structures used in initializing the pins */
GPIO_InitTypeDef  GPIO_InitStruct;
TIM_OC_InitTypeDef sConfig;/* Timer Output Compare (technical name for PWM) */
TIM_HandleTypeDef    TimHandle;/* Timer handler*/


int main(void)  
{
	//must be included to initially configure the library
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__TIM2_CLK_ENABLE();


	/*this initializes the pin A5 to be act as its alternate functions */
	GPIO_InitStruct.Pin = GPIO_PIN_5; //pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  //puts it in Alterate Fucntion mode (AF mode)
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;  //puts it in AF1_TIM2 mode (pwm mode)
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin as an alternate function
					

	  
	/* Computes the prescaler */
	uint32_t uhPrescalerValue = (uint32_t)(SystemCoreClock / 16000000);

	/*TIMx defined in main.h*/
	TimHandle.Instance = TIMx;

	TimHandle.Init.Prescaler         = uhPrescalerValue;  //scales down the master clock 
	TimHandle.Init.Period            = 1000;  //creates the period of the pwm cycle in clock ticks
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



	uint8_t brightness = 0;

	while (1)
	{
		//brightness increases until full brightness and then restarts
		sConfig.Pulse = brightness++;  
		HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) ;

		HAL_Delay(5);
	}
}
