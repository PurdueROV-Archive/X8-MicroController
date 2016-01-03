//put all of your #includes into main.h file
#include "main.h"
#include "shiftRegister.h"

/* structure used to initialize the gpio pin */
GPIO_InitTypeDef  GPIO_InitStruct;


int main(void)  {
	  
    //must be included to initially configure the library
	  HAL_Init();
	  SystemClock_Config();
	  
	 //enable the led clock
	 __HAL_RCC_GPIOA_CLK_ENABLE();
		
  
	  //configures the led pin  
	  GPIO_InitStruct.Pin = GPIO_PIN_5;  //pin 5
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin A5 


	  while (1) {

		    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		
		    // 500ms delay
		    HAL_Delay(500);
	  }
}



