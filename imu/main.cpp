//put all of your #includes into main.h file
#include "main.h"

#include "imu.h"
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

	//uint8_t buffer[2] = {2, 3};
	imu imu1(&hi2c1);

	while (1) {

		//HAL_I2C_Master_Transmit_DMA(&hi2c1, (0x28 << 1), buffer, 2);
		if (imu1.retrieve()) {
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			printString("\tX: ");
			printDouble(imu1.getX());
			printString("\tY: ");
			printDouble(imu1.getY());
			printString("\tZ: ");
			printDouble(imu1.getZ());
			printString("\n");
		}


		// 50ms delay
		//HAL_Delay(50);
	}
}



