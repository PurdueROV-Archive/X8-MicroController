#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_it.h"
#include "init.h"
#include "print.h"


#define TIMx					TIM2
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOA

#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_5

#define TIMx_GPIO_AF_CHANNEL1          GPIO_AF3_TIM2


#endif /* __MAIN_H */