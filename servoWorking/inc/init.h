/* The .h file that will store the function declarations of initialization functions */
#ifndef __INIT_H
#define __INIT_H

#include "main.h"

extern GPIO_InitTypeDef  GPIO_InitStruct;
extern TIM_OC_InitTypeDef sConfig;/* Timer Output Compare (technical name for PWM) */
extern TIM_HandleTypeDef    TimHandle;/* Timer handler*/


void SystemClock_Config(void);
void Error_Handler(void);

void initPwm(void);

#endif