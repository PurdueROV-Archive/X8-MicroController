#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

/* Definition for USARTx clock resources */
// TODO: THIS MAY NEED TO BE CHANGED?! TOOK THIS FROM OLD CODE TO GET IT TO COMPILE.
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

#include "main.h"
#include "stm32f4xx_hal_uart.h"

/* UART handler declaration */
extern UART_HandleTypeDef UartHandle;

void initPrint(UART_HandleTypeDef handler);

void printInt(int num);
void printDouble(double num);
void printString(char* phrase);
