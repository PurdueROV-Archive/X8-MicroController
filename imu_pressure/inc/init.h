/* The .h file that will store the function declarations of initialization functions */
#ifndef __INIT_H
#define __INIT_H

#include "main.h"


extern TIM_MasterConfigTypeDef sMasterConfig;
extern TIM_OC_InitTypeDef sConfigOC;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

void initEverything(void);

/* function declarations */
void SystemClock_Config(void);
void Error_Handler(void);


void MX_GPIO_Init(void);
void MX_USART3_UART_Init(void);

/* initializes timer settings for pwm */
void initPwm(void);

/* configurres the I2C with DMA */
void MX_I2C1_Init(void);

/* configures the DMA nested vector interrupt priorities */
void MX_DMA_Init(void);

/* configures the can bus */
void initCan(void);

/*function for initializing and using leds */
void initDebugLeds(void);
void LedOn(int ledNum);
void LedOff(int ledNum);
void LedToggle(int ledNum);



void HAL_MspInit(void);

#endif