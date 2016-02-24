//put all of your #includes into main.h file
 #include "main.h"
 #include "motor.h"
 #include "print.h"
 
 /* structure used to initialize the gpio pin */
 GPIO_InitTypeDef  GPIO_InitStruct;
 

void setMotors(uint8_t* thrusts);

 
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
 
 	//motor    thruster(&hi2c1, 8);

 	uint8_t thrusts[8] = {0x0102, 0x0304, 0x0506, 0x0708,0x0102, 0x0304,0x0506, 0x0708};
			//3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
 
 	int i = 0;
 	while (1) {
 
 
 			/*thruster.set(7000);
 			thruster.update();
 
 			printString("ESC: ");
 			if(thruster.isAlive())
 				printString("ok\t\t");
 			else
 				printString("NA");
 
 			printInt(thruster.voltage());printString(" V\t\t");
 			printInt(thruster.current());printString(" A\t\t");
 			printInt(thruster.temperature());printString("  C\n");*/
 			
 			setMotors(thrusts);	
 
 			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
 
 		    HAL_Delay(1000);
 
 	  }
 }

 void setMotors(uint8_t* thrusts)
 {
 	/*uint8_t packet[19] = {0x12, thrusts[0] >> 8 , thrusts[0],
						  		thrusts[1] >> 8 , thrusts[1],
						  		thrusts[2] >> 8 , thrusts[2],
						  		thrusts[3] >> 8 , thrusts[3],
						  		thrusts[4] >> 8 , thrusts[4],
						  		thrusts[5] >> 8 , thrusts[5],
 	 							thrusts[6] >> 8 , thrusts[6],
						  		thrusts[7] >> 8 , thrusts[7], 0x13};*/
	 uint8_t packet[] = {1,2, 3, 4, 5, 6, 0, 0, 0};


 	HAL_I2C_Master_Transmit_DMA(&hi2c1, 0x44 << 1, packet, sizeof(packet));


 }