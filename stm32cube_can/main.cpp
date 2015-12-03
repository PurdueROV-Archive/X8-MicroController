
#include "commons.h"

uint8_t CAN1dataReceivedFlag;


CAN_HandleTypeDef HCAN1;
CanTxMsgTypeDef TxMessage; /* Tx message struct */
CanRxMsgTypeDef RxMessage; /* Rx message struct */

void InitializeCANBUS1();
void CAN1SendMessage(uint8_t length, uint8_t *data);

void InitializeLEDS();
void LedState(uint16_t led, uint8_t state);

#define LED_GREEN GPIO_PIN_12
#define LED_ORANGE GPIO_PIN_13
#define LED_RED GPIO_PIN_14
#define LED_BLUE GPIO_PIN_15

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

	/* Insert a 500ms delay */
	    HAL_Delay(500);

        CAN1SendMessage(8, &CANBUSTxPacket[0]); // Send CAN data packet
        LedState(GPIO_PIN_12, 2);
    }

    return 0;
}


/* Interrupt handler of core SysTick */
//static int interruptCount = 0;


void InitializeCANBUS1()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    CAN_FilterConfTypeDef CAN_FilterStruct;


    __HAL_RCC_CAN1_CLK_ENABLE(); /* Enable CANBUS-1 clock */

    __GPIOD_CLK_ENABLE(); /* Enable GPIOD clock */


    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* Alternate Function Push-Pull mode */
    GPIO_InitStruct.Pull = GPIO_NOPULL; /* No resistor */
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1; /* Alternate pin function: AF9 / CAN1 */
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_0; /* PD1(CAN1_TX) & PD0(CAN1_RX) */
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); /* Initialize registers */


    HCAN1.Instance = CAN1;
    HCAN1.pTxMsg = &TxMessage; /* Pointer to CAN Tx message */
    HCAN1.pRxMsg = &RxMessage; /* Pointer to CAN Rx message */
    HCAN1.Init.Prescaler = 19; /* Time quanta prescaler (tq = (Prescaler+1)/42MHz) */
    HCAN1.Init.Mode = CAN_MODE_NORMAL; /* Normal communication mode */
    HCAN1.Init.SJW = CAN_SJW_4TQ; /* 4 time-quanta re-sync jump width */
    HCAN1.Init.BS1 = CAN_BS1_9TQ; /* 9 tq before sampling point */
    HCAN1.Init.BS2 = CAN_BS2_8TQ; /* 8 tq after sampling point (before end) */
    HCAN1.Init.TTCM = DISABLE; /* Time-triggered communication */
    HCAN1.Init.ABOM = DISABLE; /* Automatic bus-off management */
    HCAN1.Init.AWUM = DISABLE; /* Automatic wake-up mode */
    HCAN1.Init.NART = DISABLE; /* Non-automatic retransmission mode */
    HCAN1.Init.RFLM = DISABLE; /* Receive FIFO locked mode */
    HCAN1.Init.TXFP = DISABLE; /* Transmit FIFO priority */
    HAL_CAN_Init(&HCAN1); /* Init CAN1 registers */


    CAN_FilterStruct.FilterIdHigh = 0x0000; /* Upper 16bit filter ID */
    CAN_FilterStruct.FilterIdLow = 0x0000; /* Filter lower 16bit ID */
    CAN_FilterqStruct.FilterMaskIdHigh = 0x0000; /* Upper 16bit filter mask */
    CAN_FilterStruct.FilterMaskIdLow = 0x0000; /* Lower 16bit filter mask */
    CAN_FilterStruct.FilterFIFOAssignment = CAN_FILTER_FIFO0; /* Which FIFO will be assigned to filter */
    CAN_FilterStruct.FilterNumber = 0;
    CAN_FilterStruct.FilterMode = CAN_FILTERMODE_IDMASK; /* Identifier mask mode */
    CAN_FilterStruct.FilterScale = CAN_FILTERSCALE_32BIT; /* 32bit ID filter */
    CAN_FilterStruct.FilterActivation = ENABLE; /* Enable this filter */
    CAN_FilterStruct.BankNumber = 14; /* Start slave bank filter (?) */
    HAL_CAN_ConfigFilter(&HCAN1, &CAN_FilterStruct); /* Initialize filter */


    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 1); /* Set CAN1 Rx interrupt priority to 1-1 */
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn); /* Enable CAN1 Rx Interrupt */

    __HAL_CAN_ENABLE_IT(&HCAN1, CAN_IT_FMP0); /* Enable 'message pending in FIFO0' interrupt */

    HCAN1.pTxMsg->StdId = 0x14; /* Standard ID of this device */
    HCAN1.pTxMsg->ExtId = 0x01; /* Extended ID */
    HCAN1.pTxMsg->RTR = CAN_RTR_DATA; /* Remote transmission request:data 0x00000000 */
    HCAN1.pTxMsg->IDE = CAN_ID_STD; /* Identifier type: standard 0x00000000 */

    return;
}


void CAN1SendMessage(uint8_t length, uint8_t *data)
{
    HCAN1.pTxMsg->DLC = length; /* Specify the data length */

    uint8_t d;

    for(d = 0; d < length; ++d)
    {
        HCAN1.pTxMsg->Data[d] = *(data + d); /* Write every byte to Data[] variable */
    }

    HAL_CAN_Transmit(&HCAN1, 3000); /* Transmit the packet */

    return;
}


/* CAN1 Rx FIFO0 interrupt service routine */
void CAN1_RX0_IRQHandler()
{
    if(HAL_CAN_Receive(&HCAN1, 0, 5000) == HAL_OK) /* Packet received from CANBUS successfully */
    {
        if(RxMessage.Data[0] == 0xAA && RxMessage.Data[2] == 0x55) /* Sync bytes are correct */
        {
            LedState(LED_BLUE, 1); /* Blue LED ON */

            CAN1dataReceivedFlag = 1; /* Set data received flag */
        }
    }

    return;
}

void InitializeLEDS()
{
    __GPIOD_CLK_ENABLE(); /* Enable GPIOD clock for leds */

    GPIO_InitTypeDef GPIO_Leds;

    GPIO_Leds.Mode = GPIO_MODE_OUTPUT_PP; /* Output push-pull mode */
    GPIO_Leds.Pull = GPIO_NOPULL; /* No resistor */
    GPIO_Leds.Speed = GPIO_SPEED_HIGH;
    GPIO_Leds.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; /* PD12(Green) PD13(Orange) PD14(Red) PD15(Blue) */
    HAL_GPIO_Init(GPIOD, &GPIO_Leds); /* Init GPIOD registers */


    return;
}


void LedState(uint16_t led, uint8_t state)
{
    switch(state)
    {
        case 1: /* ON */
            HAL_GPIO_WritePin(GPIOD, led, GPIO_PIN_SET);
            break;
        case 0: /* OFF */
            HAL_GPIO_WritePin(GPIOD, led, GPIO_PIN_RESET);
            break;
        case 2: /* Toggle */
            HAL_GPIO_TogglePin(GPIOD, led);
            break;
    }

    return;
}

