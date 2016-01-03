/* The .h file that will store the function declarations of initialization functions */
#ifndef __INIT_H
#define __INIT_H

#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

/* function declarations */
void SystemClock_Config(void);
void Error_Handler(void);

void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_I2C1_Init(void);



#endif