/* The .h file that will store the function declarations of initialization functions */
#ifndef __INIT_H
#define __INIT_H

#include "main.h"

extern TIM_OC_InitTypeDef sConfig;/* Timer Output Compare (technical name for PWM) */
extern TIM_HandleTypeDef    TimHandle;/* Timer handler declaration */

/* Counter Prescaler value */
extern uint32_t uhPrescalerValue;

void initGPIO(void);
void initPWM(void);

void SystemClock_Config(void);
void Error_Handler(void);

#endif