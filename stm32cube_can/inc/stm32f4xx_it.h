/*This file is used to include the function declarations of all interrupts that we will be using

*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void CAN1_RX0_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);
void CAN1_RX1_IRQHandler(void);
void CAN2_RX1_IRQHandler(void);
void CAN1_TX_IRQHandler(void);
void CAN2_TX_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */
