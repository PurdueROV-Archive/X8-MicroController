#include "print.h"

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

void initPrint(UART_HandleTypeDef handler)
{
    UartHandle = handler;
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



