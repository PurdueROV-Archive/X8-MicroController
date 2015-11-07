/* STM32F4-CANBUS main.c
 * http://www.eraycanli.com */

#include "commons.h"

#include "leds.h"
#include "canbus.h"


int main() {

    HAL_Init(); /* Called before anything related to HAL */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2); /* 2 pre 2 sub priority configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0); /* SysTick priority (highest) */
    SysTick_Config(SystemCoreClock / SYSTICK_FREQ); /* Configure SysTick as SYSTICK_FREQ */

    InitializeLEDS();
    InitializeCANBUS1();

    //uint8_t CANBUSTxPacket[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; /* Create CANBUS data packet */
    uint8_t CANBUSTxPacket[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x53}; /* Create CANBUS data packet */
    //uint8_t CANBUSTxPacket[] = {0x01}; /* Create CANBUS data packet */

    int loop = 1;
    while (loop) {

        if (loop++ % 100) {
            for (int i = 0; i < 100; i++) {
                LedState(LED_GREEN, 2);
            }
        } else {
            LedState(LED_GREEN, 2);             //  Toggle green, every 100 messages sent.
            loop = 1;
        }

        CAN1SendMessage(8, &CANBUSTxPacket[0]); // Send CAN data packet
    }

    return 0;
}


/* Interrupt handler of core SysTick */
static int interruptCount = 0;



/* main.c */
