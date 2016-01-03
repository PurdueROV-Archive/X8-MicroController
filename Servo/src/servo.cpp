/* Put all of your #includes and #defines up here   */

/*
 *   int prescaller  = 1000000;
 *   int period = 3333;
 *   int pulse = 500;
 *
 *   //this is how we set how fast ticks happen (pwm is measured in clock ticks not seconds) and we set the length of the period of the alternating pulse width
 *	 TimHandle.Init.Prescaler         = uhPrescalerValue;
 *	 TimHandle.Init.Period            = period;
 *
 */



#include "servo.h"


servo::servo(TIM_HandleTypeDef* HTIM, TIM_OC_InitTypeDef* SCONFIG, uint32_t CHANNEL)
{
    timerHandler = HTIM;
    sConfig = SCONFIG;
    channel = CHANNEL;

}

void servo::set(uint8_t dutyCycle)
{
    sConfig->Pulse = timerHandler->Init.Period * dutyCycle / 255;

    HAL_TIM_PWM_ConfigChannel(timerHandler, sConfig, channel);
    HAL_TIM_PWM_Start(timerHandler, channel) ;
}



