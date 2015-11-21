
#include "commons.h"
#include "canbus.h"



int main() {

    HAL_Init(); /* Called before anything related to HAL */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2); /* 2 pre 2 sub priority configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0); /* SysTick priority (highest) */
    SysTick_Config(SystemCoreClock / SYSTICK_FREQ); /* Configure SysTick as SYSTICK_FREQ */

    InitializeCANBUS1();

    //uint8_t CANBUSTxPacket[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; /* Create CANBUS data packet */
    uint8_t CANBUSTxPacket[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x53}; /* Create CANBUS data packet */
    //uint8_t CANBUSTxPacket[] = {0x01}; /* Create CANBUS data packet */

    int loop = 1;
    while (loop) {

	/* Insert a 500ms delay */
	HAL_Delay(500);

        CAN1SendMessage(8, &CANBUSTxPacket[0]); // Send CAN data packet
    }

    return 0;
}


/* Interrupt handler of core SysTick */
static int interruptCount = 0;



/* main.c */
