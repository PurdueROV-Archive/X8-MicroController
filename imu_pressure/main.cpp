//put all of your #includes into main.h file
#include "main.h"

#include "print.h"
#include "imu.h"
#include "pressure.h"


/*CAN2 GPIO Configuration    
    PB5  ------> CAN2_RX
    PB6  ------> CAN2_TX 
		* CAN_HandleTypeDef hcan2 		 	*/

/*I2C1 GPIO Configuration    
    PB7  ------> I2C1_SDA
    PB8  ------> I2C1_SCL 
		* I2C_HandleTypeDef hi2c1;  		*/

/*TIM5 GPIO Configuration    
    PA0  ------> TIM5_CH1
		* Main Camera servo
		* TIM_OC_InitTypeDef sConfigOC;
		* TIM_HandleTypeDef htim5
		* TIM_CHANNEL_1

    PA3  ------> TIM5_CH4
		* Alternate camera servo
		* TIM_OC_InitTypeDef sConfigOC;
		* TIM_HandleTypeDef htim5
		* TIM_CHANNEL_4						*/

/*USART1 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    	* UART_HandleTypeDef huart3;		*/


/* 	USEFULL FUNCTIONS

	RED; BLUE; GREEN; YELLOW

	void LedOn(int ledNum);
	void LedOff(int ledNum);
	void LedToggle(int ledNum);
*/

/*
	Example how to send can code

	CanHandle.pTxMsg->DLC = 3; //sets the size of the message in bytes. Max 8 bytes per message

	//sets the information that is sent over the message
	CanHandle.pTxMsg->Data[0] = 5;
    CanHandle.pTxMsg->Data[1] = 246;
    CanHandle.pTxMsg->Data[2] = 17;

	HAL_CAN_Transmit(&hcan2, 10);  //sends the message
*/


/* structure used to initialize the gpio pin */
GPIO_InitTypeDef  GPIO_InitStruct;
//I2C_HandleTypeDef hi2c1;

/* Variables used in the motor controlling code */

int main(void) {


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

	printString("Creating IMU Object\r\n");

	// IMU init
	IMU imu = IMU(&hi2c1);
	
	printString("Creating Pressure Object\r\n");
	// pressure init
	Pressure pressure = Pressure(ADDRESS_HIGH, &hi2c1);
	printString("Creating Pressure Object created\r\n");
	pressure.reset();
	printString("Creating Pressure Object reset\r\n");
	pressure.begin();
	printString("Creating Pressure Object begun\r\n");

	while (1) {
		//printString("While\n");
		// Update piController's sensor data and compute its PID modulated output to the Rotational force vector.
		imu.get_linear_accel(); // Gets linear movement
		imu.retrieve_euler(); // Gets angular movement

		printDouble(imu.lX());
		//piController.sensorInput(vect3Make((int16_t) (imu.rX() * 1000), (int16_t) (imu.rY() * 1000), (int16_t) (imu.rZ() * 1000)), 
		//	vect3Make(0,0,0/*(int16_t) (imu.aX() * 1000), (int16_t) (imu.aY() * 1000), (int16_t) (imu.aZ() * 1000)*/), HAL_GetTick());
		//force_output.R = piController.getOutput();

		// Pressure Sensor:
		 // Returns mbar pressure from sensor.
		printString(", ");
		printDouble(pressure.getPressure(ADC_4096));

		printString("\n");
		HAL_Delay(100);
	}

}
