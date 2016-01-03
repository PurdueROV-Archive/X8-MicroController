/* Put all of your #includes and #defines up here   */

#include "esc.h"


esc::esc(TIM_HandleTypeDef* HTIM, TIM_OC_InitTypeDef* SCONFIG, uint32_t CHANNEL)
{
    timerHandler = HTIM;
    sConfig = SCONFIG;
    channel = CHANNEL;

}

void esc::set(uint8_t dutyCycle)
{
    sConfig->Pulse = timerHandler->Init.Period * dutyCycle / 255;

    HAL_TIM_PWM_ConfigChannel(timerHandler, sConfig, channel);
    HAL_TIM_PWM_Start(timerHandler, channel) ;
}



