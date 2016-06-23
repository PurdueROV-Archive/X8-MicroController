//put all of your #includes into main.h file
#include "main.h"

#include "temp.h"
#include "print.h"

/* structure used to initialize the gpio pin */
GPIO_InitTypeDef  GPIO_InitStruct;
//I2C_HandleTypeDef hi2c1;


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
	HAL_I2C_MspInit(&hi2c1);

	initPrint();

	Temp tempSensor(&hi2c1);
	tempSensor.init();
	while (1) {

		tempSensor.read();
		printDouble(tempSensor.rawInfo);
		printString("  ");
printDouble(tempSensor.data[0]);
		printString("  ");
printDouble(tempSensor.data[1]);
		printString("  ");
printDouble(tempSensor.data[2]);
		printString("  ");
		printDouble(tempSensor.getTemp());

		printString("\n");
		HAL_Delay(50);
	}
}



