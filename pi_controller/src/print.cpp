#include "main.h"

#include "print.h"

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

void initPrint(void)
{
    static GPIO_InitTypeDef  GPIO_InitStruct;

    //enable the led clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    //configures the led pin
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



    /*##-1- Enable peripherals and GPIO Clocks ##*/
    /* Enable GPIO TX/RX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();

    /* Enable USARTx clock */
    USARTx_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO##*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;

    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;

    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);


    /*##-1- Configure the UART peripheral ######################################*/

    UartHandle.Instance          = USARTx;

    UartHandle.Init.BaudRate     = 115200;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        /* Initialization Error */

    }

}



void printInt(int num)
{
    char buffer [33];

    itoa(num, buffer, 10);

    printString(buffer);

}

void printDouble(double num)
{
    char buffer[50];

    sprintf(buffer,"%.4f", num);

    printString(buffer);
}

void printString(char* phrase)
{
    for(int i = 0; phrase[i] != '\0'; i++)
    {
        HAL_UART_Transmit(&UartHandle,(uint8_t*)&phrase[i],1,10);
    }

}



