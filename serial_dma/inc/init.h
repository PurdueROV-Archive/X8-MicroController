/* The .h file that will store the function declarations of initialization functions */
#ifndef __INIT_H
#define __INIT_H

#include "main.h"

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern UART_HandleTypeDef huart2;

void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_USART2_UART_Init(void);

/* function declarations */
void SystemClock_Config(void);
void Error_Handler(void);

void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_USART2_UART_Init(void);

#endif