//put all of your #includes into main.h file
#include "main.h"
#include "motor.h"
#include "print.h"

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

		MX_GPIO_Init();
		MX_DMA_Init();
		MX_I2C1_Init();

	initPrint();

	motor    thruster(&hi2c1, 8);

	int i = 0;
	while (1) {


			thruster.set(7000);
			thruster.update();

			printString("ESC: ");
			if(thruster.isAlive())
				printString("ok\t\t");
			else
				printString("NA");

			printInt(thruster.voltage());printString(" V\t\t");
			printInt(thruster.current());printString(" A\t\t");
			printInt(thruster.temperature());printString("  C\n");


			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		    HAL_Delay(500);

	  }
}



