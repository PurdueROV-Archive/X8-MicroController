#include "main.h"
/*
    First off note that the function that get trigerred when there is a packet received with interrupts
    is at the bottom of the code right after the main loop gets over. This function gets triggered 
    not mater what the board is doing and the code inside of it gets triggered. We will be using this
    way of receiving can commands for the final project so look a lot at the interrupt portion.
 */

/**CAN1 GPIO Configuration
       PD0     ------> CAN1_RX
       PD1     ------> CAN1_TX
 */



CAN_HandleTypeDef CanHandle;

static CanTxMsgTypeDef        TxMessage;
static CanRxMsgTypeDef        RxMessage;

/* These #defines are used to include or not include certain portions of code.
   Example, if you #define SEND then the code will be in sending mode, if you
   do not include it, then it will be in not send, or receive mode. The interrupt 
   mode determines whether the code uses the while loop the receive commands or whether 
   it uses the commands in an interrupt. */

//#define SEND
#define INTERRUPT



void SystemClock_Config(void);

void initLEDS(void);

static void Error_Handler(int one, int two);

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan);


HAL_StatusTypeDef CAN_init(void);

int main() {

    HAL_Init(); /* Called before anything related to HAL */
    SystemClock_Config(); /* Configure SysTick as SYSTICK_FREQ */

    initLEDS();  //initializes the four leds on the discovery board

    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

    //initialize the can
    if(CAN_init() != HAL_OK)  Error_Handler(100, 100);

//initializes the interrupt if you #define it at the top
#ifdef INTERRUPT
     //initialize the interrupt function to handle receiving the can receives
  if(HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0) != HAL_OK)  Error_Handler(500, 500);
#endif



    while (1) {

    /* Insert a 500ms delay */
        HAL_Delay(200);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

//include this code if you are sending
#ifdef SEND
       //send a message, if the message fails, turn on a light and then blink another light
       if(HAL_CAN_Transmit(&CanHandle, 10) != HAL_OK)
        {
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
            Error_Handler(2000, 100);
        }

//else include this code which is used in receiving
#else

//if we are receiving and not using an interrupt
#ifndef INTERRUPT

        //receive a can message
        if(HAL_CAN_Receive(&CanHandle, CAN_FIFO0,10) == HAL_OK)
        {
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
        }

        //wait until the can state is ready meaning a message has been received
        if(HAL_CAN_GetState(&CanHandle) != HAL_CAN_STATE_READY)
        {
            //Error_Handler(100, 500);
        }

        if(CanHandle.pRxMsg->StdId != 0x11)
        {
            //Error_Handler(5000, 5000);
        }

        if(CanHandle.pRxMsg->IDE != CAN_ID_STD)
        {
            //Error_Handler(100, 0);
        }

        if(CanHandle.pRxMsg->DLC != 8)
        {
            //Error_Handler(0, 100);
        }

        //By the way DLC stands for Data Length
        for(int i = CanHandle.pRxMsg->DLC; i < 8; i++)
        {
           if( CanHandle.pRxMsg->Data[i] != 1)
           {
               //Error_Handler(1000, 1000);
           }
        }
//end for not interrupt portion of code
#endif

//end of send or receive portion of code
#endif
    }

    return 0;
}

//VERY IMPORTANT FUNCTION!!!!!!!

//function used to receive the can command via an interrupt
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle)
{
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

    //checks if the address received was correct
    if ((CanHandle->pRxMsg)->StdId == 0x11 && (CanHandle->pRxMsg)->IDE == CAN_ID_STD)
    {
        //if the data length is 8
        if((CanHandle->pRxMsg)->DLC == 8)
        {
            //checks if each value is 1, because that's what I send it
            for(int i = (CanHandle->pRxMsg)->DLC; i < 8; i++)
            {
                if( (CanHandle->pRxMsg)->Data[i] != 1)
                {
                    //Error_Handler(1000, 1000);
                }
            }
            //blink the orange light if a message containing all ones is received
           // HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
        }
        else
        {
            //Error_Handler(1000, 1000);
        }
    }
    else
    {
        //Error_Handler(1000, 1000);
    }

    //checks the interrupt handler to see if it recieved everything correctly
    if(HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0) != HAL_OK)
    {
        /* Reception Error */
        //Error_Handler(2000, 2000);
    }
}

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;

    __PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//sets up the pins for can to be alternate functions
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(hcan->Instance==CAN2)
    {

        
        /* Peripheral clock enable */
        __CAN2_CLK_ENABLE();
        __CAN1_CLK_ENABLE();



        /**CAN1 GPIO Configuration
        PD0     ------> CAN1_RX
        PD1     ------> CAN1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral interrupt init*/
        HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
    }

}

HAL_StatusTypeDef CAN_init(void)
{

    CAN_FilterConfTypeDef  sFilterConfig;


    /*##-1- Configure the CAN peripheral #######################################*/
    CanHandle.Instance = CAN2;
    CanHandle.pTxMsg = &TxMessage;
    CanHandle.pRxMsg = &RxMessage;


    //standard settings for can bus
    CanHandle.Init.TTCM = DISABLE;
    CanHandle.Init.ABOM = DISABLE;
    CanHandle.Init.AWUM = DISABLE;
    CanHandle.Init.NART = DISABLE;
    CanHandle.Init.RFLM = DISABLE;
    CanHandle.Init.TXFP = DISABLE;
    CanHandle.Init.Mode = CAN_MODE_NORMAL;
    CanHandle.Init.SJW = CAN_SJW_1TQ;
    CanHandle.Init.BS1 = CAN_BS1_6TQ;
    CanHandle.Init.BS2 = CAN_BS2_8TQ;
    CanHandle.Init.Prescaler = 2;

    //if can is unable to initialize
    if(HAL_CAN_Init(&CanHandle) != HAL_OK)
    {
        while(1);
    }

    /*##-2- Configure the CAN Filter ###########################################*/
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 0;

    if(HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
    {
        while(1);
    }

    //sets up the communication information
    CanHandle.pTxMsg->StdId = 0x11;  //the id of this microboard
    

    CanHandle.pTxMsg->RTR = CAN_RTR_DATA;
    CanHandle.pTxMsg->IDE = CAN_ID_STD;



    CanHandle.pTxMsg->DLC = 8;  //data length; how many bytes of data you are sending. This number is always less than 8
    
    uint8_t num = 1;  //test number we will be sending

    //the bytes of data that are sent
    CanHandle.pTxMsg->Data[0] = num;
    CanHandle.pTxMsg->Data[1] = num;
    CanHandle.pTxMsg->Data[2] = num;
    CanHandle.pTxMsg->Data[3] = num;
    CanHandle.pTxMsg->Data[4] = num;
    CanHandle.pTxMsg->Data[5] = num;
    CanHandle.pTxMsg->Data[6] = num;
    CanHandle.pTxMsg->Data[7] = num;



    return HAL_OK; /* Test Passed */
}


//function used to handle errors
static void Error_Handler(int one, int two)
{
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
        HAL_Delay(one);

        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
        HAL_Delay(two);
    }
}

//codes that initializes the leds for the discovery board, if using the black board, change what pins are initialized
void initLEDS(void)
{
    /* structure used to initialize the gpio pin */
    static GPIO_InitTypeDef  GPIO_InitStruct;

    //enable the led clock
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    //configures the led pin
    GPIO_InitStruct.Pin = GPIO_PIN_12;  //pin 5
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);  //initializes the pin A5 based on the settings your created earlier

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

