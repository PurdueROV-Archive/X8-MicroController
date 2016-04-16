#include "main.h"
#include "servo.h"

int main(void)  
{
	//must be included to initially configure the library
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_GPIOA_CLK_ENABLE();



	/*this initializes the pin A5 to be act as its alternate functions */
	GPIO_InitStruct.Pin = GPIO_PIN_5; //pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  //puts it in Alterate Fucntion mode (AF mode)
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;  //puts it in AF1_TIM2 mode (pwm mode)
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin as an alternate function

	initPwm();


	servo led(&TimHandle, &sConfig, TIM_CHANNEL_1);

	//led.set(0);

	while (1)
	{
		led.set(0); //2.24ms
		HAL_Delay(5000);
		led.set(180); //0.76ms
		HAL_Delay(5000);
		led.set(0);
		HAL_Delay(5000);
		led.set(180);
		HAL_Delay(5000);
		/*led.set(120);
		HAL_Delay(1000);
		led.set(180);
		HAL_Delay(1000);*/

	}
}
