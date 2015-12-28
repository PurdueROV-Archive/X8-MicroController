#include "main.h"

int i = 1;

int prescaller  = 1;
int period = 1000 * i;
int pulse = 500 * i;


int main(void)  
{
	//does some of the initializations
	initGPIO();
	/* computes the prescalar */
	uhPrescalerValue = (uint32_t)(SystemCoreClock / prescaller) - 1;



	//this is how we set how fast ticks happen (pwm is measured in clock ticks not seconds) and we set the length of the period of the alternating pulse width
	TimHandle.Init.Prescaler         = uhPrescalerValue;
	TimHandle.Init.Period            = period;





	TimHandle.Instance = TIMx;
	TimHandle.Init.ClockDivision     = 0;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	//some other random initializations
	HAL_TIM_PWM_Init(&TimHandle);
	initPWM();
	initPrint();



	//this code changes the length of the on pulse for the pwm. the length is measured in clock ticks yet agian.
	sConfig.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) ;



	while (1)
	{

	}
}


