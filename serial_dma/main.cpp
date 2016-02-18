#include "main.h"

uint8_t buffer[2] = {'A', 'B'};

int main(void)  
{
	  //must be included to initially configure the library
	  HAL_Init();
	  SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();


    HAL_UART_Receive_DMA(&huart2, (uint8_t*)buffer, 2);
 
    while(buffer[1] == 'B')
    {
        HAL_Delay(100);
    }

	while (1)
	{
       if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buffer, 2) == HAL_OK)
       {
           HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
       }
       HAL_Delay(500);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_UART_Receive_DMA(UartHandle, (uint8_t*)buffer, 2);
}