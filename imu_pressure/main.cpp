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



/* Variables used in the motor controlling code */

int main(void) {
	//initializes all of the pins!
	initEverything();



	// IMU init
	IMU imu = IMU(&hi2c1);

	// pressure init
	Pressure pressure = Pressure(ADDRESS_HIGH);
	pressure.reset();
	pressure.begin();

	while (1) {

		// Update piController's sensor data and compute its PID modulated output to the Rotational force vector.
		imu.get_linear_accel(); // Gets linear movement
		imu.retrieve_euler(); // Gets angular movement

		//piController.sensorInput(vect3Make((int16_t) (imu.rX() * 1000), (int16_t) (imu.rY() * 1000), (int16_t) (imu.rZ() * 1000)), 
		//	vect3Make(0,0,0/*(int16_t) (imu.aX() * 1000), (int16_t) (imu.aY() * 1000), (int16_t) (imu.aZ() * 1000)*/), HAL_GetTick());
		//force_output.R = piController.getOutput();

		// Pressure Sensor:
		// sensor.getPressure(ADC_4096); // Returns mbar pressure from sensor.

		HAL_Delay(1);
	}

}
