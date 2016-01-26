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

    startPosition = 0.5;
    range = 1.5;

}

//sets the angle of the servo from 0 to 180 degrees
void servo::set(uint8_t angle)
{
    if(angle > 180)
       angle = 180;

    sConfig->Pulse = ((double) timerHandler->Init.Period / 20.0) * range * angle / 180 + ((double) timerHandler->Init.Period / 20.0) * startPosition;

    HAL_TIM_PWM_ConfigChannel(timerHandler, sConfig, channel);
    HAL_TIM_PWM_Start(timerHandler, channel) ;
}

//sets the starting pulse length that will corespond to an angle of 0 in miliseconds
void servo::setStart(double start)
{
    startPosition = start;
}

//sets the range that the pulse length will vary in mili seconds
void servo::setRange(double Range)
{
    range = Range;
}



