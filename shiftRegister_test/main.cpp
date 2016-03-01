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



	shiftRegister hydraulics(GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6, GPIOA, GPIO_PIN_9);


	  //configures the led pin  
	  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6| GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


		//A9 is the latach pin
		//A6 is the clock pin
		//A5 is the data pin

		int i = 0;
		while (1) {


			HAL_Delay(100);

			hydraulics.control(i++);





	  }
}



