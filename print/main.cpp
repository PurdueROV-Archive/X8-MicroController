#include "main.h"

#include "print.h"  //this library includes the code to initialize the serial used to print and the print functions


int main(void) {
	
	//must be included to initially configure the library
	HAL_Init();
	SystemClock_Config();

	//initializes the print funcctions
	initPrint();


	while (1) {
		
		for(double i = 1; i < 10; i+= 0.1) {


			// /HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);


			printDouble(i);
			printInt((int) i);
			printString("\n");

			HAL_Delay(100);
		}
	}
}










